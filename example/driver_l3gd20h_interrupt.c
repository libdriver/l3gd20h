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
 * @file      driver_l3gd20h_interrupt.c
 * @brief     driver l3gd20h interrupt source file
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

#include "driver_l3gd20h_interrupt.h"

static l3gd20h_handle_t gs_handle;                     /**< l3gd20h handle */
static void (*a_callback)(uint8_t type) = NULL;        /**< irq callback */

/**
 * @brief  interrupt example irq callback
 * @return status code
 *         - 0 success
 *         - 1 run failed
 * @note   none
 */
uint8_t l3gd20h_interrupt_irq_handler(void)
{
    if (l3gd20h_irq_handler(&gs_handle, 1) != 0)
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
        case L3GD20H_INTERRUPT1_INTERRUPT_ACTIVE :
        {
            if (a_callback != NULL)
            {
                a_callback(L3GD20H_INTERRUPT1_INTERRUPT_ACTIVE);
            }
            
            break;
        }
        case L3GD20H_INTERRUPT1_Z_HIGH :
        {
            if (a_callback != NULL)
            {
                a_callback(L3GD20H_INTERRUPT1_Z_HIGH);
            }
            
            break;
        }
        case L3GD20H_INTERRUPT1_Z_LOW :
        {
            if (a_callback != NULL)
            {
                a_callback(L3GD20H_INTERRUPT1_Z_LOW);
            }
            
            break;
        }
        case L3GD20H_INTERRUPT1_Y_HIGH :
        {
            if (a_callback != NULL)
            {
                a_callback(L3GD20H_INTERRUPT1_Y_HIGH);
            }
            
            break;
        }
        case L3GD20H_INTERRUPT1_Y_LOW :
        {
            if (a_callback != NULL)
            {
                a_callback(L3GD20H_INTERRUPT1_Y_LOW);
            }
            
            break;
        }
        case L3GD20H_INTERRUPT1_X_HIGH :
        {
            if (a_callback != NULL)
            {
                a_callback(L3GD20H_INTERRUPT1_X_HIGH);
            }
            
            break;
        }
        case L3GD20H_INTERRUPT1_X_LOW :
        {
            if (a_callback != NULL)
            {
                a_callback(L3GD20H_INTERRUPT1_X_LOW);
            }
            
            break;
        }
        default :
        {
            break;
        }
    }
}

/**
 * @brief     interrupt example init
 * @param[in] interface is the chip interface
 * @param[in] addr_pin is the iic device address
 * @param[in] interrupt_threshold is the interrupt threshold
 * @param[in] *callback points to a irq callback address
 * @return    status code
 *            - 0 success
 *            - 1 init failed
 * @note      none
 */
uint8_t l3gd20h_interrupt_init(l3gd20h_interface_t interface, l3gd20h_address_t addr_pin,
                               float interrupt_threshold, void (*callback)(uint8_t type))
{
    uint8_t res;
    uint16_t threshold;

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
   
    /* set boot */
    res = l3gd20h_set_boot(&gs_handle, L3GD20H_INTERRUPT_DEFAULT_BOOT);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: set boot failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set x axis*/
    res = l3gd20h_set_axis(&gs_handle, L3GD20H_AXIS_X, L3GD20H_INTERRUPT_DEFAULT_AXIS_X);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: set axis failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set y axis*/
    res = l3gd20h_set_axis(&gs_handle, L3GD20H_AXIS_Y, L3GD20H_INTERRUPT_DEFAULT_AXIS_Y);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: set axis failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set z axis*/
    res = l3gd20h_set_axis(&gs_handle, L3GD20H_AXIS_Z, L3GD20H_INTERRUPT_DEFAULT_AXIS_Z);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: set axis failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set rate bandwidth */
    res = l3gd20h_set_rate_bandwidth(&gs_handle, L3GD20H_INTERRUPT_DEFAULT_RATE_BANDWIDTH);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: set rate bandwidth failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set edge trigger */
    res = l3gd20h_set_edge_trigger(&gs_handle, L3GD20H_INTERRUPT_DEFAULT_EDGE_TRIGGER);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: set edge trigger failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set level trigger */
    res = l3gd20h_set_level_trigger(&gs_handle, L3GD20H_INTERRUPT_DEFAULT_LEVEL_TRIGGER);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: set level trigger failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set latched */
    res = l3gd20h_set_level_sensitive_latched(&gs_handle, L3GD20H_INTERRUPT_DEFAULT_LEVEL_SENSITIVE_LATCHED);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: set level sensitive latched failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set self test */
    res = l3gd20h_set_self_test(&gs_handle, L3GD20H_INTERRUPT_DEFAULT_SELF_TEST);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: set self test failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set high pass filter mode */
    res = l3gd20h_set_high_pass_filter_mode(&gs_handle, L3GD20H_INTERRUPT_DEFAULT_HIGH_PASS_FILTER_MODE);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: set high pass filter normal mode failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set high pass filter cut off */
    res = l3gd20h_set_high_pass_filter_cut_off_frequency(&gs_handle, L3GD20H_INTERRUPT_DEFAULT_HIGH_PASS_FILTER_CUT_OFF);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: set high pass filter cut off frequency failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set high pass filter */
    res = l3gd20h_set_high_pass_filter(&gs_handle, L3GD20H_INTERRUPT_DEFAULT_HIGH_PASS_FILTER);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: set high pass filter failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set block data update */
    res = l3gd20h_set_block_data_update(&gs_handle, L3GD20H_INTERRUPT_DEFAULT_BLOCK_DATA_UPDATE);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: set block data update failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set out selection */
    res = l3gd20h_set_out_selection(&gs_handle, L3GD20H_INTERRUPT_DEFAULT_OUT_SELECTION);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: set out selection failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set interrupt selection */
    res = l3gd20h_set_interrupt_selection(&gs_handle, L3GD20H_INTERRUPT_DEFAULT_INTERRUPT_SELECTION);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: set interrupt selection failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set high pass filter reference */
    res = l3gd20h_set_high_pass_filter_reference(&gs_handle, L3GD20H_INTERRUPT_DEFAULT_HIGH_PASS_FILTER_REFERENCE);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: set high pass filter reference failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set bypass fifo mode */
    res = l3gd20h_set_fifo_mode(&gs_handle, L3GD20H_FIFO_MODE_BYPASS);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: set fifo mode failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set interrupt 1*/
    res = l3gd20h_set_interrupt1(&gs_handle, L3GD20H_BOOL_TRUE);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: set interrupt1 failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set boot on interrupt 1*/
    res = l3gd20h_set_boot_on_interrupt1(&gs_handle, L3GD20H_INTERRUPT_DEFAULT_BOOT_ON_INTERRUPT1);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: set boot on interrupt1 failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set active level */
    res = l3gd20h_set_interrupt_active_level(&gs_handle, L3GD20H_INTERRUPT_DEFAULT_INTERRUPT_ACTIVE_LEVEL);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: set interrupt active level failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set interrupt pin type */
    res = l3gd20h_set_interrupt_pin_type(&gs_handle, L3GD20H_INTERRUPT_DEFAULT_INTERRUPT_PIN_TYPE);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: set interrupt pin type failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set data ready on interrupt2 */
    res = l3gd20h_set_data_ready_on_interrupt2(&gs_handle, L3GD20H_INTERRUPT_DEFAULT_DATA_READY_ON_INTERRUPT2);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: set data ready on interrupt2 failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set fifo threshold on interrupt2 */
    res = l3gd20h_set_fifo_threshold_on_interrupt2(&gs_handle, L3GD20H_INTERRUPT_DEFAULT_FIFO_THRESHOLD_ON_INTERRUPT2);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: set fifo threshold on interrupt2 failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set fifo overrun on interrupt2 */
    res = l3gd20h_set_fifo_overrun_on_interrupt2(&gs_handle, L3GD20H_INTERRUPT_DEFAULT_FIFO_OVERRUN_ON_INTERRUPT2);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: set fifo overrun on interrupt2 failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set fifo empty on interrupt2 */
    res = l3gd20h_set_fifo_empty_on_interrupt2(&gs_handle, L3GD20H_INTERRUPT_DEFAULT_FIFO_EMPTY_ON_INTERRUPT2);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: set fifo empty on interrupt2 failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set L3GD20H_INTERRUPT_EVENT_AND_OR_COMBINATION */
    res = l3gd20h_set_interrupt_event(&gs_handle, L3GD20H_INTERRUPT_EVENT_AND_OR_COMBINATION, 
                                      L3GD20H_INTERRUPT_DEFAULT_INTERRUPT_EVENT_AND_OR_COMBINATION);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: set interrupt event failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set L3GD20H_INTERRUPT_EVENT_LATCH */
    res = l3gd20h_set_interrupt_event(&gs_handle, L3GD20H_INTERRUPT_EVENT_LATCH, 
                                      L3GD20H_INTERRUPT_DEFAULT_INTERRUPT_EVENT_LATCH);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: set interrupt event failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set L3GD20H_INTERRUPT_EVENT_Z_HIGH_EVENT */
    res = l3gd20h_set_interrupt_event(&gs_handle, L3GD20H_INTERRUPT_EVENT_Z_HIGH_EVENT, 
                                      L3GD20H_INTERRUPT_DEFAULT_INTERRUPT_EVENT_Z_HIGH_EVENT);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: set interrupt event failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set L3GD20H_INTERRUPT_EVENT_Z_LOW_EVENT */
    res = l3gd20h_set_interrupt_event(&gs_handle, L3GD20H_INTERRUPT_EVENT_Z_LOW_EVENT, 
                                      L3GD20H_INTERRUPT_DEFAULT_INTERRUPT_EVENT_Z_LOW_EVENT);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: set interrupt event failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set L3GD20H_INTERRUPT_EVENT_Y_HIGH_EVENT */
    res = l3gd20h_set_interrupt_event(&gs_handle, L3GD20H_INTERRUPT_EVENT_Y_HIGH_EVENT, 
                                      L3GD20H_INTERRUPT_DEFAULT_INTERRUPT_EVENT_Y_HIGH_EVENT);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: set interrupt event failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set L3GD20H_INTERRUPT_EVENT_Y_LOW_EVENT */
    res = l3gd20h_set_interrupt_event(&gs_handle, L3GD20H_INTERRUPT_EVENT_Y_LOW_EVENT, 
                                      L3GD20H_INTERRUPT_DEFAULT_INTERRUPT_EVENT_Y_LOW_EVENT);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: set interrupt event failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set L3GD20H_INTERRUPT_EVENT_X_HIGH_EVENT */
    res = l3gd20h_set_interrupt_event(&gs_handle, L3GD20H_INTERRUPT_EVENT_X_HIGH_EVENT, 
                                      L3GD20H_INTERRUPT_DEFAULT_INTERRUPT_EVENT_X_HIGH_EVENT);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: set interrupt event failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set L3GD20H_INTERRUPT_EVENT_X_LOW_EVENT */
    res = l3gd20h_set_interrupt_event(&gs_handle, L3GD20H_INTERRUPT_EVENT_X_LOW_EVENT, 
                                      L3GD20H_INTERRUPT_DEFAULT_INTERRUPT_EVENT_X_LOW_EVENT);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: set interrupt event failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    
    /* convert the threshold */
    res = l3gd20h_interrupt_threshold_convert_to_register(&gs_handle, interrupt_threshold, 
                                                         (uint16_t *)&threshold);
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
    
    /* set counter mode */
    res = l3gd20h_set_counter_mode(&gs_handle, L3GD20H_INTERRUPT_DEFAULT_COUNTER_MODE);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: set counter mode failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set wait */
    res = l3gd20h_set_wait(&gs_handle, L3GD20H_INTERRUPT_DEFAULT_WAIT);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: set wait failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set duration */
    res = l3gd20h_set_duration(&gs_handle, L3GD20H_INTERRUPT_DEFAULT_DURATION);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: set duration failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set data ready active level */
    res = l3gd20h_set_data_ready_active_level(&gs_handle, L3GD20H_INTERRUPT_DEFAULT_DATA_READY_ACTIVE_LEVEL);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: set data ready active level failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    
    /* enable fifo */
    res = l3gd20h_set_fifo(&gs_handle, L3GD20H_BOOL_FALSE);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: set fifo failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set stop on fifo threshold */
    res = l3gd20h_set_stop_on_fifo_threshold(&gs_handle, L3GD20H_INTERRUPT_DEFAULT_STOP_ON_FIFO_THRESHOLD);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: set stop on fifo threshold failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set fifo threshold */
    res = l3gd20h_set_fifo_threshold(&gs_handle, L3GD20H_INTERRUPT_DEFAULT_FIFO_THRESHOLD);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: set fifo threshold failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set endian */
    res = l3gd20h_set_data_format(&gs_handle, L3GD20H_INTERRUPT_DEFAULT_DATA_FORMAT);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: set data format failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set full scale */
    res = l3gd20h_set_full_scale(&gs_handle, L3GD20H_INTERRUPT_DEFAULT_FULL_SCALE);
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
    
    a_callback = callback;
    
    /* success return 0 */
    return 0;
}

/**
 * @brief  interrupt example deinit
 * @return status code
 *         - 0 success
 *         - 1 deinit failed
 * @note   none
 */
uint8_t l3gd20h_interrupt_deinit(void)
{
    if (l3gd20h_deinit(&gs_handle) != 0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}


