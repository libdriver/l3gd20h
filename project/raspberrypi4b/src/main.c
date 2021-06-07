/**
 * Copyright (C) LibDriver 2015-2021 All rights reserved
 * 
 * The MIT License (MIT)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE. 
 *
 * @file      main.c
 * @brief     main source file
 * @version   1.0.0
 * @author    Shifeng Li
 * @date      2021-05-29
 *
 * <h3>history</h3>
 * <table>
 * <tr><th>Date        <th>Version  <th>Author      <th>Description
 * <tr><td>2021/05/29  <td>1.0      <td>Shifeng Li  <td>first upload
 * </table>
 */

#include "driver_l3gd20h_interrupt.h"
#include "driver_l3gd20h_fifo.h"
#include "driver_l3gd20h_basic.h"
#include "driver_l3gd20h_fifo_test.h"
#include "driver_l3gd20h_interrupt_test.h"
#include "driver_l3gd20h_read_test.h"
#include "driver_l3gd20h_register_test.h"
#include "gpio.h"
#include <stdlib.h>

uint8_t g_flag;                            /**< interrupt flag */
uint8_t (*g_gpio_irq)(void) = NULL;        /**< gpio irq function address */


/**
 * @brief     interface fifo receive callback
 * @param[in] type is the irq type
 * @return    status code
 *            - 0 success
 *            - 1 run failed
 * @note      none
 */
static uint8_t _l3gd20h_fifo_receive_callback(float (*dps)[3], uint16_t len)
{
    l3gd20h_interface_debug_print("l3gd20h: fifo irq with %d.\n", len);
    g_flag = 1;
    
    return 0;
}

/**
 * @brief     interface interrupt receive callback
 * @param[in] type is the irq type
 * @return    status code
 *            - 0 success
 *            - 1 run failed
 * @note      none
 */
static uint8_t _l3gd20h_interrupt_receive_callback(uint8_t type)
{
    volatile uint8_t res;

    switch (type)
    {
        case L3GD20H_INTERRUPT1_Z_HIGH :
        {
            g_flag = 1;
            l3gd20h_interface_debug_print("l3gd20h: irq z high threshold.\n");
            
            break;
        }
        case L3GD20H_INTERRUPT1_Y_HIGH :
        {
            g_flag = 1;
            l3gd20h_interface_debug_print("l3gd20h: irq y high threshold.\n");
            
            break;
        }
        case L3GD20H_INTERRUPT1_X_HIGH :
        {
            g_flag = 1;
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

/**
 * @brief     l3gd20h full function
 * @param[in] argc is arg numbers
 * @param[in] **argv is the arg address
 * @return    status code
 *             - 0 success
 *             - 1 run failed
 *             - 5 param is invalid
 * @note      none
 */
uint8_t l3gd20h(uint8_t argc, char **argv)
{
    if (argc == 1)
    {
        goto help;
    }
    else if (argc == 2)
    {
        if (strcmp("-i", argv[1]) == 0)
        {
            l3gd20h_info_t info;
            
            /* print l3gd20h info */
            
            l3gd20h_info(&info);
            l3gd20h_interface_debug_print("l3gd20h: chip is %s.\n", info.chip_name);
            l3gd20h_interface_debug_print("l3gd20h: manufacturer is %s.\n", info.manufacturer_name);
            l3gd20h_interface_debug_print("l3gd20h: interface is %s.\n", info.interface);
            l3gd20h_interface_debug_print("l3gd20h: driver version is %d.%d.\n", info.driver_version/1000, (info.driver_version%1000)/100);
            l3gd20h_interface_debug_print("l3gd20h: min supply voltage is %0.1fV.\n", info.supply_voltage_min_v);
            l3gd20h_interface_debug_print("l3gd20h: max supply voltage is %0.1fV.\n", info.supply_voltage_max_v);
            l3gd20h_interface_debug_print("l3gd20h: max current is %0.2fmA.\n", info.max_current_ma);
            l3gd20h_interface_debug_print("l3gd20h: max temperature is %0.1fC.\n", info.temperature_max);
            l3gd20h_interface_debug_print("l3gd20h: min temperature is %0.1fC.\n", info.temperature_min);
            
            return 0;
        }
        else if (strcmp("-p", argv[1]) == 0)
        {
            /* print pin connection */
            
            l3gd20h_interface_debug_print("l3gd20h: SPI interface SCK connected to GPIO11(BCM).\n");
            l3gd20h_interface_debug_print("l3gd20h: SPI interface MISO connected to GPIO9(BCM).\n");
            l3gd20h_interface_debug_print("l3gd20h: SPI interface MOSI connected to GPIO10(BCM).\n");
            l3gd20h_interface_debug_print("l3gd20h: SPI interface CS connected to GPIO8(BCM).\n");
            l3gd20h_interface_debug_print("l3gd20h: IIC interface SCL connected to GPIO3(BCM).\n");
            l3gd20h_interface_debug_print("l3gd20h: IIC interface SDA connected to GPIO2(BCM).\n");
            l3gd20h_interface_debug_print("l3gd20h: INT connected to GPIOB17(BCM).\n");
            
            return 0;
        }
        else if (strcmp("-h", argv[1]) == 0)
        {
            /* show l3gd20h help */
            
            help:
            
            l3gd20h_interface_debug_print("l3gd20h -i\n\tshow l3gd20h chip and driver information.\n");
            l3gd20h_interface_debug_print("l3gd20h -h\n\tshow l3gd20h help.\n");
            l3gd20h_interface_debug_print("l3gd20h -p\n\tshow l3gd20h pin connections of the current board.\n");
            l3gd20h_interface_debug_print("l3gd20h -t reg (-iic (0 | 1) | -spi)\n\trun l3gd20h register test.\n");
            l3gd20h_interface_debug_print("l3gd20h -t read <times> (-iic (0 | 1) | -spi)\n\trun l3gd20h read test.times means the test times.\n");
            l3gd20h_interface_debug_print("l3gd20h -t fifo (-iic (0 | 1) | -spi)\n\trun l3gd20h fifo test.\n");
            l3gd20h_interface_debug_print("l3gd20h -t int (-iic (0 | 1) | -spi)\n\trun l3gd20h interrupt test.\n");
            l3gd20h_interface_debug_print("l3gd20h -c basic <times> (-iic (0 | 1) | -spi)\n\trun l3gd20h basic function.times is the read times.\n");
            l3gd20h_interface_debug_print("l3gd20h -c fifo <times> (-iic (0 | 1) | -spi)\n\trun l3gd20h fifo function.times is the read times.\n");
            l3gd20h_interface_debug_print("l3gd20h -c int (-iic (0 | 1) | -spi) <threshold>\n\trun l3gd20h interrupt function.threshold is the interrupt threshold.\n");
            
            return 0;
        }
        else
        {
            return 5;
        }
    }
    else if (argc == 4)
    {
        if (strcmp("-t", argv[1]) == 0)
        {
            if (strcmp("reg", argv[2]) == 0)
            {
                if (strcmp("-spi", argv[3]) == 0)
                {
                    volatile uint8_t res;
                    
                    /* run register test */
                    res = l3gd20h_register_test(L3GD20H_INTERFACE_SPI, L3GD20H_ADDRESS_SDO_0);
                    if (res)
                    {
                        return 1;
                    }
                    
                    return 0;
                }
                else
                {
                    return 5;
                }
            }
            else if (strcmp("fifo", argv[2]) == 0)
            {
                if (strcmp("-spi", argv[3]) == 0)
                {
                    volatile uint8_t res;
                    
                    res = gpio_interrupt_init();
                    if (res)
                    {
                        return 1;
                    }
                    g_gpio_irq = l3gd20h_fifo_test_irq_handler;
                    
                    /* run fifo test */
                    res = l3gd20h_fifo_test(L3GD20H_INTERFACE_SPI, L3GD20H_ADDRESS_SDO_0);
                    if (res)
                    {
                        gpio_interrupt_deinit();
                        g_gpio_irq = NULL;
                        
                        return 1;
                    }
                    gpio_interrupt_deinit();
                    g_gpio_irq = NULL;
                    
                    return 0;
                }
                else
                {
                    return 5;
                }
            }
            else if (strcmp("int", argv[2]) == 0)
            {
                if (strcmp("-spi", argv[3]) == 0)
                {
                    volatile uint8_t res;
                    
                    res = gpio_interrupt_init();
                    if (res)
                    {
                        return 1;
                    }
                    g_gpio_irq = l3gd20h_interrupt_test_irq_handler;
                    
                    /* run interrupt test */
                    res = l3gd20h_interrupt_test(L3GD20H_INTERFACE_SPI, L3GD20H_ADDRESS_SDO_0, 50.f, 100);
                    if (res)
                    {
                        gpio_interrupt_deinit();
                        g_gpio_irq = NULL;
                        
                        return 1;
                    }
                    gpio_interrupt_deinit();
                    g_gpio_irq = NULL;
                    
                    return 0;
                }
                else
                {
                    return 5;
                }
            }
            else
            {
                return 5;
            }
        }
        else
        {
            return 5;
        }
    }
    else if (argc == 5)
    {
        if (strcmp("-t", argv[1]) == 0)
        {
            if (strcmp("reg", argv[2]) == 0)
            {
                if (strcmp("-iic", argv[3]) == 0)
                {
                    volatile uint8_t res;
                    l3gd20h_address_t addr;
                    
                    if (strcmp("0", argv[4]) == 0)
                    {
                        addr = L3GD20H_ADDRESS_SDO_0;
                    }
                    else if (strcmp("1", argv[4]) == 0)
                    {
                        addr = L3GD20H_ADDRESS_SDO_1;
                    }
                    else
                    {
                        return 5;
                    }
                    
                    /* run register test */
                    res = l3gd20h_register_test(L3GD20H_INTERFACE_IIC, addr);
                    if (res)
                    {
                        return 1;
                    }
                    
                    return 0;
                }
                else
                {
                    return 5;
                }
            }
            else if (strcmp("read", argv[2]) == 0)
            {
                if (strcmp("-spi", argv[4]) == 0)
                {
                    volatile uint8_t res;
                    
                    /* run register test */
                    res = l3gd20h_read_test(L3GD20H_INTERFACE_SPI, L3GD20H_ADDRESS_SDO_0, atoi(argv[3]));
                    if (res)
                    {
                        return 1;
                    }
                    
                    return 0;
                }
                else
                {
                    return 5;
                }
            }
            else if (strcmp("fifo", argv[2]) == 0)
            {
                if (strcmp("-iic", argv[3]) == 0)
                {
                    volatile uint8_t res;
                    l3gd20h_address_t addr;
                    
                    if (strcmp("0", argv[4]) == 0)
                    {
                        addr = L3GD20H_ADDRESS_SDO_0;
                    }
                    else if (strcmp("1", argv[4]) == 0)
                    {
                        addr = L3GD20H_ADDRESS_SDO_1;
                    }
                    else
                    {
                        return 5;
                    }

                    res = gpio_interrupt_init();
                    if (res)
                    {
                        return 1;
                    }
                    g_gpio_irq = l3gd20h_fifo_test_irq_handler;
                    
                    /* run fifo test */
                    res = l3gd20h_fifo_test(L3GD20H_INTERFACE_IIC, addr);
                    if (res)
                    {
                        gpio_interrupt_deinit();
                        g_gpio_irq = NULL;
                        
                        return 1;
                    }
                    gpio_interrupt_deinit();
                    g_gpio_irq = NULL;
                    
                    return 0;
                }
                else
                {
                    return 5;
                }
            }
            else if (strcmp("int", argv[2]) == 0)
            {
                if (strcmp("-iic", argv[3]) == 0)
                {
                    volatile uint8_t res;
                    l3gd20h_address_t addr;
                    
                    if (strcmp("0", argv[4]) == 0)
                    {
                        addr = L3GD20H_ADDRESS_SDO_0;
                    }
                    else if (strcmp("1", argv[4]) == 0)
                    {
                        addr = L3GD20H_ADDRESS_SDO_1;
                    }
                    else
                    {
                        return 5;
                    }

                    res = gpio_interrupt_init();
                    if (res)
                    {
                        return 1;
                    }
                    g_gpio_irq = l3gd20h_interrupt_test_irq_handler;
                    
                    /* run interrupt test */
                    res = l3gd20h_interrupt_test(L3GD20H_INTERFACE_IIC, addr, 50.0f, 100);
                    if (res)
                    {
                        gpio_interrupt_deinit();
                        g_gpio_irq = NULL;
                        
                        return 1;
                    }
                    gpio_interrupt_deinit();
                    g_gpio_irq = NULL;
                    
                    return 0;
                }
                else
                {
                    return 5;
                }
            }
            else
            {
                return 5;
            }
        }
        else if (strcmp("-c", argv[1]) == 0)
        {
            if (strcmp("basic", argv[2]) == 0)
            {
                volatile uint8_t res;
                volatile uint32_t i, times;
                volatile float dps[3];
                
                times = atoi(argv[3]);
                if (strcmp("-spi", argv[4]) == 0)
                {
                    res = l3gd20h_basic_init(L3GD20H_INTERFACE_SPI, L3GD20H_ADDRESS_SDO_0);
                    if (res)
                    {
                        return 1;
                    }
                    for (i = 0; i < times; i++)
                    {
                        res = l3gd20h_basic_read((float *)dps);
                        if (res)
                        {
                            l3gd20h_basic_deinit();
                            
                            return 1;
                        }
                        l3gd20h_interface_debug_print("l3gd20h: %d/%d.\n", i + 1, times);
                        l3gd20h_interface_debug_print("l3gd20h: x %0.2f dps.\n", dps[0]);
                        l3gd20h_interface_debug_print("l3gd20h: y %0.2f dps.\n", dps[1]);
                        l3gd20h_interface_debug_print("l3gd20h: z %0.2f dps.\n", dps[2]);
                        l3gd20h_interface_delay_ms(1000);
                    }
                    l3gd20h_basic_deinit();
                    
                    return 0;
                }
                else
                {
                    return 5;
                }
            }
            else if (strcmp("fifo", argv[2]) == 0)
            {
                volatile uint8_t res;
                volatile uint32_t times, timeout;
                
                times = atoi(argv[3]);
                if (strcmp("-spi", argv[4]) == 0)
                {
                    res = gpio_interrupt_init();
                    if (res)
                    {
                        return 1;
                    }
                    g_gpio_irq = l3gd20h_fifo_irq_handler;
                    
                    res = l3gd20h_fifo_init(L3GD20H_INTERFACE_SPI, L3GD20H_ADDRESS_SDO_0, _l3gd20h_fifo_receive_callback);
                    if (res)
                    {
                        l3gd20h_fifo_deinit();
                        gpio_interrupt_deinit();
                        g_gpio_irq = NULL;
                        
                        return 1;
                    }
                    while (times)
                    {
                        timeout = 5000;
                        g_flag = 0;
                        while (timeout)
                        {
                            timeout--;
                            if (g_flag)
                            {
                                break;
                            }
                            l3gd20h_interface_delay_ms(1);
                        }
                        if (timeout == 0)
                        {
                            l3gd20h_interface_debug_print("l3gd20h: fifo timeout.\n");
                        }
                        times--;
                    }
                    
                    l3gd20h_fifo_deinit();
                    gpio_interrupt_deinit();
                    g_gpio_irq = NULL;
                    
                    return 0;
                }
                else
                {
                    return 5;
                }
            }
            else if (strcmp("int", argv[2]) == 0)
            {
                volatile uint8_t res;
                volatile uint32_t timeout;
                volatile float threshold;
                
                threshold = atof(argv[4]);
                if (strcmp("-spi", argv[3]) == 0)
                {
                    res = gpio_interrupt_init();
                    if (res)
                    {
                        return 1;
                    }
                    g_gpio_irq = l3gd20h_interrupt_irq_handler;
                    
                    res = l3gd20h_interrupt_init(L3GD20H_INTERFACE_SPI, L3GD20H_ADDRESS_SDO_0, threshold, _l3gd20h_interrupt_receive_callback);
                    if (res)
                    {
                        l3gd20h_interrupt_deinit();
                        gpio_interrupt_deinit();
                        g_gpio_irq = NULL;
                        
                        return 1;
                    }
                    l3gd20h_interface_debug_print("l3gd20h: set threshold %0.2f.\n", threshold);
                    timeout = 5000;
                    g_flag = 0;
                    while (timeout)
                    {
                        timeout--;
                        if (g_flag)
                        {
                            break;
                        }
                        l3gd20h_interface_delay_ms(1);
                    }
                    if (timeout == 0)
                    {
                        l3gd20h_interface_debug_print("l3gd20h: interrupt timeout.\n");
                    }
                    else
                    {
                        l3gd20h_interface_debug_print("l3gd20h: find interrupt.\n");
                    }
                    
                    l3gd20h_interrupt_deinit();
                    gpio_interrupt_deinit();
                    g_gpio_irq = NULL;
                    
                    return 0;
                }
                else
                {
                    return 5;
                }
            }
            else
            {
                return 5;
            }
        }
        else
        {
            return 5;
        }
    }
    else if (argc == 6)
    {
        if (strcmp("-t", argv[1]) == 0)
        {
            if (strcmp("read", argv[2]) == 0)
            {
                if (strcmp("-iic", argv[4]) == 0)
                {
                    volatile uint8_t res;
                    l3gd20h_address_t addr;
                    
                    if (strcmp("0", argv[5]) == 0)
                    {
                        addr = L3GD20H_ADDRESS_SDO_0;
                    }
                    else if (strcmp("1", argv[5]) == 0)
                    {
                        addr = L3GD20H_ADDRESS_SDO_1;
                    }
                    else
                    {
                        return 5;
                    }
                    
                    /* run register test */
                    res = l3gd20h_read_test(L3GD20H_INTERFACE_IIC, addr, atoi(argv[3]));
                    if (res)
                    {
                        return 1;
                    }
                    
                    return 0;
                }
                else
                {
                    return 5;
                }
            }
            else
            {
                return 5;
            }
        }
        else if (strcmp("-c", argv[1]) == 0)
        {
            if (strcmp("basic", argv[2]) == 0)
            {
                volatile uint8_t res;
                volatile uint32_t i, times;
                volatile float dps[3];
                l3gd20h_address_t addr;
                
                times = atoi(argv[3]);
                if (strcmp("0", argv[5]) == 0)
                {
                    addr = L3GD20H_ADDRESS_SDO_0;
                }
                else if (strcmp("1", argv[5]) == 0)
                {
                    addr = L3GD20H_ADDRESS_SDO_1;
                }
                else
                {
                    return 5;
                }
                if (strcmp("-iic", argv[4]) == 0)
                {
                    res = l3gd20h_basic_init(L3GD20H_INTERFACE_IIC, addr);
                    if (res)
                    {
                        return 1;
                    }
                    for (i = 0; i < times; i++)
                    {
                        res = l3gd20h_basic_read((float *)dps);
                        if (res)
                        {
                            l3gd20h_basic_deinit();
                            
                            return 1;
                        }
                        l3gd20h_interface_debug_print("l3gd20h: %d/%d.\n", i + 1, times);
                        l3gd20h_interface_debug_print("l3gd20h: x %0.2f dps.\n", dps[0]);
                        l3gd20h_interface_debug_print("l3gd20h: y %0.2f dps.\n", dps[1]);
                        l3gd20h_interface_debug_print("l3gd20h: z %0.2f dps.\n", dps[2]);
                        l3gd20h_interface_delay_ms(1000);
                    }
                    l3gd20h_basic_deinit();
                    
                    return 0;
                }
                else
                {
                    return 5;
                }
            }
            else if (strcmp("fifo", argv[2]) == 0)
            {
                volatile uint8_t res;
                volatile uint32_t times, timeout;
                l3gd20h_address_t addr;
                
                times = atoi(argv[3]);
                if (strcmp("0", argv[5]) == 0)
                {
                    addr = L3GD20H_ADDRESS_SDO_0;
                }
                else if (strcmp("1", argv[5]) == 0)
                {
                    addr = L3GD20H_ADDRESS_SDO_1;
                }
                else
                {
                    return 5;
                }
                if (strcmp("-iic", argv[4]) == 0)
                {
                    res = gpio_interrupt_init();
                    if (res)
                    {
                        return 1;
                    }
                    g_gpio_irq = l3gd20h_fifo_irq_handler;
                    
                    res = l3gd20h_fifo_init(L3GD20H_INTERFACE_IIC, addr, _l3gd20h_fifo_receive_callback);
                    if (res)
                    {
                        l3gd20h_fifo_deinit();
                        gpio_interrupt_deinit();
                        g_gpio_irq = NULL;
                        
                        return 1;
                    }
                    while (times)
                    {
                        timeout = 5000;
                        g_flag = 0;
                        while (timeout)
                        {
                            timeout--;
                            if (g_flag)
                            {
                                break;
                            }
                            l3gd20h_interface_delay_ms(1);
                        }
                        if (timeout == 0)
                        {
                            l3gd20h_interface_debug_print("l3gd20h: fifo timeout.\n");
                        }
                        times--;
                    }
                    
                    l3gd20h_fifo_deinit();
                    gpio_interrupt_deinit();
                    g_gpio_irq = NULL;
                    
                    return 0;
                }
                else
                {
                    return 5;
                }
            }
            else if (strcmp("int", argv[2]) == 0)
            {
                volatile uint8_t res;
                volatile uint32_t timeout;
                volatile float threshold;
                l3gd20h_address_t addr;
                
                threshold = atof(argv[5]);
                if (strcmp("0", argv[4]) == 0)
                {
                    addr = L3GD20H_ADDRESS_SDO_0;
                }
                else if (strcmp("1", argv[4]) == 0)
                {
                    addr = L3GD20H_ADDRESS_SDO_1;
                }
                else
                {
                    return 5;
                }
                if (strcmp("-iic", argv[3]) == 0)
                {
                    res = gpio_interrupt_init();
                    if (res)
                    {
                        return 1;
                    }
                    g_gpio_irq = l3gd20h_interrupt_irq_handler;
                    
                    res = l3gd20h_interrupt_init(L3GD20H_INTERFACE_IIC, addr, threshold, _l3gd20h_interrupt_receive_callback);
                    if (res)
                    {
                        l3gd20h_interrupt_deinit();
                        gpio_interrupt_deinit();
                        g_gpio_irq = NULL;
                        
                        return 1;
                    }
                    l3gd20h_interface_debug_print("l3gd20h: set threshold %0.2f.\n", threshold);
                    timeout = 5000;
                    g_flag = 0;
                    while (timeout)
                    {
                        timeout--;
                        if (g_flag)
                        {
                            break;
                        }
                        l3gd20h_interface_delay_ms(1);
                    }
                    if (timeout == 0)
                    {
                        l3gd20h_interface_debug_print("l3gd20h: interrupt timeout.\n");
                    }
                    else
                    {
                        l3gd20h_interface_debug_print("l3gd20h: find interrupt.\n");
                    }
                    
                    l3gd20h_interrupt_deinit();
                    gpio_interrupt_deinit();
                    g_gpio_irq = NULL;
                    
                    return 0;
                }
                else
                {
                    return 5;
                }
            }
            else
            {
                return 5;
            }
        }
        else
        {
            return 5;
        }
    }
    
    /* param is invalid */
    else
    {
        return 5;
    }
}

/**
 * @brief     main function
 * @param[in] argc is arg numbers
 * @param[in] **argv is the arg address
 * @return    status code
 *             - 0 success
 * @note      none
 */
int main(uint8_t argc, char **argv)
{
    uint8_t res;

    res = l3gd20h(argc, argv);
    if (res == 0)
    {
        /* run success */
    }
    else if (res == 1)
    {
        l3gd20h_interface_debug_print("l3gd20h: run failed.\n");
    }
    else if (res == 5)
    {
        l3gd20h_interface_debug_print("l3gd20h: param is invalid.\n");
    }
    else
    {
        l3gd20h_interface_debug_print("l3gd20h: unknow status code.\n");
    }

    return 0;
}
