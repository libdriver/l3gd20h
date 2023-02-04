/**
 * Copyright (c) 2015 - present LibDriver All rights reserved
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
#include "shell.h"
#include "clock.h"
#include "delay.h"
#include "gpio.h"
#include "uart.h"
#include "getopt.h"
#include <stdlib.h>

/**
 * @brief global var definition
 */
uint8_t g_buf[256];                        /**< uart buffer */
volatile uint16_t g_len;                   /**< uart buffer length */
uint8_t (*g_gpio_irq)(void) = NULL;        /**< gpio irq */
volatile uint8_t g_flag;                   /**< global flag */

/**
 * @brief exti 0 irq
 * @note  none
 */
void EXTI0_IRQHandler(void)
{
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_0);
}

/**
 * @brief     gpio exti callback
 * @param[in] pin is the gpio pin
 * @note      none
 */
void HAL_GPIO_EXTI_Callback(uint16_t pin)
{
    if (pin == GPIO_PIN_0)
    {
        if (g_gpio_irq != NULL)
        {
            g_gpio_irq();
        }
    }
}

/**
 * @brief     interface fifo receive callback
 * @param[in] type is the irq type
 * @note      none
 */
static void a_l3gd20h_fifo_receive_callback(float (*dps)[3], uint16_t len)
{
    l3gd20h_interface_debug_print("l3gd20h: fifo irq with %d.\n", len);
    g_flag = 1;
}

/**
 * @brief     interface interrupt receive callback
 * @param[in] type is the irq type
 * @note      none
 */
static void a_l3gd20h_interrupt_receive_callback(uint8_t type)
{
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
}

/**
 * @brief     l3gd20h full function
 * @param[in] argc is arg numbers
 * @param[in] **argv is the arg address
 * @return    status code
 *            - 0 success
 *            - 1 run failed
 *            - 5 param is invalid
 * @note      none
 */
uint8_t l3gd20h(uint8_t argc, char **argv)
{
    int c;
    int longindex = 0;
    const char short_options[] = "hipe:t:";
    const struct option long_options[] =
    {
        {"help", no_argument, NULL, 'h'},
        {"information", no_argument, NULL, 'i'},
        {"port", no_argument, NULL, 'p'},
        {"example", required_argument, NULL, 'e'},
        {"test", required_argument, NULL, 't'},
        {"addr", required_argument, NULL, 1},
        {"interface", required_argument, NULL, 2},
        {"threshold", required_argument, NULL, 3},
        {"times", required_argument, NULL, 4},
        {"timeout", required_argument, NULL, 5},
        {NULL, 0, NULL, 0},
    };
    char type[33] = "unknown";
    uint32_t times = 3;
    uint32_t timeout = 5000;
    l3gd20h_address_t addr = L3GD20H_ADDRESS_SDO_0;
    l3gd20h_interface_t interface = L3GD20H_INTERFACE_IIC;
    float threshold = 50.0f;
    
    /* if no params */
    if (argc == 1)
    {
        /* goto the help */
        goto help;
    }
    
    /* init 0 */
    optind = 0;
    
    /* parse */
    do
    {
        /* parse the args */
        c = getopt_long(argc, argv, short_options, long_options, &longindex);
        
        /* judge the result */
        switch (c)
        {
            /* help */
            case 'h' :
            {
                /* set the type */
                memset(type, 0, sizeof(char) * 33);
                snprintf(type, 32, "h");
                
                break;
            }
            
            /* information */
            case 'i' :
            {
                /* set the type */
                memset(type, 0, sizeof(char) * 33);
                snprintf(type, 32, "i");
                
                break;
            }
            
            /* port */
            case 'p' :
            {
                /* set the type */
                memset(type, 0, sizeof(char) * 33);
                snprintf(type, 32, "p");
                
                break;
            }
            
            /* example */
            case 'e' :
            {
                /* set the type */
                memset(type, 0, sizeof(char) * 33);
                snprintf(type, 32, "e_%s", optarg);
                
                break;
            }
            
            /* test */
            case 't' :
            {
                /* set the type */
                memset(type, 0, sizeof(char) * 33);
                snprintf(type, 32, "t_%s", optarg);
                
                break;
            }
            
            /* addr */
            case 1 :
            {
                /* set the addr pin */
                if (strcmp("0", optarg) == 0)
                {
                    addr = L3GD20H_ADDRESS_SDO_0;
                }
                else if (strcmp("1", optarg) == 0)
                {
                    addr = L3GD20H_ADDRESS_SDO_1;
                }
                else
                {
                    return 5;
                }
                
                break;
            }
            
            /* interface */
            case 2 :
            {
                /* set the interface */
                if (strcmp("iic", optarg) == 0)
                {
                    interface = L3GD20H_INTERFACE_IIC;
                }
                else if (strcmp("spi", optarg) == 0)
                {
                    interface = L3GD20H_INTERFACE_SPI;
                }
                else
                {
                    return 5;
                }
                
                break;
            }
            
            /* threshold */
            case 3 :
            {
                threshold = atof(optarg);
                
                break;
            }
            
            /* running times */
            case 4 :
            {
                /* set the times */
                times = atol(optarg);
                
                break;
            } 
            
            /* timeout */
            case 5 :
            {
                /* set the timeout */
                timeout = atol(optarg);
                
                break;
            } 

            /* the end */
            case -1 :
            {
                break;
            }
            
            /* others */
            default :
            {
                return 5;
            }
        }
    } while (c != -1);
    
    /* run the function */
    if (strcmp("t_reg", type) == 0)
    {
        uint8_t res;
        
        /* run reg test */
        res = l3gd20h_register_test(interface, addr);
        if (res != 0)
        {
            return 1;
        }
        
        return 0;
    }
    else if (strcmp("t_read", type) == 0)
    {
        uint8_t res;
        
        /* run read test */
        res = l3gd20h_read_test(interface, addr, times);
        if (res != 0)
        {
            return 1;
        }
        
        return 0;
    }
    else if (strcmp("t_fifo", type) == 0)
    {
        uint8_t res;
        
        /* gpio init */
        res = gpio_interrupt_init();
        if (res != 0)
        {
            return 1;
        }
        
        /* set gpio irq */
        g_gpio_irq = l3gd20h_fifo_test_irq_handler;
        
        /* run fifo test */
        res = l3gd20h_fifo_test(interface, addr);
        if (res != 0)
        {
            (void)gpio_interrupt_deinit();
            g_gpio_irq = NULL;
            
            return 1;
        }
        
        /* gpio deinit */
        (void)gpio_interrupt_deinit();
        g_gpio_irq = NULL;
        
        return 0;
    }
    else if (strcmp("t_int", type) == 0)
    {
        uint8_t res;
        
        /* gpio init */
        res = gpio_interrupt_init();
        if (res != 0)
        {
            return 1;
        }
        
        /* set gpio irq */
        g_gpio_irq = l3gd20h_interrupt_test_irq_handler;
        
        /* run interrupt test */
        res = l3gd20h_interrupt_test(interface, addr, 50.f, 100);
        if (res != 0)
        {
            (void)gpio_interrupt_deinit();
            g_gpio_irq = NULL;
            
            return 1;
        }
        
        /* gpio deinit */
        (void)gpio_interrupt_deinit();
        g_gpio_irq = NULL;
        
        return 0;
    }
    else if (strcmp("e_read", type) == 0)
    {
        uint8_t res;
        uint32_t i;
        float dps[3];
        
        /* basic init */
        res = l3gd20h_basic_init(interface, addr);
        if (res != 0)
        {
            return 1;
        }
        
        /* loop */
        for (i = 0; i < times; i++)
        {
            /* read data */
            res = l3gd20h_basic_read((float *)dps);
            if (res != 0)
            {
                (void)l3gd20h_basic_deinit();
                
                return 1;
            }
            
            /* output */
            l3gd20h_interface_debug_print("l3gd20h: %d/%d.\n", i + 1, times);
            l3gd20h_interface_debug_print("l3gd20h: x %0.2f dps.\n", dps[0]);
            l3gd20h_interface_debug_print("l3gd20h: y %0.2f dps.\n", dps[1]);
            l3gd20h_interface_debug_print("l3gd20h: z %0.2f dps.\n", dps[2]);
            l3gd20h_interface_delay_ms(1000);
        }
        
        /* basic deinit */
        (void)l3gd20h_basic_deinit();
        
        return 0;
    }
    else if (strcmp("e_fifo", type) == 0)
    {
        uint8_t res;
        
        /* gpio init */
        res = gpio_interrupt_init();
        if (res != 0)
        {
            return 1;
        }
        
        /* set gpio irq */
        g_gpio_irq = l3gd20h_fifo_irq_handler;
        
        /* fifo init */
        res = l3gd20h_fifo_init(interface, addr, a_l3gd20h_fifo_receive_callback);
        if (res != 0)
        {
            (void)l3gd20h_fifo_deinit();
            (void)gpio_interrupt_deinit();
            g_gpio_irq = NULL;
            
            return 1;
        }
        
        /* loop */
        while (times != 0)
        {
            timeout = 5000;
            g_flag = 0;
            while (timeout != 0)
            {
                timeout--;
                if (g_flag != 0)
                {
                    break;
                }
                l3gd20h_interface_delay_ms(1);
            }
            
            /* check timeout */
            if (timeout == 0)
            {
                l3gd20h_interface_debug_print("l3gd20h: fifo timeout.\n");
            }
            times--;
        }
        
        /* fifo deinit */
        (void)l3gd20h_fifo_deinit();
        
        /* gpio deinit */
        (void)gpio_interrupt_deinit();
        g_gpio_irq = NULL;
        
        return 0;
    }
    else if (strcmp("e_int", type) == 0)
    {
        uint8_t res;
        
        /* gpio init */
        res = gpio_interrupt_init();
        if (res != 0)
        {
            return 1;
        }
        
        /* set gpio irq */
        g_gpio_irq = l3gd20h_interrupt_irq_handler;
        
        /* interrupt init */
        res = l3gd20h_interrupt_init(interface, addr, threshold, a_l3gd20h_interrupt_receive_callback);
        if (res != 0)
        {
            (void)l3gd20h_interrupt_deinit();
            (void)gpio_interrupt_deinit();
            g_gpio_irq = NULL;
            
            return 1;
        }
        
        /* output */
        l3gd20h_interface_debug_print("l3gd20h: set threshold %0.2f.\n", threshold);
        
        /* set the timeout */
        timeout = 5000;
        g_flag = 0;
        while (timeout != 0)
        {
            timeout--;
            if (g_flag != 0)
            {
                break;
            }
            
            /* delay 1ms */
            l3gd20h_interface_delay_ms(1);
        }
        
        /* check the timeout */
        if (timeout == 0)
        {
            l3gd20h_interface_debug_print("l3gd20h: interrupt timeout.\n");
        }
        else
        {
            l3gd20h_interface_debug_print("l3gd20h: find interrupt.\n");
        }
        
        /* interrupt deinit */
        (void)l3gd20h_interrupt_deinit();
        
        /* gpio deinit */
        (void)gpio_interrupt_deinit();
        g_gpio_irq = NULL;
        
        return 0;
    }
    else if (strcmp("h", type) == 0)
    {
        help:
        l3gd20h_interface_debug_print("Usage:\n");
        l3gd20h_interface_debug_print("  l3gd20h (-i | --information)\n");
        l3gd20h_interface_debug_print("  l3gd20h (-h | --help)\n");
        l3gd20h_interface_debug_print("  l3gd20h (-p | --port)\n");
        l3gd20h_interface_debug_print("  l3gd20h (-t reg | --test=reg) [--addr=<0 | 1>] [--interface=<iic | spi>]\n");
        l3gd20h_interface_debug_print("  l3gd20h (-t read | --test=read) [--addr=<0 | 1>] [--interface=<iic | spi>] [--times=<num>]\n");
        l3gd20h_interface_debug_print("  l3gd20h (-t fifo | --test=fifo) [--addr=<0 | 1>] [--interface=<iic | spi>]\n");
        l3gd20h_interface_debug_print("  l3gd20h (-t int | --test=int) [--addr=<0 | 1>] [--interface=<iic | spi>]\n");
        l3gd20h_interface_debug_print("  l3gd20h (-e read | --example=read) [--addr=<0 | 1>] [--interface=<iic | spi>] [--times=<num>]\n");
        l3gd20h_interface_debug_print("  l3gd20h (-e fifo | --example=fifo) [--addr=<0 | 1>] [--interface=<iic | spi>] [--times=<num>] [--timeout=<ms>]\n");
        l3gd20h_interface_debug_print("  l3gd20h (-e int | --example=int) [--addr=<0 | 1>] [--interface=<iic | spi>] [--threshold=<th>] [--timeout=<ms>]\n");
        l3gd20h_interface_debug_print("\n");
        l3gd20h_interface_debug_print("Options:\n");
        l3gd20h_interface_debug_print("      --addr=<0 | 1>             Set the addr pin.([default: 0])\n");
        l3gd20h_interface_debug_print("  -e <read | fifo | int>, --example=<read | fifo | int>\n");
        l3gd20h_interface_debug_print("                                 Run the driver example.\n");
        l3gd20h_interface_debug_print("  -h, --help                     Show the help.\n");
        l3gd20h_interface_debug_print("  -i, --information              Show the chip information.\n");
        l3gd20h_interface_debug_print("      --interface=<iic | spi>    Set the chip interface.([default: iic])\n");
        l3gd20h_interface_debug_print("  -p, --port                     Display the pin connections of the current board.\n");
        l3gd20h_interface_debug_print("  -t <reg | read | fifo | init>, --test=<reg | read | fifo | int>\n");
        l3gd20h_interface_debug_print("                                 Run the driver test.\n");
        l3gd20h_interface_debug_print("      --threshold=<th>           Set the interrupt threshold.([default: 50.0f])\n");
        l3gd20h_interface_debug_print("      --times=<num>              Set the running times.([default: 3])\n");
        l3gd20h_interface_debug_print("      --timeout=<ms>             Set the interrupt timeout in ms.([default: 5000])\n");
        
        return 0;
    }
    else if (strcmp("i", type) == 0)
    {
        l3gd20h_info_t info;
        
        /* print l3gd20h info */
        l3gd20h_info(&info);
        l3gd20h_interface_debug_print("l3gd20h: chip is %s.\n", info.chip_name);
        l3gd20h_interface_debug_print("l3gd20h: manufacturer is %s.\n", info.manufacturer_name);
        l3gd20h_interface_debug_print("l3gd20h: interface is %s.\n", info.interface);
        l3gd20h_interface_debug_print("l3gd20h: driver version is %d.%d.\n", info.driver_version / 1000, (info.driver_version % 1000) / 100);
        l3gd20h_interface_debug_print("l3gd20h: min supply voltage is %0.1fV.\n", info.supply_voltage_min_v);
        l3gd20h_interface_debug_print("l3gd20h: max supply voltage is %0.1fV.\n", info.supply_voltage_max_v);
        l3gd20h_interface_debug_print("l3gd20h: max current is %0.2fmA.\n", info.max_current_ma);
        l3gd20h_interface_debug_print("l3gd20h: max temperature is %0.1fC.\n", info.temperature_max);
        l3gd20h_interface_debug_print("l3gd20h: min temperature is %0.1fC.\n", info.temperature_min);
        
        return 0;
    }
    else if (strcmp("p", type) == 0)
    {
        /* print pin connection */
        
        l3gd20h_interface_debug_print("l3gd20h: SPI interface SCK connected to GPIOA PIN5.\n");
        l3gd20h_interface_debug_print("l3gd20h: SPI interface MISO connected to GPIOA PIN6.\n");
        l3gd20h_interface_debug_print("l3gd20h: SPI interface MOSI connected to GPIOA PIN7.\n");
        l3gd20h_interface_debug_print("l3gd20h: SPI interface CS connected to GPIOA PIN4.\n");
        l3gd20h_interface_debug_print("l3gd20h: IIC interface SCL connected to GPIOB PIN8.\n");
        l3gd20h_interface_debug_print("l3gd20h: IIC interface SDA connected to GPIOB PIN9.\n");
        l3gd20h_interface_debug_print("l3gd20h: INT connected to GPIOB PIN0.\n");
        
        return 0;
    }
    else
    {
        return 5;
    }
}

/**
 * @brief main function
 * @note  none
 */
int main(void)
{
    uint8_t res;
    
    /* stm32f407 clock init and hal init */
    clock_init();
    
    /* delay init */
    delay_init();
    
    /* uart init */
    uart_init(115200);
    
    /* shell init && register l3gd20h function */
    shell_init();
    shell_register("l3gd20h", l3gd20h);
    uart_print("l3gd20h: welcome to libdriver l3gd20h.\n");
    
    while (1)
    {
        /* read uart */
        g_len = uart_read(g_buf, 256);
        if (g_len != 0)
        {
            /* run shell */
            res = shell_parse((char *)g_buf, g_len);
            if (res == 0)
            {
                /* run success */
            }
            else if (res == 1)
            {
                uart_print("l3gd20h: run failed.\n");
            }
            else if (res == 2)
            {
                uart_print("l3gd20h: unknown command.\n");
            }
            else if (res == 3)
            {
                uart_print("l3gd20h: length is too long.\n");
            }
            else if (res == 4)
            {
                uart_print("l3gd20h: pretreat failed.\n");
            }
            else if (res == 5)
            {
                uart_print("l3gd20h: param is invalid.\n");
            }
            else
            {
                uart_print("l3gd20h: unknown status code.\n");
            }
            uart_flush();
        }
        delay_ms(100);
    }
}
