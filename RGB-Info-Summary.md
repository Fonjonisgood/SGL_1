# Summary of key information for the ISL29125

## Configuration 1
Address: **0x01**   
default: **0x00**
Current configuration **0x2D**
| 7 | 6 | 5 | 4 | 3 | 2 | 1 | 0 |
|---|---|---|---|---|---|---|---|
| 0 | 0 | 1 | 0 | 1 | 1 | 0 | 1 |

### RGB Operating Modes [2:0] 

| Power down | Green only | Red only | Blue only | Stand by | All colors | Green and Red | Green and Blue |
|---|---|---|---|---|---|---|---|
| 000 | 001 | 010 | 011 | 100 | 101 | 110 | 111 |

### RGB Data Sensing Range [3]
The range determines the ADC resolution (12 or 16 bit). Higher
range values offer better resolution and wider lux value.
| 375 lux | 10,000 lux|
|---|---|
| 0 | 1 |

### ADC Resolution [4]
This represent the number of samples taken per measurement 
| 16 bits | 12 bits|
|---|---|
| 0 | 1 |

### RGB Start Synced at INT Pin [5]
when set to 0 then the INT pin gets asserted whenever the sensor interrupts. When set to 1 the INT pin becomes input pin. witch actavates on the rising edge the INT pin, SYNC starts ADC conversion
| ADC start at I2C write 0x01 | ADC start at rising INT |
|---|---|
| 0 | 1 |

## Configuration-2 Register 
Address: **0x02**
default: **0x00**
Current configuration **0x02**

| 7 | 6 | 5 | 4 | 3 | 2 | 1 | 0 |
|---|---|---|---|---|---|---|---|
| 0 | 0 | 0 | 0 | 0 | 1 | 0 | 0 |
### IR compensation [0:5]
The device has an on chip passive optical filter designed to block (reject) most of the incident Infra Red.
The default settings have ample blocking for our uses so a configuartion of **0x02** is okay.

## Configuration-3 Register 
Address: **0x03**
default: **0x00**
Current configuration **0x00**
| 7 | 6 | 5 | 4 | 3 | 2 | 1 | 0 |
|---|---|---|---|---|---|---|---|
| 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 |

### INTERRUPT THRESHOLD ASSIGNMENT [1:0]
This lets us assign witch color will  e watched for an interupt.
The default is no interupt whitch we will be using.
|No int|Green int|Red int|Blue int|
|---|---|---|---|
| 00 | 01 | 10 | 11 |

### INTERRUPT PERSIST CONTROL [3:2]
This is used to minimize interupts due to noise or bright flashes, with this you can set a required number of interrupts that must happen
before the interrupt is triggered.

|1|2|4|8|
|---|---|---|---|
| 00 | 01 | 10 | 11 |

## Interrupt Threshold (Reg 0x4, Reg0x5, Reg0x6 and Reg0x7)
Address: **0x4, 0x5, 0x6, 0x7**
default: **0x00 for low, 0xFF for high**
This lets you set a specific value for the interupt for both bytes so you can get an exact point. For this program we are going to use the defualt.

## Status Flag Register
Address: **0x08**
default: **0x04**
Current configuration **0x00**
| 7 | 6 | 5 | 4 | 3 | 2 | 1 | 0 |
|---|---|---|---|---|---|---|---|
| 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 |

### RGBTHF [0]
This bit is the status of the interupt. When a interupt is triggered the logic is set to high. It gets reset to low when the interupt is cleared. For our purposes we will use the default of 0
| No interupt | interupt |
|---|---|
| 0 | 1 |

### Brown out flag
This is the status bit for a brownout condition (BOUT). The default value of this bit is HIGH(1) it needs to be set to low(0)
|  No Brownout | Power down or Brownout occurred |
|---|---|
| 0 | 1 |

## Data Registers
Address: **0x09-0x0E**
Each colors data comes in 2 sets of 8 bit reas outs starting with green the red then blue. With the least significant byte being the first of the pair.
