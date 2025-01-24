#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/i2c.h>
#include <zephyr/logging/log.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/gpio.h>


LOG_MODULE_REGISTER(Selfdirectedlearning, LOG_LEVEL_INF);

#define RGB_NODE DT_NODELABEL(rgbsensor)
const struct i2c_dt_spec RGB = I2C_DT_SPEC_GET(RGB_NODE);

#define SLEEP_TIME_MS 1000

#define BTN_NI DT_ALIAS(pb)
const struct gpio_dt_spec btn = GPIO_DT_SPEC_GET(BTN_NI, gpios); // configure the analog as a button
static struct gpio_callback btn_cb_data;

#define RED_NI DT_ALIAS(red)
const struct gpio_dt_spec red1 = GPIO_DT_SPEC_GET(RED_NI, gpios); // configure the analog as a button

#define GREEN_NI DT_ALIAS(green)
const struct gpio_dt_spec green2 = GPIO_DT_SPEC_GET(GREEN_NI, gpios); // configure the analog as a button

#define BLUE_NI DT_ALIAS(blue)
const struct gpio_dt_spec blue3 = GPIO_DT_SPEC_GET(BLUE_NI, gpios); // configure the analog as a button

#define INT_NI DT_ALIAS(turnon)
const struct gpio_dt_spec intRGB = GPIO_DT_SPEC_GET(INT_NI, gpios); // configure the analog as a button

#define BTN_EVENT BIT(0)      // event for single taps
#define RGB_EVENT BIT(1)      // evnent to signal any tap has happened
#define COLOR_EVENT BIT(2)    // event to signal mode change
#define DBLTAP_EVENT BIT(3)   // evetn for double tap
K_EVENT_DEFINE(eventManager); // define the event manager

volatile int16_t RGBComponents[3];

void buttonPressed(const struct device *dev, struct gpio_callback *cb, uint32_t pins); // when analog button is "pressed do interupt"
void brightColor();
K_THREAD_DEFINE(bright_id, 1024, brightColor, NULL, NULL, NULL, 2, 0, 0);
void talkRGB();
void rgbToggle();
K_THREAD_DEFINE(rgbToggle_id, 1024, rgbToggle, NULL, NULL, NULL, 2, 0, 0);
void talkRGB();
K_THREAD_DEFINE(talk_id, 1024, talkRGB, NULL, NULL, NULL, 2, 0, 0);

int main(void)
{
        gpio_pin_configure_dt(&btn, GPIO_INPUT);
        gpio_init_callback(&btn_cb_data, buttonPressed, BIT(btn.pin)); // set up interupt
        gpio_add_callback(btn.port, &btn_cb_data);
        gpio_pin_interrupt_configure_dt(&btn, GPIO_INT_EDGE_TO_ACTIVE);
        LOG_DBG("main");
        
}
void buttonPressed(const struct device *dev, struct gpio_callback *cb, uint32_t pins)
{
        k_event_post(&eventManager, BTN_EVENT);
}

void rgbToggle()
{
        gpio_pin_configure_dt(&intRGB, 0);
        while (true)
        {
                k_event_wait(&eventManager, BTN_EVENT, false, K_FOREVER);
                k_event_post(&eventManager, RGB_EVENT);
                gpio_pin_set_dt(&intRGB, 1);
                k_msleep(50);
                gpio_pin_set_dt(&intRGB, 0);
                LOG_DBG("BTN");
                k_event_clear(&eventManager, BTN_EVENT);
                k_msleep(1000);
        }
}

void talkRGB()
{
        {

                uint8_t buffer[6]; // set arrays

                int err;

                if (!i2c_is_ready_dt(&RGB))
                { // check if the sensor is ready
                        LOG_ERR("I2C bus not ready");
                        return -1;
                }

                buffer[0] = 0x01; // set to continouse conversion
                buffer[1] = 0x2D;
                err = i2c_write_dt(&RGB, buffer, 2);
                if (err)
                {
                        LOG_ERR("I2C write failed");
                        return -1;
                }
                k_msleep(SLEEP_TIME_MS);

                buffer[0] = 0x02; // set to continouse conversion
                buffer[1] = 0x02;
                err = i2c_write_dt(&RGB, buffer, 2);
                if (err)
                {
                        LOG_ERR("I2C write failed");
                        return -1;
                }
                k_msleep(SLEEP_TIME_MS);

                buffer[0] = 0x08; // set to continouse conversion
                buffer[1] = 0x00;
                err = i2c_write_dt(&RGB, buffer, 2);
                if (err)
                {
                        LOG_ERR("I2C write failed");
                }
                LOG_DBG("RGB READY");
                k_msleep(SLEEP_TIME_MS);

                while (true)
                {
                        k_event_wait(&eventManager, RGB_EVENT, false, K_FOREVER);
                        //LOG_DBG("RGB READ");

                        buffer[0] = 0x09;                                    // Data register for Green low byte
                        err = i2c_write_read_dt(&RGB, buffer, 1, buffer, 6); // read all data register in order green red blue
                        if (err)
                        {
                                LOG_ERR("I2C write-read failed");
                                // return -1;
                        }
                        RGBComponents[1] = (buffer[0] | buffer[1] << 8); // green
                        RGBComponents[0] = (buffer[2] | buffer[3] << 8); // red
                        RGBComponents[2] = (buffer[4] | buffer[5] << 8); // blue
                                                                         // Array is now in order RGB
                        LOG_INF("Red %x, Green %x, Blue %x", RGBComponents[0], RGBComponents[1], RGBComponents[2]);
                        k_msleep(10);
                        k_event_post(&eventManager, COLOR_EVENT);
                        k_event_clear(&eventManager, RGB_EVENT);
                }
        }
}

void brightColor()
{
        if (gpio_is_ready_dt(&red1) && gpio_is_ready_dt(&green2) && gpio_is_ready_dt(&blue3))
        {
                gpio_pin_configure_dt(&red1, GPIO_OUTPUT);
                gpio_pin_configure_dt(&green2, GPIO_OUTPUT);
                gpio_pin_configure_dt(&blue3, GPIO_OUTPUT);
        }else{
                LOG_ERR("NO CONFIG LED");
        }       
                while (true)
                {
                        k_event_wait(&eventManager, COLOR_EVENT, false, K_FOREVER);
                       // LOG_DBG("this one");
                        if (RGBComponents[0] > RGBComponents[1])
                        {

                                if ((RGBComponents[0]) > RGBComponents[2])
                                {
                                        gpio_pin_toggle_dt(&red1);
                                        LOG_INF("Red is the greatest color");
                                        k_msleep(1000);
                                        gpio_pin_toggle_dt(&red1);
                                }
                                else
                                {
                                        gpio_pin_set_dt(&blue3, 1);
                                        LOG_INF("Blue is the greatest color");
                                        k_msleep(1000);
                                        gpio_pin_set_dt(&blue3, 0);
                                }
                        }
                        else
                        {
                                if (RGBComponents[1] > RGBComponents[2])
                                {
                                        gpio_pin_set_dt(&green2, 1);
                                        LOG_INF("Green is the greatest color");
                                        k_msleep(1000);
                                        gpio_pin_set_dt(&green2, 0);
                                }
                        }
                        k_event_clear(&eventManager, COLOR_EVENT);
                }
        
}
