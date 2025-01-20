#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/i2c.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(Selfdirected learning, LOG_LEVEL_DBG);

#define RGB_NODE DT_NODELABEL(RGBsensor)
const struct i2c_dt_spec RGBSensor = I2C_DT_SPEC_GET(RGB_NODE);

#define SLEEP_TIME_MS 1000

#define COMP_NI DT_NODELABLE(pb)
const struct gpio_dt_spec COMPARE = GPIO_DT_SPEC_GET(COMP_NI, gpios);//configure the analog as a button

int main(void){
        uint8_t buffer[6];//set arrays    
        int16_t RGBComponents[3];
        int err;

        if (!i2c_is_ready_dt(&magSensor)) {//check if the sensor is ready
                LOG_ERR("I2C bus not ready");
                return -1;
        }

        buffer[0] = 0x01;//set to continouse conversion
        buffer[1] = 0x2D;
        err = i2c_write_dt(&magSensor, buffer, 3);
        if (err) {
                LOG_ERR("I2C write failed");
                return -1;
        }
        k_msleep(SLEEP_TIME_MS);
       
        buffer[0] = 0x02;//set to continouse conversion
        buffer[1] = 0x02;
        err = i2c_write_dt(&magSensor, buffer, 3);
        if (err) {
                LOG_ERR("I2C write failed");
                return -1;
        }
        k_msleep(SLEEP_TIME_MS);

        
        buffer[0] = 0x08;//set to continouse conversion
        buffer[1] = 0x00;
        err = i2c_write_dt(&magSensor, buffer, 3);
        if (err) {
                LOG_ERR("I2C write failed");
                return -1;
        }
        k_msleep(SLEEP_TIME_MS);



}
