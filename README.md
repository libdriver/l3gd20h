<div align=center>
<img src="/doc/image/logo.png"/>
</div>

## LibDriver L3GD20H

[English](/README.md) | [ 简体中文](/README_CN.md)

The L3GD20H is a low-power three-axis angular rate sensor. It includes a sensing element and an IC interface able to provide the measured angular rate to the external world through digital interface (I2C/SPI).The sensing element is manufactured using a dedicated micromachining process developed by ST to produce inertial sensors and actuators on silicon wafers.The IC interface is manufactured using a CMOS process that allows a high level of integration to design a dedicated circuit which is trimmed to better match the sensing element characteristics.The L3GD20H has a full scale of ±245/±500/±2000 dps and is capable of measuring rates with a user selectable bandwidth.It can be used in gaming and virtual reality input devices,motion control with MMI (man-machine interface),GPS navigation systems,GPS navigation systems and so on.

LibDriver L3GD20H is the full function driver of L3GD20H  launched by LibDriver.It provides angular velocity reading, angular velocity FIFO mode acquisition, threshold interrupt and other functions.

### Table of Contents

  - [Instruction](#Instruction)
  - [Install](#Install)
  - [Usage](#Usage)
    - [example basic](#example-basic)
    - [example fifo](#example-fifo)
    - [example interrupt](#example-interrupt)
  - [Document](#Document)
  - [Contributing](#Contributing)
  - [License](#License)
  - [Contact Us](#Contact-Us)

### Instruction

/src includes LibDriver L3GD20H source files.

/interface includes LibDriver L3GD20H IIC and SPI platform independent template。

/test includes LibDriver L3GD20H driver test code and this code can test the chip necessary function simply。

/example includes LibDriver L3GD20H sample code.

/doc includes LibDriver L3GD20H offline document.

/datasheet includes L3GD20H datasheet。

/project includes the common Linux and MCU development board sample code. All projects use the shell script to debug the driver and the detail instruction can be found in each project's README.md.

### Install

Reference /interface IIC and SPI platform independent template and finish your platform IIC and SPI driver.

Add /src, /interface and /example to your project.

### Usage

#### example basic

```C
volatile uint8_t res;
volatile float dps[3];

res = l3gd20h_basic_init(L3GD20H_INTERFACE_IIC, L3GD20H_ADDRESS_SDO_0);
if (res)
{
    return 1;
}

...

for (i = 0; i < 3; i++)
{
    res = l3gd20h_basic_read((float *)dps);
    if (res)
    {
        l3gd20h_basic_deinit();

        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: x %0.2f dps.\n", dps[0]);
    l3gd20h_interface_debug_print("l3gd20h: y %0.2f dps.\n", dps[1]);
    l3gd20h_interface_debug_print("l3gd20h: z %0.2f dps.\n", dps[2]);
    l3gd20h_interface_delay_ms(1000);
    
    ...
    
}

...

l3gd20h_basic_deinit();

return 0;
```

#### example fifo

```C
volatile uint8_t res;

static uint8_t _l3gd20h_fifo_receive_callback(float (*dps)[3], uint16_t len)
{
    ...
        
    return 0;
}

res = gpio_interrupt_init();
if (res)
{
    return 1;
}

res = l3gd20h_fifo_init(L3GD20H_INTERFACE_IIC, L3GD20H_ADDRESS_SDO_0, _l3gd20h_fifo_receive_callback);
if (res)
{
    l3gd20h_fifo_deinit();
    gpio_interrupt_deinit();

    return 1;
}

...

while (times)
{

...
    
}

...

l3gd20h_fifo_deinit();
gpio_interrupt_deinit();

return 0;
```

#### example interrupt

```C
volatile uint8_t res;

static uint8_t _l3gd20h_interrupt_receive_callback(uint8_t type)
{
    volatile uint8_t res;

    switch (type)
    {
        case L3GD20H_INTERRUPT1_Z_HIGH :
        {
            l3gd20h_interface_debug_print("l3gd20h: irq z high threshold.\n");
            
            break;
        }
        case L3GD20H_INTERRUPT1_Y_HIGH :
        {
            l3gd20h_interface_debug_print("l3gd20h: irq y high threshold.\n");
            
            break;
        }
        case L3GD20H_INTERRUPT1_X_HIGH :
        {
            l3gd20h_interface_debug_print("l3gd20h: irq x high threshold.\n");
            
            break;
        }
        default :
        {
            break;
        }
    }
    
    return 0;
}

res = gpio_interrupt_init();
if (res)
{
    return 1;
}

res = l3gd20h_interrupt_init(L3GD20H_INTERFACE_IIC, L3GD20H_ADDRESS_SDO_0, 20.f, _l3gd20h_interrupt_receive_callback);
if (res)
{
    l3gd20h_interrupt_deinit();
    gpio_interrupt_deinit();
    
    return 1;
}

...

while (1)
{
    
...
    
}

...

l3gd20h_interrupt_deinit();
gpio_interrupt_deinit();

return 0;
```

### Document

Online documents: https://www.libdriver.com/docs/l3gd20h/index.html

Offline documents: /doc/html/index.html

### Contributing

Please sent an e-mail to lishifenging@outlook.com

### License

Copyright (C) LibDriver 2015-2021 All rights reserved 



The MIT License (MIT) 



Permission is hereby granted, free of charge, to any person obtaining a copy

of this software and associated documentation files (the "Software"), to deal

in the Software without restriction, including without limitation the rights

to use, copy, modify, merge, publish, distribute, sublicense, and/or sell

copies of the Software, and to permit persons to whom the Software is

furnished to do so, subject to the following conditions: 



The above copyright notice and this permission notice shall be included in all

copies or substantial portions of the Software. 



THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR

IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,

FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE

AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER

LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,

OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE

SOFTWARE. 

### Contact Us

Please sent an e-mail to lishifenging@outlook.com