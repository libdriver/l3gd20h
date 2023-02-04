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
 * @file      driver_l3gd20h_fifo_test.c
 * @brief     driver l3gd20h fifo test source file
 * @version   2.0.0
 * @author    Shifeng Li
 * @date      2021-05-29
 *
 * <h3>history</h3>
 * <table>
 * <tr><th>Date        <th>Version  <th>Author      <th>Description
 * <tr><td>2021/05/29  <td>2.0      <td>Shifeng Li  <td>format the code
 * <tr><td>2020/12/26  <td>1.0      <td>Shifeng Li  <td>first upload
 * </table>
 */

#include "driver_l3gd20h_fifo_test.h"
#include <stdlib.h>

static l3gd20h_handle_t gs_handle;        /**< l3gd20h handle */
static volatile uint8_t g_flag = 0;       /**< fifo flag */
static int16_t gs_raw[32][3];             /**< raw data */
static float gs_dps[32][3];               /**< dps */
static volatile uint16_t gs_len;          /**< length */

/**
 * @brief  fifo test irq callback
 * @return status code
 *         - 0 success
 *         - 1 run failed
 * @note   none
 */
uint8_t l3gd20h_fifo_test_irq_handler(void)
{
    if (l3gd20h_irq_handler(&gs_handle, 2) != 0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/**
 * @brief     interface receive callback
 * @param[in] type is the irq type
 * @note      none
 */
static void a_l3gd20h_interface_receive_callback(uint8_t type)
{
    switch (type)
    {
        case L3GD20H_INTERRUPT2_FIFO_THRESHOLD :
        {
            uint8_t res;
            
            gs_len = 32;
            res = l3gd20h_read(&gs_handle, (int16_t (*)[3])(gs_raw), (float (*)[3])(gs_dps), (uint16_t *)&gs_len);
            if (res == 0)
            {
                g_flag = 1;
            }
            
            /* disable fifo */
            res = l3gd20h_set_fifo(&gs_handle, L3GD20H_BOOL_FALSE);
            if (res != 0)
            {
                l3gd20h_interface_debug_print("l3gd20h: set fifo failed.\n");
                
                return;
            }
           
            l3gd20h_interface_debug_print("l3gd20h: irq fifo threshold with %d.\n", gs_len);
            
            break;
        }
        case L3GD20H_INTERRUPT2_FIFO_OVERRRUN :
        {
            uint8_t res;
            
            gs_len = 32;
            res = l3gd20h_read(&gs_handle, (int16_t (*)[3])(gs_raw), (float (*)[3])(gs_dps), (uint16_t *)&gs_len);
            if (res == 0)
            {
                g_flag = 1;
            }
            
            /* disable fifo */
            res = l3gd20h_set_fifo(&gs_handle, L3GD20H_BOOL_FALSE);
            if (res != 0)
            {
                l3gd20h_interface_debug_print("l3gd20h: set fifo failed.\n");
                
                return;
            }
            
            l3gd20h_interface_debug_print("l3gd20h: irq fifo overrun with %d.\n", gs_len);
            
            break;
        }
        case L3GD20H_INTERRUPT2_FIFO_EMPTY :
        {
            l3gd20h_interface_debug_print("l3gd20h: irq fifo empty.\n");
            
            break;
        }
        default :
        {
            break;
        }
    }
}

/**
 * @brief     fifo test
 * @param[in] interface is the chip interface
 * @param[in] addr_pin is the iic device address
 * @return    status code
 *            - 0 success
 *            - 1 test failed
 * @note      none
 */
uint8_t l3gd20h_fifo_test(l3gd20h_interface_t interface, l3gd20h_address_t addr_pin)
{
    uint8_t res;
    uint16_t threshold;
    uint32_t timeout;
    l3gd20h_info_t info;
    
    /* link interface function */
    DRIVER_L3GD20H_LINK_INIT(&gs_handle, l3gd20h_handle_t);
    DRIVER_L3GD20H_LINK_IIC_INIT(&gs_handle, l3gd20h_interface_iic_init);
    DRIVER_L3GD20H_LINK_IIC_DEINIT(&gs_handle, l3gd20h_interface_iic_deinit);
    DRIVER_L3GD20H_LINK_IIC_READ(&gs_handle, l3gd20h_interface_iic_read);
    DRIVER_L3GD20H_LINK_IIC_WRITE(&gs_handle, l3gd20h_interface_iic_write);
    DRIVER_L3GD20H_LINK_SPI_INIT(&gs_handle, l3gd20h_interface_spi_init);
    DRIVER_L3GD20H_LINK_SPI_DEINIT(&gs_handle, l3gd20h_interface_spi_deinit);
    DRIVER_L3GD20H_LINK_SPI_READ(&gs_handle, l3gd20h_interface_spi_read);
    DRIVER_L3GD20H_LINK_SPI_WRITE(&gs_handle, l3gd20h_interface_spi_write);
    DRIVER_L3GD20H_LINK_DELAY_MS(&gs_handle, l3gd20h_interface_delay_ms);
    DRIVER_L3GD20H_LINK_DEBUG_PRINT(&gs_handle, l3gd20h_interface_debug_print);
    DRIVER_L3GD20H_LINK_RECEIVE_CALLBACK(&gs_handle, a_l3gd20h_interface_receive_callback);
    
    /* get information */
    res = l3gd20h_info(&info);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: get info failed.\n");
       
        return 1;
    }
    else
    {
        /* print chip info */
        l3gd20h_interface_debug_print("l3gd20h: chip is %s.\n", info.chip_name);
        l3gd20h_interface_debug_print("l3gd20h: manufacturer is %s.\n", info.manufacturer_name);
        l3gd20h_interface_debug_print("l3gd20h: interface is %s.\n", info.interface);
        l3gd20h_interface_debug_print("l3gd20h: driver version is %d.%d.\n", info.driver_version / 1000, (info.driver_version % 1000) / 100);
        l3gd20h_interface_debug_print("l3gd20h: min supply voltage is %0.1fV.\n", info.supply_voltage_min_v);
        l3gd20h_interface_debug_print("l3gd20h: max supply voltage is %0.1fV.\n", info.supply_voltage_max_v);
        l3gd20h_interface_debug_print("l3gd20h: max current is %0.2fmA.\n", info.max_current_ma);
        l3gd20h_interface_debug_print("l3gd20h: max temperature is %0.1fC.\n", info.temperature_max);
        l3gd20h_interface_debug_print("l3gd20h: min temperature is %0.1fC.\n", info.temperature_min);
    }
    
    /* start fifo test */
    l3gd20h_interface_debug_print("l3gd20h: start fifo test.\n");
    
    /* set the interface */
    res = l3gd20h_set_interface(&gs_handle, interface);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: set interface failed.\n");
       
        return 1;
    }
    
    /* set the address pin */
    res = l3gd20h_set_addr_pin(&gs_handle, addr_pin);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: set addr pin failed.\n");
       
        return 1;
    }
    
    /* init the l3gd20h */
    res = l3gd20h_init(&gs_handle);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: init failed.\n");
       
        return 1;
    }
   
    /* set normal boot */
    res = l3gd20h_set_boot(&gs_handle, L3GD20H_BOOT_NORMAL);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: set boot failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    
    /* enable x axis*/
    res = l3gd20h_set_axis(&gs_handle, L3GD20H_AXIS_X, L3GD20H_BOOL_TRUE);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: set axis failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    
    /* enable y axis*/
    res = l3gd20h_set_axis(&gs_handle, L3GD20H_AXIS_Y, L3GD20H_BOOL_TRUE);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: set axis failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    
    /* enable z axis*/
    res = l3gd20h_set_axis(&gs_handle, L3GD20H_AXIS_Z, L3GD20H_BOOL_TRUE);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: set axis failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set low power 12.5 Hz */
    res = l3gd20h_set_rate_bandwidth(&gs_handle, L3GD20H_LOW_ODR_1_ODR_12P5HZ_BW_0_NA);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: set rate bandwidth failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    
    /* disable edge trigger */
    res = l3gd20h_set_edge_trigger(&gs_handle, L3GD20H_BOOL_FALSE);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: set edge trigger failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    
    /* disable level trigger */
    res = l3gd20h_set_level_trigger(&gs_handle, L3GD20H_BOOL_FALSE);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: set level trigger failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    
    /* enable latched */
    res = l3gd20h_set_level_sensitive_latched(&gs_handle, L3GD20H_BOOL_TRUE);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: set level sensitive latched failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    
    /* normal mode */
    res = l3gd20h_set_self_test(&gs_handle, L3GD20H_SELF_TEST_NORMAL);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: set self test failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set high pass filter normal mode */
    res = l3gd20h_set_high_pass_filter_mode(&gs_handle, L3GD20H_HIGH_PASS_FILTER_MODE_NORMAL);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: set high pass filter normal mode failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set high pass filter cut-off frequency 0 */
    res = l3gd20h_set_high_pass_filter_cut_off_frequency(&gs_handle, L3GD20H_HIGH_PASS_FILTER_CUT_OFF_FREQUENCY_0);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: set high pass filter cut off frequency failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    
    /* enable high pass filter */
    res = l3gd20h_set_high_pass_filter(&gs_handle, L3GD20H_BOOL_TRUE);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: set high pass filter failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    
    /* disable block data update */
    res = l3gd20h_set_block_data_update(&gs_handle, L3GD20H_BOOL_FALSE);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: set block data update failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set LPF1-HPF-LPF2 */
    res = l3gd20h_set_out_selection(&gs_handle, L3GD20H_SELECTION_LPF1_HPF_LPF2);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: set out selection failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set LPF1-HPF-LPF2 */
    res = l3gd20h_set_interrupt_selection(&gs_handle, L3GD20H_SELECTION_LPF1_HPF_LPF2);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: set interrupt selection failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set high pass filter reference 0x00 */
    res = l3gd20h_set_high_pass_filter_reference(&gs_handle, 0x00);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: set high pass filter reference failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    
    /* fifo mode */
    res = l3gd20h_set_fifo_mode(&gs_handle, L3GD20H_FIFO_MODE_FIFO);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: set fifo mode failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    
    /* disable interrupt 1*/
    res = l3gd20h_set_interrupt1(&gs_handle, L3GD20H_BOOL_FALSE);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: set interrupt1 failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    
    /* disable boot on interrupt 1*/
    res = l3gd20h_set_boot_on_interrupt1(&gs_handle, L3GD20H_BOOL_FALSE);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: set boot on interrupt1 failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    
    /* active level low */
    res = l3gd20h_set_interrupt_active_level(&gs_handle, L3GD20H_INTERRUPT_ACTIVE_LEVEL_LOW);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: set interrupt active level failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    
    /* push-pull pin type */
    res = l3gd20h_set_interrupt_pin_type(&gs_handle, L3GD20H_PIN_PUSH_PULL);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: set interrupt pin type failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    
    /* disable data ready on interrupt2 */
    res = l3gd20h_set_data_ready_on_interrupt2(&gs_handle, L3GD20H_BOOL_FALSE);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: set data ready on interrupt2 failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    
    /* enable fifo threshold on interrupt2 */
    res = l3gd20h_set_fifo_threshold_on_interrupt2(&gs_handle, L3GD20H_BOOL_TRUE);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: set fifo threshold on interrupt2 failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    
    /* enable fifo overrun on interrupt2 */
    res = l3gd20h_set_fifo_overrun_on_interrupt2(&gs_handle, L3GD20H_BOOL_TRUE);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: set fifo overrun on interrupt2 failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    
    /* disable fifo empty on interrupt2 */
    res = l3gd20h_set_fifo_empty_on_interrupt2(&gs_handle, L3GD20H_BOOL_FALSE);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: set fifo empty on interrupt2 failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    
    /* disable L3GD20H_INTERRUPT_EVENT_AND_OR_COMBINATION */
    res = l3gd20h_set_interrupt_event(&gs_handle, L3GD20H_INTERRUPT_EVENT_AND_OR_COMBINATION, L3GD20H_BOOL_FALSE);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: set interrupt event failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    
    /* disable L3GD20H_INTERRUPT_EVENT_LATCH */
    res = l3gd20h_set_interrupt_event(&gs_handle, L3GD20H_INTERRUPT_EVENT_LATCH, L3GD20H_BOOL_FALSE);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: set interrupt event failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    
    /* disable L3GD20H_INTERRUPT_EVENT_Z_HIGH_EVENT */
    res = l3gd20h_set_interrupt_event(&gs_handle, L3GD20H_INTERRUPT_EVENT_Z_HIGH_EVENT, L3GD20H_BOOL_FALSE);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: set interrupt event failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    
    /* disable L3GD20H_INTERRUPT_EVENT_Z_LOW_EVENT */
    res = l3gd20h_set_interrupt_event(&gs_handle, L3GD20H_INTERRUPT_EVENT_Z_LOW_EVENT, L3GD20H_BOOL_FALSE);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: set interrupt event failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    
    /* disable L3GD20H_INTERRUPT_EVENT_Y_HIGH_EVENT */
    res = l3gd20h_set_interrupt_event(&gs_handle, L3GD20H_INTERRUPT_EVENT_Y_HIGH_EVENT, L3GD20H_BOOL_FALSE);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: set interrupt event failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    
    /* disable L3GD20H_INTERRUPT_EVENT_Y_LOW_EVENT */
    res = l3gd20h_set_interrupt_event(&gs_handle, L3GD20H_INTERRUPT_EVENT_Y_LOW_EVENT, L3GD20H_BOOL_FALSE);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: set interrupt event failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    
    /* disable L3GD20H_INTERRUPT_EVENT_X_HIGH_EVENT */
    res = l3gd20h_set_interrupt_event(&gs_handle, L3GD20H_INTERRUPT_EVENT_X_HIGH_EVENT, L3GD20H_BOOL_FALSE);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: set interrupt event failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    
    /* disable L3GD20H_INTERRUPT_EVENT_X_LOW_EVENT */
    res = l3gd20h_set_interrupt_event(&gs_handle, L3GD20H_INTERRUPT_EVENT_X_LOW_EVENT, L3GD20H_BOOL_FALSE);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: set interrupt event failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    
    /* convert the threshold */
    res = l3gd20h_interrupt_threshold_convert_to_register(&gs_handle, 0.0f, (uint16_t *)&threshold);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: convert to register failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set x interrupt threshold */
    res = l3gd20h_set_x_interrupt_threshold(&gs_handle, threshold);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: set x interrupt threshold failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set y interrupt threshold */
    res = l3gd20h_set_y_interrupt_threshold(&gs_handle, threshold);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: set y interrupt threshold failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set z interrupt threshold */
    res = l3gd20h_set_z_interrupt_threshold(&gs_handle, threshold);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: set z interrupt threshold failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    
    /* reset counter mode */
    res = l3gd20h_set_counter_mode(&gs_handle, L3GD20H_COUNTER_MODE_RESET);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: set counter mode failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    
    /* enable wait */
    res = l3gd20h_set_wait(&gs_handle, L3GD20H_BOOL_TRUE);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: set wait failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set duration 0x01 */
    res = l3gd20h_set_duration(&gs_handle, 0x01);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: set duration failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set data ready active level low */
    res = l3gd20h_set_data_ready_active_level(&gs_handle, L3GD20H_INTERRUPT_ACTIVE_LEVEL_LOW);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: set data ready active level failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }

    /* little endian */
    res = l3gd20h_set_data_format(&gs_handle, L3GD20H_DATA_FORMAT_LITTLE_ENDIAN);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: set data format failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    
    /* ±245 dps */
    res = l3gd20h_set_full_scale(&gs_handle, L3GD20H_FULL_SCALE_245_DPS);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: set full scale failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    
    /* normal mode */
    res = l3gd20h_set_mode(&gs_handle, L3GD20H_MODE_NORMAL);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: set mode failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    
    /* disable stop on fifo threshold */
    res = l3gd20h_set_stop_on_fifo_threshold(&gs_handle, L3GD20H_BOOL_FALSE);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: set stop on fifo threshold failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    
    /* fifo threshold 16 */
    res = l3gd20h_set_fifo_threshold(&gs_handle, 16);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: set fifo threshold failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }

    /* enable fifo */
    res = l3gd20h_set_fifo(&gs_handle, L3GD20H_BOOL_TRUE);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: set fifo failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    
    timeout = 5000;
    g_flag = 0;
    while (timeout != 0)
    {
        if (g_flag != 0)
        {
            l3gd20h_interface_debug_print("l3gd20h: find interrupt.\n");
            
            break;
        }
        l3gd20h_interface_delay_ms(1);
        timeout--;
    }
    if (timeout == 0)
    {
        /* fifo test failed */
        l3gd20h_interface_debug_print("l3gd20h: fifo test failed.\n");
        
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    else
    {
        /* finish fifo test */
        l3gd20h_interface_debug_print("l3gd20h: finish fifo test.\n");
        
        (void)l3gd20h_deinit(&gs_handle);
        
        return 0;
    }
}
