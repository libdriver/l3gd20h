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
 * @file      driver_l3gd20h_register_test.c
 * @brief     driver l3gd20h register test source file
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

#include "driver_l3gd20h_register_test.h"
#include <stdlib.h>

static l3gd20h_handle_t gs_handle;        /**< l3gd20h handle */

/**
 * @brief     register test
 * @param[in] interface is the chip interface
 * @param[in] addr_pin is the iic device address
 * @return    status code
 *            - 0 success
 *            - 1 test failed
 * @note      none
 */
uint8_t l3gd20h_register_test(l3gd20h_interface_t interface, l3gd20h_address_t addr_pin)
{
    uint8_t res;
    uint8_t value, value_check;
    uint8_t duration, duration_check;
    uint16_t threshold, threshold_check;
    float dps, dps_check;
    uint16_t reg;
    l3gd20h_info_t info;
    l3gd20h_boot_t boot;
    l3gd20h_interface_t interface_test;
    l3gd20h_address_t addr;
    l3gd20h_mode_t mode;
    l3gd20h_bool_t enable;
    l3gd20h_lodr_odr_bw_t rate_bandwidth;
    l3gd20h_high_pass_filter_mode_t high_mode;
    l3gd20h_high_pass_filter_cut_off_frequency_t frequency;
    l3gd20h_data_format_t data_format;
    l3gd20h_full_scale_t full_scale;
    l3gd20h_self_test_t self_test;
    l3gd20h_spi_wire_t spi_wire;
    l3gd20h_selection_t selection;
    l3gd20h_interrupt_active_level_t level;
    l3gd20h_pin_type_t pin_type;
    l3gd20h_counter_mode_t counter_mode;
    l3gd20h_interrupt_active_level_t active_level;
    l3gd20h_fifo_mode_t fifo_mode;
  
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
    DRIVER_L3GD20H_LINK_RECEIVE_CALLBACK(&gs_handle, l3gd20h_interface_receive_callback);
    
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
    
    /* start register test */
    l3gd20h_interface_debug_print("l3gd20h: start register test.\n");
    
    /* l3gd20h_set_interface/l3gd20h_get_interface test */
    l3gd20h_interface_debug_print("l3gd20h: l3gd20h_set_interface/l3gd20h_get_interface test.\n");
    
    /* set IIC interface */
    res = l3gd20h_set_interface(&gs_handle, L3GD20H_INTERFACE_IIC);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: set interface failed.\n");
       
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: set interface iic.\n");
    res = l3gd20h_get_interface(&gs_handle, &interface_test);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: get interface failed.\n");
       
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: check interface %s.\n", interface_test == L3GD20H_INTERFACE_IIC ? "ok" : "error");
    
    /* set SPI interface */
    res = l3gd20h_set_interface(&gs_handle, L3GD20H_INTERFACE_SPI);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: set interface failed.\n");
       
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: set interface spi.\n");
    res = l3gd20h_get_interface(&gs_handle, &interface_test);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: get interface failed.\n");
       
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: check interface %s.\n", interface_test == L3GD20H_INTERFACE_SPI ? "ok" : "error");
    
    /* l3gd20h_set_addr_pin/l3gd20h_get_addr_pin test */
    l3gd20h_interface_debug_print("l3gd20h: l3gd20h_set_addr_pin/l3gd20h_get_addr_pin test.\n");
    
    /* sdo 0 */
    res = l3gd20h_set_addr_pin(&gs_handle, L3GD20H_ADDRESS_SDO_0);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: set addr pin failed.\n");
       
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: set addr pin sdo0.\n");
    res = l3gd20h_get_addr_pin(&gs_handle, &addr);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: get addr pin failed.\n");
       
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: check addr pin %s.\n", addr == L3GD20H_ADDRESS_SDO_0 ? "ok" : "error");
    
    /* sdo 1 */
    res = l3gd20h_set_addr_pin(&gs_handle, L3GD20H_ADDRESS_SDO_1);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: set addr pin failed.\n");
       
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: set addr pin sdo1.\n");
    res = l3gd20h_get_addr_pin(&gs_handle, &addr);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: get addr pin failed.\n");
       
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: check addr pin %s.\n", addr == L3GD20H_ADDRESS_SDO_1 ? "ok" : "error");
    
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
    
    /* l3gd20h_set_mode/l3gd20h_get_mode test */
    l3gd20h_interface_debug_print("l3gd20h: l3gd20h_set_mode/l3gd20h_get_mode test.\n");
    
    /* power down mode */
    res = l3gd20h_set_mode(&gs_handle, L3GD20H_MODE_POWER_DOWN);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: set mode failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: set power down mode.\n");
    res = l3gd20h_get_mode(&gs_handle, &mode);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: get mode failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: check mode %s.\n", mode == L3GD20H_MODE_POWER_DOWN ? "ok" : "error");
    
    /* sleep mode */
    res = l3gd20h_set_mode(&gs_handle, L3GD20H_MODE_SLEEP);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: set mode failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: set sleep mode.\n");
    res = l3gd20h_get_mode(&gs_handle, &mode);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: get mode failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: check mode %s.\n", mode == L3GD20H_MODE_SLEEP ? "ok" : "error"); 
    
    /* normal mode */
    res = l3gd20h_set_mode(&gs_handle, L3GD20H_MODE_NORMAL);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: set mode failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: set normal mode.\n");
    res = l3gd20h_get_mode(&gs_handle, &mode);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: get mode failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: check mode %s.\n", mode == L3GD20H_MODE_NORMAL ? "ok" : "error"); 
    
    /* l3gd20h_set_axis/l3gd20h_get_axis test */
    l3gd20h_interface_debug_print("l3gd20h: l3gd20h_set_axis/l3gd20h_get_axis test.\n");
    
    /* axis x false */
    res = l3gd20h_set_axis(&gs_handle, L3GD20H_AXIS_X, L3GD20H_BOOL_FALSE);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: set axis failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: set axis x false.\n");
    res = l3gd20h_get_axis(&gs_handle, L3GD20H_AXIS_X, &enable);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: get axis failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: check axis %s.\n", enable == L3GD20H_BOOL_FALSE ? "ok" : "error"); 
    
    /* axis x true */
    res = l3gd20h_set_axis(&gs_handle, L3GD20H_AXIS_X, L3GD20H_BOOL_TRUE);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: set axis failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: set axis x true.\n");
    res = l3gd20h_get_axis(&gs_handle, L3GD20H_AXIS_X, &enable);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: get axis failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: check axis %s.\n", enable == L3GD20H_BOOL_TRUE ? "ok" : "error"); 
    
    /* axis y false */
    res = l3gd20h_set_axis(&gs_handle, L3GD20H_AXIS_Y, L3GD20H_BOOL_FALSE);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: set axis failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: set axis y false.\n");
    res = l3gd20h_get_axis(&gs_handle, L3GD20H_AXIS_Y, &enable);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: get axis failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: check axis %s.\n", enable == L3GD20H_BOOL_FALSE ? "ok" : "error"); 
    
    /* axis y true */
    res = l3gd20h_set_axis(&gs_handle, L3GD20H_AXIS_Y, L3GD20H_BOOL_TRUE);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: set axis failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: set axis y true.\n");
    res = l3gd20h_get_axis(&gs_handle, L3GD20H_AXIS_Y, &enable);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: get axis failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: check axis %s.\n", enable == L3GD20H_BOOL_TRUE ? "ok" : "error"); 
    
    /* axis z false */
    res = l3gd20h_set_axis(&gs_handle, L3GD20H_AXIS_Z, L3GD20H_BOOL_FALSE);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: set axis failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: set axis z false.\n");
    res = l3gd20h_get_axis(&gs_handle, L3GD20H_AXIS_Z, &enable);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: get axis failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: check axis %s.\n", enable == L3GD20H_BOOL_FALSE ? "ok" : "error"); 
    
    /* axis z true */
    res = l3gd20h_set_axis(&gs_handle, L3GD20H_AXIS_Z, L3GD20H_BOOL_TRUE);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: set axis failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: set axis z true.\n");
    res = l3gd20h_get_axis(&gs_handle, L3GD20H_AXIS_Z, &enable);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: get axis failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: check axis %s.\n", enable == L3GD20H_BOOL_TRUE ? "ok" : "error"); 

    /* l3gd20h_set_rate_bandwidth/l3gd20h_get_rate_bandwidth test */
    l3gd20h_interface_debug_print("l3gd20h: l3gd20h_set_rate_bandwidth/l3gd20h_get_rate_bandwidth test.\n");
    
    /* rate 100Hz bandwidth 12.5Hz */
    res = l3gd20h_set_rate_bandwidth(&gs_handle, L3GD20H_LOW_ODR_0_ODR_100HZ_BW_0_12P5HZ);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: set rate bandwidth failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: set rate 100Hz bandwidth 12.5Hz.\n");
    res = l3gd20h_get_rate_bandwidth(&gs_handle, &rate_bandwidth);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: get rate bandwidth failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: check rate bandwidth %s.\n", rate_bandwidth == L3GD20H_LOW_ODR_0_ODR_100HZ_BW_0_12P5HZ ? "ok" : "error");
    
    /* rate 100Hz bandwidth 25Hz */
    res = l3gd20h_set_rate_bandwidth(&gs_handle, L3GD20H_LOW_ODR_0_ODR_100HZ_BW_1_25HZ);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: set rate bandwidth failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: set rate 100Hz bandwidth 25Hz.\n");
    res = l3gd20h_get_rate_bandwidth(&gs_handle, &rate_bandwidth);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: get rate bandwidth failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: check rate bandwidth %s.\n", rate_bandwidth == L3GD20H_LOW_ODR_0_ODR_100HZ_BW_1_25HZ ? "ok" : "error");
    
    /* rate 100Hz bandwidth 25Hz */
    res = l3gd20h_set_rate_bandwidth(&gs_handle, L3GD20H_LOW_ODR_0_ODR_100HZ_BW_2_25HZ);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: set rate bandwidth failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: set rate 100Hz bandwidth 25Hz.\n");
    res = l3gd20h_get_rate_bandwidth(&gs_handle, &rate_bandwidth);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: get rate bandwidth failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: check rate bandwidth %s.\n", rate_bandwidth == L3GD20H_LOW_ODR_0_ODR_100HZ_BW_2_25HZ ? "ok" : "error");
    
    /* rate 100Hz bandwidth 25Hz */
    res = l3gd20h_set_rate_bandwidth(&gs_handle, L3GD20H_LOW_ODR_0_ODR_100HZ_BW_3_25HZ);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: set rate bandwidth failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: set rate 100Hz bandwidth 25Hz.\n");
    res = l3gd20h_get_rate_bandwidth(&gs_handle, &rate_bandwidth);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: get rate bandwidth failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: check rate bandwidth %s.\n", rate_bandwidth == L3GD20H_LOW_ODR_0_ODR_100HZ_BW_3_25HZ ? "ok" : "error");
    
    /* rate 200Hz bandwidth NA */
    res = l3gd20h_set_rate_bandwidth(&gs_handle, L3GD20H_LOW_ODR_0_ODR_200HZ_BW_1_NA);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: set rate bandwidth failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: set rate 200Hz bandwidth NA.\n");
    res = l3gd20h_get_rate_bandwidth(&gs_handle, &rate_bandwidth);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: get rate bandwidth failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: check rate bandwidth %s.\n", rate_bandwidth == L3GD20H_LOW_ODR_0_ODR_200HZ_BW_1_NA ? "ok" : "error");

    /* rate 200Hz bandwidth NA */
    res = l3gd20h_set_rate_bandwidth(&gs_handle, L3GD20H_LOW_ODR_0_ODR_200HZ_BW_2_NA);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: set rate bandwidth failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: set rate 200Hz bandwidth NA.\n");
    res = l3gd20h_get_rate_bandwidth(&gs_handle, &rate_bandwidth);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: get rate bandwidth failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: check rate bandwidth %s.\n", rate_bandwidth == L3GD20H_LOW_ODR_0_ODR_200HZ_BW_2_NA ? "ok" : "error");
    
    /* rate 200Hz bandwidth 70Hz */
    res = l3gd20h_set_rate_bandwidth(&gs_handle, L3GD20H_LOW_ODR_0_ODR_200HZ_BW_3_70HZ);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: set rate bandwidth failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: set rate 200Hz bandwidth 70Hz.\n");
    res = l3gd20h_get_rate_bandwidth(&gs_handle, &rate_bandwidth);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: get rate bandwidth failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: check rate bandwidth %s.\n", rate_bandwidth == L3GD20H_LOW_ODR_0_ODR_200HZ_BW_3_70HZ ? "ok" : "error");
   
    /* rate 400Hz bandwidth 20Hz */
    res = l3gd20h_set_rate_bandwidth(&gs_handle, L3GD20H_LOW_ODR_0_ODR_400HZ_BW_0_20HZ);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: set rate bandwidth failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: set rate 400Hz bandwidth 20Hz.\n");
    res = l3gd20h_get_rate_bandwidth(&gs_handle, &rate_bandwidth);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: get rate bandwidth failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: check rate bandwidth %s.\n", rate_bandwidth == L3GD20H_LOW_ODR_0_ODR_400HZ_BW_0_20HZ ? "ok" : "error");
    
    /* rate 400Hz bandwidth 25Hz */
    res = l3gd20h_set_rate_bandwidth(&gs_handle, L3GD20H_LOW_ODR_0_ODR_400HZ_BW_1_25HZ);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: set rate bandwidth failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: set rate 400Hz bandwidth 25Hz.\n");
    res = l3gd20h_get_rate_bandwidth(&gs_handle, &rate_bandwidth);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: get rate bandwidth failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: check rate bandwidth %s.\n", rate_bandwidth == L3GD20H_LOW_ODR_0_ODR_400HZ_BW_1_25HZ ? "ok" : "error");
    
    /* rate 400Hz bandwidth 50Hz */
    res = l3gd20h_set_rate_bandwidth(&gs_handle, L3GD20H_LOW_ODR_0_ODR_400HZ_BW_2_50HZ);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: set rate bandwidth failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: set rate 400Hz bandwidth 50Hz.\n");
    res = l3gd20h_get_rate_bandwidth(&gs_handle, &rate_bandwidth);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: get rate bandwidth failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: check rate bandwidth %s.\n", rate_bandwidth == L3GD20H_LOW_ODR_0_ODR_400HZ_BW_2_50HZ ? "ok" : "error");
    
    /* rate 400Hz bandwidth 110Hz */
    res = l3gd20h_set_rate_bandwidth(&gs_handle, L3GD20H_LOW_ODR_0_ODR_400HZ_BW_3_110HZ);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: set rate bandwidth failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: set rate 400Hz bandwidth 110Hz.\n");
    res = l3gd20h_get_rate_bandwidth(&gs_handle, &rate_bandwidth);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: get rate bandwidth failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: check rate bandwidth %s.\n", rate_bandwidth == L3GD20H_LOW_ODR_0_ODR_400HZ_BW_3_110HZ ? "ok" : "error");
    
    /* rate 800Hz bandwidth 30Hz */
    res = l3gd20h_set_rate_bandwidth(&gs_handle, L3GD20H_LOW_ODR_0_ODR_800HZ_BW_0_30HZ);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: set rate bandwidth failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: set rate 800Hz bandwidth 30Hz.\n");
    res = l3gd20h_get_rate_bandwidth(&gs_handle, &rate_bandwidth);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: get rate bandwidth failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: check rate bandwidth %s.\n", rate_bandwidth == L3GD20H_LOW_ODR_0_ODR_800HZ_BW_0_30HZ ? "ok" : "error");
    
    /* rate 800Hz bandwidth 35Hz */
    res = l3gd20h_set_rate_bandwidth(&gs_handle, L3GD20H_LOW_ODR_0_ODR_800HZ_BW_1_35HZ);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: set rate bandwidth failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: set rate 800Hz bandwidth 35Hz.\n");
    res = l3gd20h_get_rate_bandwidth(&gs_handle, &rate_bandwidth);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: get rate bandwidth failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: check rate bandwidth %s.\n", rate_bandwidth == L3GD20H_LOW_ODR_0_ODR_800HZ_BW_1_35HZ ? "ok" : "error");
    
    /* rate 800Hz bandwidth NA */
    res = l3gd20h_set_rate_bandwidth(&gs_handle, L3GD20H_LOW_ODR_0_ODR_800HZ_BW_2_NA);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: set rate bandwidth failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: set rate 800Hz bandwidth NA.\n");
    res = l3gd20h_get_rate_bandwidth(&gs_handle, &rate_bandwidth);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: get rate bandwidth failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: check rate bandwidth %s.\n", rate_bandwidth == L3GD20H_LOW_ODR_0_ODR_800HZ_BW_2_NA ? "ok" : "error");

    /* rate 800Hz bandwidth 100Hz */
    res = l3gd20h_set_rate_bandwidth(&gs_handle, L3GD20H_LOW_ODR_0_ODR_800HZ_BW_3_100HZ);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: set rate bandwidth failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: set rate 800Hz bandwidth 100Hz.\n");
    res = l3gd20h_get_rate_bandwidth(&gs_handle, &rate_bandwidth);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: get rate bandwidth failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: check rate bandwidth %s.\n", rate_bandwidth == L3GD20H_LOW_ODR_0_ODR_800HZ_BW_3_100HZ ? "ok" : "error");
    
    /* low power rate 12.5Hz bandwidth NA */
    res = l3gd20h_set_rate_bandwidth(&gs_handle, L3GD20H_LOW_ODR_1_ODR_12P5HZ_BW_0_NA);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: set rate bandwidth failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: set low power rate 12.5Hz bandwidth NA.\n");
    res = l3gd20h_get_rate_bandwidth(&gs_handle, &rate_bandwidth);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: get rate bandwidth failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: check rate bandwidth %s.\n", rate_bandwidth == L3GD20H_LOW_ODR_1_ODR_12P5HZ_BW_0_NA ? "ok" : "error");
    
    /* low power rate 12.5Hz bandwidth NA */
    res = l3gd20h_set_rate_bandwidth(&gs_handle, L3GD20H_LOW_ODR_1_ODR_12P5HZ_BW_1_NA);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: set rate bandwidth failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: set low power rate 12.5Hz bandwidth NA.\n");
    res = l3gd20h_get_rate_bandwidth(&gs_handle, &rate_bandwidth);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: get rate bandwidth failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: check rate bandwidth %s.\n", rate_bandwidth == L3GD20H_LOW_ODR_1_ODR_12P5HZ_BW_1_NA ? "ok" : "error");
    
    /* low power rate 12.5Hz bandwidth NA */
    res = l3gd20h_set_rate_bandwidth(&gs_handle, L3GD20H_LOW_ODR_1_ODR_12P5HZ_BW_2_NA);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: set rate bandwidth failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: set low power rate 12.5Hz bandwidth NA.\n");
    res = l3gd20h_get_rate_bandwidth(&gs_handle, &rate_bandwidth);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: get rate bandwidth failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: check rate bandwidth %s.\n", rate_bandwidth == L3GD20H_LOW_ODR_1_ODR_12P5HZ_BW_2_NA ? "ok" : "error");
    
    /* low power rate 12.5Hz bandwidth NA */
    res = l3gd20h_set_rate_bandwidth(&gs_handle, L3GD20H_LOW_ODR_1_ODR_12P5HZ_BW_3_NA);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: set rate bandwidth failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: set low power rate 12.5Hz bandwidth NA.\n");
    res = l3gd20h_get_rate_bandwidth(&gs_handle, &rate_bandwidth);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: get rate bandwidth failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: check rate bandwidth %s.\n", rate_bandwidth == L3GD20H_LOW_ODR_1_ODR_12P5HZ_BW_3_NA ? "ok" : "error");
    
    /* low power rate 25Hz bandwidth NA */
    res = l3gd20h_set_rate_bandwidth(&gs_handle, L3GD20H_LOW_ODR_1_ODR_25HZ_BW_0_NA);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: set rate bandwidth failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: set low power rate 25Hz bandwidth NA.\n");
    res = l3gd20h_get_rate_bandwidth(&gs_handle, &rate_bandwidth);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: get rate bandwidth failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: check rate bandwidth %s.\n", rate_bandwidth == L3GD20H_LOW_ODR_1_ODR_25HZ_BW_0_NA ? "ok" : "error");
    
    /* low power rate 25Hz bandwidth NA */
    res = l3gd20h_set_rate_bandwidth(&gs_handle, L3GD20H_LOW_ODR_1_ODR_25HZ_BW_1_NA);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: set rate bandwidth failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: set low power rate 25Hz bandwidth NA.\n");
    res = l3gd20h_get_rate_bandwidth(&gs_handle, &rate_bandwidth);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: get rate bandwidth failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: check rate bandwidth %s.\n", rate_bandwidth == L3GD20H_LOW_ODR_1_ODR_25HZ_BW_1_NA ? "ok" : "error");
    
    /* low power rate 25Hz bandwidth NA */
    res = l3gd20h_set_rate_bandwidth(&gs_handle, L3GD20H_LOW_ODR_1_ODR_25HZ_BW_2_NA);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: set rate bandwidth failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: set low power rate 25Hz bandwidth NA.\n");
    res = l3gd20h_get_rate_bandwidth(&gs_handle, &rate_bandwidth);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: get rate bandwidth failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: check rate bandwidth %s.\n", rate_bandwidth == L3GD20H_LOW_ODR_1_ODR_25HZ_BW_2_NA ? "ok" : "error");
    
    /* low power rate 25Hz bandwidth NA */
    res = l3gd20h_set_rate_bandwidth(&gs_handle, L3GD20H_LOW_ODR_1_ODR_25HZ_BW_3_NA);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: set rate bandwidth failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: set low power rate 25Hz bandwidth NA.\n");
    res = l3gd20h_get_rate_bandwidth(&gs_handle, &rate_bandwidth);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: get rate bandwidth failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: check rate bandwidth %s.\n", rate_bandwidth == L3GD20H_LOW_ODR_1_ODR_25HZ_BW_3_NA ? "ok" : "error");
    
    /* low power rate 50Hz bandwidth 16.6Hz */
    res = l3gd20h_set_rate_bandwidth(&gs_handle, L3GD20H_LOW_ODR_1_ODR_50HZ_BW_0_16P6HZ);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: set rate bandwidth failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: set low power rate 50Hz bandwidth 16.6Hz.\n");
    res = l3gd20h_get_rate_bandwidth(&gs_handle, &rate_bandwidth);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: get rate bandwidth failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: check rate bandwidth %s.\n", rate_bandwidth == L3GD20H_LOW_ODR_1_ODR_50HZ_BW_0_16P6HZ ? "ok" : "error");
    
    /* low power rate 50Hz bandwidth 16.6Hz */
    res = l3gd20h_set_rate_bandwidth(&gs_handle, L3GD20H_LOW_ODR_1_ODR_50HZ_BW_1_16P6HZ);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: set rate bandwidth failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: set low power rate 50Hz bandwidth 16.6Hz.\n");
    res = l3gd20h_get_rate_bandwidth(&gs_handle, &rate_bandwidth);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: get rate bandwidth failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: check rate bandwidth %s.\n", rate_bandwidth == L3GD20H_LOW_ODR_1_ODR_50HZ_BW_1_16P6HZ ? "ok" : "error");
    
    /* low power rate 50Hz bandwidth 16.6Hz */
    res = l3gd20h_set_rate_bandwidth(&gs_handle, L3GD20H_LOW_ODR_1_ODR_50HZ_BW_2_16P6HZ);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: set rate bandwidth failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: set low power rate 50Hz bandwidth 16.6Hz.\n");
    res = l3gd20h_get_rate_bandwidth(&gs_handle, &rate_bandwidth);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: get rate bandwidth failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: check rate bandwidth %s.\n", rate_bandwidth == L3GD20H_LOW_ODR_1_ODR_50HZ_BW_2_16P6HZ ? "ok" : "error");
    
    /* low power rate 50Hz bandwidth 16.6Hz */
    res = l3gd20h_set_rate_bandwidth(&gs_handle, L3GD20H_LOW_ODR_1_ODR_50HZ_BW_3_16P6HZ);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: set rate bandwidth failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: set low power rate 50Hz bandwidth 16.6Hz.\n");
    res = l3gd20h_get_rate_bandwidth(&gs_handle, &rate_bandwidth);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: get rate bandwidth failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: check rate bandwidth %s.\n", rate_bandwidth == L3GD20H_LOW_ODR_1_ODR_50HZ_BW_3_16P6HZ ? "ok" : "error");
    
    /* l3gd20h_set_edge_trigger/l3gd20h_get_edge_trigger test */
    l3gd20h_interface_debug_print("l3gd20h: l3gd20h_set_edge_trigger/l3gd20h_get_edge_trigger test.\n");
    
    /* enable */
    res = l3gd20h_set_edge_trigger(&gs_handle, L3GD20H_BOOL_TRUE);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: set edge trigger failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: enable edge trigger.\n");
    res = l3gd20h_get_edge_trigger(&gs_handle, &enable);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: get edge trigger failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: check edge trigger %s.\n", enable == L3GD20H_BOOL_TRUE ? "ok" : "error");
    
    /* disable */
    res = l3gd20h_set_edge_trigger(&gs_handle, L3GD20H_BOOL_FALSE);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: set edge trigger failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: disable edge trigger.\n");
    res = l3gd20h_get_edge_trigger(&gs_handle, &enable);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: get edge trigger failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: check edge trigger %s.\n", enable == L3GD20H_BOOL_FALSE ? "ok" : "error");
    
    /* l3gd20h_set_level_trigger/l3gd20h_get_level_trigger test */
    l3gd20h_interface_debug_print("l3gd20h: l3gd20h_set_level_trigger/l3gd20h_get_level_trigger test.\n");

    /* enable */
    res = l3gd20h_set_level_trigger(&gs_handle, L3GD20H_BOOL_TRUE);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: set level trigger failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: enable level trigger.\n");
    res = l3gd20h_get_level_trigger(&gs_handle, &enable);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: get level trigger failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: check level trigger %s.\n", enable == L3GD20H_BOOL_TRUE ? "ok" : "error");
    
    /* disable */
    res = l3gd20h_set_level_trigger(&gs_handle, L3GD20H_BOOL_FALSE);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: set level trigger failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: disable level trigger.\n");
    res = l3gd20h_get_level_trigger(&gs_handle, &enable);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: get level trigger failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: check level trigger %s.\n", enable == L3GD20H_BOOL_FALSE ? "ok" : "error");
    
    /* l3gd20h_set_high_pass_filter_mode/l3gd20h_get_high_pass_filter_mode test */
    l3gd20h_interface_debug_print("l3gd20h: l3gd20h_set_high_pass_filter_mode/l3gd20h_get_high_pass_filter_mode test.\n");
    
    /* normal reset mode*/
    res = l3gd20h_set_high_pass_filter_mode(&gs_handle, L3GD20H_HIGH_PASS_FILTER_MODE_NORMAL_RESET);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: set high pass filter mode failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: set high pass filter normal reset mode.\n");
    res = l3gd20h_get_high_pass_filter_mode(&gs_handle, &high_mode);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: get high pass filter mode failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: check high pass filter mode %s.\n", high_mode == L3GD20H_HIGH_PASS_FILTER_MODE_NORMAL_RESET ? "ok" : "error");
    
    /* reference signal mode */
    res = l3gd20h_set_high_pass_filter_mode(&gs_handle, L3GD20H_HIGH_PASS_FILTER_MODE_REFERENCE_SIGNAL);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: set high pass filter mode failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: set high pass filter reference signal mode.\n");
    res = l3gd20h_get_high_pass_filter_mode(&gs_handle, &high_mode);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: get high pass filter mode failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: check high pass filter mode %s.\n", high_mode == L3GD20H_HIGH_PASS_FILTER_MODE_REFERENCE_SIGNAL ? "ok" : "error");
    
    /* normal mode */
    res = l3gd20h_set_high_pass_filter_mode(&gs_handle, L3GD20H_HIGH_PASS_FILTER_MODE_NORMAL);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: set high pass filter mode failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: set high pass filter normal mode.\n");
    res = l3gd20h_get_high_pass_filter_mode(&gs_handle, &high_mode);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: get high pass filter mode failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: check high pass filter mode %s.\n", high_mode == L3GD20H_HIGH_PASS_FILTER_MODE_NORMAL ? "ok" : "error");
    
    /* auto reset on int mode */
    res = l3gd20h_set_high_pass_filter_mode(&gs_handle, L3GD20H_HIGH_PASS_FILTER_MODE_AUTORESET_ON_INT);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: set high pass filter mode failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: set high pass filter auto reset on int mode.\n");
    res = l3gd20h_get_high_pass_filter_mode(&gs_handle, &high_mode);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: get high pass filter mode failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: check high pass filter mode %s.\n", high_mode == L3GD20H_HIGH_PASS_FILTER_MODE_AUTORESET_ON_INT ? "ok" : "error");
    
    /* l3gd20h_set_high_pass_filter_cut_off_frequency/l3gd20h_get_high_pass_filter_cut_off_frequency test */
    l3gd20h_interface_debug_print("l3gd20h: l3gd20h_set_high_pass_filter_cut_off_frequency/l3gd20h_get_high_pass_filter_cut_off_frequency test.\n");
    
    /* cut-off frequency 0 */
    res = l3gd20h_set_high_pass_filter_cut_off_frequency(&gs_handle, L3GD20H_HIGH_PASS_FILTER_CUT_OFF_FREQUENCY_0);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: set high pass filter cut off frequency failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: set high pass filter cut off frequency 0.\n");
    res = l3gd20h_get_high_pass_filter_cut_off_frequency(&gs_handle, &frequency);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: get high pass filter cut off frequency failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: check high pass filter cut off frequency %s.\n", frequency == L3GD20H_HIGH_PASS_FILTER_CUT_OFF_FREQUENCY_0 ? "ok" : "error");
    
    /* cut-off frequency 1 */
    res = l3gd20h_set_high_pass_filter_cut_off_frequency(&gs_handle, L3GD20H_HIGH_PASS_FILTER_CUT_OFF_FREQUENCY_1);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: set high pass filter cut off frequency failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: set high pass filter cut off frequency 1.\n");
    res = l3gd20h_get_high_pass_filter_cut_off_frequency(&gs_handle, &frequency);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: get high pass filter cut off frequency failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: check high pass filter cut off frequency %s.\n", frequency == L3GD20H_HIGH_PASS_FILTER_CUT_OFF_FREQUENCY_1 ? "ok" : "error");
    
    /* cut-off frequency 2 */
    res = l3gd20h_set_high_pass_filter_cut_off_frequency(&gs_handle, L3GD20H_HIGH_PASS_FILTER_CUT_OFF_FREQUENCY_2);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: set high pass filter cut off frequency failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: set high pass filter cut off frequency 2.\n");
    res = l3gd20h_get_high_pass_filter_cut_off_frequency(&gs_handle, &frequency);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: get high pass filter cut off frequency failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: check high pass filter cut off frequency %s.\n", frequency == L3GD20H_HIGH_PASS_FILTER_CUT_OFF_FREQUENCY_2 ? "ok" : "error");
    
    /* cut-off frequency 3 */
    res = l3gd20h_set_high_pass_filter_cut_off_frequency(&gs_handle, L3GD20H_HIGH_PASS_FILTER_CUT_OFF_FREQUENCY_3);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: set high pass filter cut off frequency failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: set high pass filter cut off frequency 3.\n");
    res = l3gd20h_get_high_pass_filter_cut_off_frequency(&gs_handle, &frequency);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: get high pass filter cut off frequency failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: check high pass filter cut off frequency %s.\n", frequency == L3GD20H_HIGH_PASS_FILTER_CUT_OFF_FREQUENCY_3 ? "ok" : "error");
    
    /* cut-off frequency 4 */
    res = l3gd20h_set_high_pass_filter_cut_off_frequency(&gs_handle, L3GD20H_HIGH_PASS_FILTER_CUT_OFF_FREQUENCY_4);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: set high pass filter cut off frequency failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: set high pass filter cut off frequency 4.\n");
    res = l3gd20h_get_high_pass_filter_cut_off_frequency(&gs_handle, &frequency);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: get high pass filter cut off frequency failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: check high pass filter cut off frequency %s.\n", frequency == L3GD20H_HIGH_PASS_FILTER_CUT_OFF_FREQUENCY_4 ? "ok" : "error");
    
    /* cut-off frequency 5 */
    res = l3gd20h_set_high_pass_filter_cut_off_frequency(&gs_handle, L3GD20H_HIGH_PASS_FILTER_CUT_OFF_FREQUENCY_5);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: set high pass filter cut off frequency failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: set high pass filter cut off frequency 5.\n");
    res = l3gd20h_get_high_pass_filter_cut_off_frequency(&gs_handle, &frequency);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: get high pass filter cut off frequency failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: check high pass filter cut off frequency %s.\n", frequency == L3GD20H_HIGH_PASS_FILTER_CUT_OFF_FREQUENCY_5 ? "ok" : "error");
    
    /* cut-off frequency 6 */
    res = l3gd20h_set_high_pass_filter_cut_off_frequency(&gs_handle, L3GD20H_HIGH_PASS_FILTER_CUT_OFF_FREQUENCY_6);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: set high pass filter cut off frequency failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: set high pass filter cut off frequency 6.\n");
    res = l3gd20h_get_high_pass_filter_cut_off_frequency(&gs_handle, &frequency);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: get high pass filter cut off frequency failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: check high pass filter cut off frequency %s.\n", frequency == L3GD20H_HIGH_PASS_FILTER_CUT_OFF_FREQUENCY_6 ? "ok" : "error");
    
    /* cut-off frequency 7 */
    res = l3gd20h_set_high_pass_filter_cut_off_frequency(&gs_handle, L3GD20H_HIGH_PASS_FILTER_CUT_OFF_FREQUENCY_7);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: set high pass filter cut off frequency failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: set high pass filter cut off frequency 7.\n");
    res = l3gd20h_get_high_pass_filter_cut_off_frequency(&gs_handle, &frequency);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: get high pass filter cut off frequency failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: check high pass filter cut off frequency %s.\n", frequency == L3GD20H_HIGH_PASS_FILTER_CUT_OFF_FREQUENCY_7 ? "ok" : "error");
    
    /* cut-off frequency 8 */
    res = l3gd20h_set_high_pass_filter_cut_off_frequency(&gs_handle, L3GD20H_HIGH_PASS_FILTER_CUT_OFF_FREQUENCY_8);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: set high pass filter cut off frequency failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: set high pass filter cut off frequency 8.\n");
    res = l3gd20h_get_high_pass_filter_cut_off_frequency(&gs_handle, &frequency);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: get high pass filter cut off frequency failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: check high pass filter cut off frequency %s.\n", frequency == L3GD20H_HIGH_PASS_FILTER_CUT_OFF_FREQUENCY_8 ? "ok" : "error");
    
    /* cut-off frequency 9 */
    res = l3gd20h_set_high_pass_filter_cut_off_frequency(&gs_handle, L3GD20H_HIGH_PASS_FILTER_CUT_OFF_FREQUENCY_9);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: set high pass filter cut off frequency failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: set high pass filter cut off frequency 9.\n");
    res = l3gd20h_get_high_pass_filter_cut_off_frequency(&gs_handle, &frequency);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: get high pass filter cut off frequency failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: check high pass filter cut off frequency %s.\n", frequency == L3GD20H_HIGH_PASS_FILTER_CUT_OFF_FREQUENCY_9 ? "ok" : "error");
    
    /* l3gd20h_set_block_data_update/l3gd20h_get_block_data_update test */
    l3gd20h_interface_debug_print("l3gd20h: l3gd20h_set_block_data_update/l3gd20h_get_block_data_update test.\n");
    
    /* enable */
    res = l3gd20h_set_block_data_update(&gs_handle, L3GD20H_BOOL_TRUE);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: set block data update failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: enable block data update.\n");
    res = l3gd20h_get_block_data_update(&gs_handle, &enable);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: get block data update failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: check block data update %s.\n", enable == L3GD20H_BOOL_TRUE ? "ok" : "error");
    
    /* disable */
    res = l3gd20h_set_block_data_update(&gs_handle, L3GD20H_BOOL_FALSE);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: set block data update failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: disable block data update.\n");
    res = l3gd20h_get_block_data_update(&gs_handle, &enable);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: get block data update failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: check block data update %s.\n", enable == L3GD20H_BOOL_FALSE ? "ok" : "error");
    
    /* l3gd20h_set_data_format/l3gd20h_get_data_format test */
    l3gd20h_interface_debug_print("l3gd20h: l3gd20h_set_data_format/l3gd20h_get_data_format test.\n");
    
    /* big endian */
    res = l3gd20h_set_data_format(&gs_handle, L3GD20H_DATA_FORMAT_BIG_ENDIAN);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: set data format failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: set data format big endian.\n");
    res = l3gd20h_get_data_format(&gs_handle, &data_format);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: get data format failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: check data format %s.\n", data_format == L3GD20H_DATA_FORMAT_BIG_ENDIAN ? "ok" : "error");
    
    /* little endian */
    res = l3gd20h_set_data_format(&gs_handle, L3GD20H_DATA_FORMAT_LITTLE_ENDIAN);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: set data format failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: set data format little endian.\n");
    res = l3gd20h_get_data_format(&gs_handle, &data_format);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: get data format failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: check data format %s.\n", data_format == L3GD20H_DATA_FORMAT_LITTLE_ENDIAN ? "ok" : "error");
    
    /* l3gd20h_set_full_scale/l3gd20h_get_full_scale test */
    l3gd20h_interface_debug_print("l3gd20h: l3gd20h_set_full_scale/l3gd20h_get_full_scale test.\n");
    
    /* ±245 dps */
    res = l3gd20h_set_full_scale(&gs_handle, L3GD20H_FULL_SCALE_245_DPS);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: set full scale failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: set full scale ± 245 dps.\n");
    res = l3gd20h_get_full_scale(&gs_handle, &full_scale);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: get full scale failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: check full scale %s.\n", full_scale == L3GD20H_FULL_SCALE_245_DPS ? "ok" : "error");
    
    /* ±500 dps */
    res = l3gd20h_set_full_scale(&gs_handle, L3GD20H_FULL_SCALE_500_DPS);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: set full scale failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: set full scale ± 500 dps.\n");
    res = l3gd20h_get_full_scale(&gs_handle, &full_scale);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: get full scale failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: check full scale %s.\n", full_scale == L3GD20H_FULL_SCALE_500_DPS ? "ok" : "error");
    
    /* ±2000 dps */
    res = l3gd20h_set_full_scale(&gs_handle, L3GD20H_FULL_SCALE_2000_DPS);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: set full scale failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: set full scale ± 2000 dps.\n");
    res = l3gd20h_get_full_scale(&gs_handle, &full_scale);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: get full scale failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: check full scale %s.\n", full_scale == L3GD20H_FULL_SCALE_2000_DPS ? "ok" : "error");
    
    
    /* l3gd20h_set_level_sensitive_latched/l3gd20h_get_level_sensitive_latched test */
    l3gd20h_interface_debug_print("l3gd20h: l3gd20h_set_level_sensitive_latched/l3gd20h_get_level_sensitive_latched test.\n");
    
    /* disable */
    res = l3gd20h_set_level_sensitive_latched(&gs_handle, L3GD20H_BOOL_FALSE);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: set level sensitive latched failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: disable level sensitive latched.\n");
    res = l3gd20h_get_level_sensitive_latched(&gs_handle, &enable);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: get level sensitive latched failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: check level sensitive latched %s.\n", enable == L3GD20H_BOOL_FALSE ? "ok" : "error");
    
    /* enable */
    res = l3gd20h_set_level_sensitive_latched(&gs_handle, L3GD20H_BOOL_TRUE);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: set level sensitive latched failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: enable level sensitive latched.\n");
    res = l3gd20h_get_level_sensitive_latched(&gs_handle, &enable);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: get level sensitive latched failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: check level sensitive latched %s.\n", enable == L3GD20H_BOOL_TRUE ? "ok" : "error");
    
    /* l3gd20h_set_self_test/l3gd20h_get_self_test test */
    l3gd20h_interface_debug_print("l3gd20h: l3gd20h_set_self_test/l3gd20h_get_self_test test.\n");
    
    /* self test 0 */
    res = l3gd20h_set_self_test(&gs_handle, L3GD20H_SELF_TEST_0);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: set self test failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: set self test 0.\n");
    res = l3gd20h_get_self_test(&gs_handle, &self_test);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: get self test failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: check self test %s.\n", self_test == L3GD20H_SELF_TEST_0 ? "ok" : "error");
    
    /* self test 1 */
    res = l3gd20h_set_self_test(&gs_handle, L3GD20H_SELF_TEST_1);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: set self test failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: set self test 1.\n");
    res = l3gd20h_get_self_test(&gs_handle, &self_test);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: get self test failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: check self test %s.\n", self_test == L3GD20H_SELF_TEST_1 ? "ok" : "error");
    
    /* self test normal */
    res = l3gd20h_set_self_test(&gs_handle, L3GD20H_SELF_TEST_NORMAL);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: set self test failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: set self test normal.\n");
    res = l3gd20h_get_self_test(&gs_handle, &self_test);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: get self test failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: check self test %s.\n", self_test == L3GD20H_SELF_TEST_NORMAL ? "ok" : "error");
    
    /* if iic interface */
    if (interface == L3GD20H_INTERFACE_IIC)
    {
        /* l3gd20h_set_spi_wire/l3gd20h_get_spi_wire test */
        l3gd20h_interface_debug_print("l3gd20h: l3gd20h_set_spi_wire/l3gd20h_get_spi_wire test.\n");
        
        /* spi wire 3 */
        res = l3gd20h_set_spi_wire(&gs_handle, L3GD20H_SPI_WIRE_3);
        if (res != 0)
        {
            l3gd20h_interface_debug_print("l3gd20h: set spi wire failed.\n");
            (void)l3gd20h_deinit(&gs_handle);
            
            return 1;
        }
        l3gd20h_interface_debug_print("l3gd20h: set spi wire 3.\n");
        res = l3gd20h_get_spi_wire(&gs_handle, &spi_wire);
        if (res != 0)
        {
            l3gd20h_interface_debug_print("l3gd20h: get spi wire failed.\n");
            (void)l3gd20h_deinit(&gs_handle);
            
            return 1;
        }
        l3gd20h_interface_debug_print("l3gd20h: check spi wire %s.\n", spi_wire == L3GD20H_SPI_WIRE_3 ? "ok" : "error");
        
        /* spi wire 4 */
        res = l3gd20h_set_spi_wire(&gs_handle, L3GD20H_SPI_WIRE_4);
        if (res != 0)
        {
            l3gd20h_interface_debug_print("l3gd20h: set spi wire failed.\n");
            (void)l3gd20h_deinit(&gs_handle);
            
            return 1;
        }
        l3gd20h_interface_debug_print("l3gd20h: set spi wire 4.\n");
        res = l3gd20h_get_spi_wire(&gs_handle, &spi_wire);
        if (res != 0)
        {
            l3gd20h_interface_debug_print("l3gd20h: get spi wire failed.\n");
            (void)l3gd20h_deinit(&gs_handle);
            
            return 1;
        }
        l3gd20h_interface_debug_print("l3gd20h: check spi wire %s.\n", spi_wire == L3GD20H_SPI_WIRE_4 ? "ok" : "error");
    }
    
    /* l3gd20h_set_high_pass_filter/l3gd20h_get_high_pass_filter test */
    l3gd20h_interface_debug_print("l3gd20h: l3gd20h_set_high_pass_filter/l3gd20h_get_high_pass_filter test.\n");
    
    /* disable */
    res = l3gd20h_set_high_pass_filter(&gs_handle, L3GD20H_BOOL_FALSE);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: set high pass filter failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: disable high pass filter.\n");
    res = l3gd20h_get_high_pass_filter(&gs_handle, &enable);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: get high pass filter failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: check high pass filter %s.\n", enable == L3GD20H_BOOL_FALSE ? "ok" : "error");
    
    /* enable */
    res = l3gd20h_set_high_pass_filter(&gs_handle, L3GD20H_BOOL_TRUE);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: set high pass filter failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: enable high pass filter.\n");
    res = l3gd20h_get_high_pass_filter(&gs_handle, &enable);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: get high pass filter failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: check high pass filter %s.\n", enable == L3GD20H_BOOL_TRUE ? "ok" : "error");
 
    /* l3gd20h_set_out_selection/l3gd20h_get_out_selection test */
    l3gd20h_interface_debug_print("l3gd20h: l3gd20h_set_out_selection/l3gd20h_get_out_selection test.\n");
    
    /* LPF1 */
    res = l3gd20h_set_out_selection(&gs_handle, L3GD20H_SELECTION_LPF1);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: set out selection failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: set out selection LPF1.\n");
    res = l3gd20h_get_out_selection(&gs_handle, &selection);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: get out selection failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: check out selection %s.\n", selection == L3GD20H_SELECTION_LPF1 ? "ok" : "error");
    
    /* LPF1 HPF */
    res = l3gd20h_set_out_selection(&gs_handle, L3GD20H_SELECTION_LPF1_HPF);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: set out selection failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: set out selection LPF1-HPF.\n");
    res = l3gd20h_get_out_selection(&gs_handle, &selection);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: get out selection failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: check out selection %s.\n", selection == L3GD20H_SELECTION_LPF1_HPF ? "ok" : "error");
    
    /* LPF1 HPF LPF2 */
    res = l3gd20h_set_out_selection(&gs_handle, L3GD20H_SELECTION_LPF1_HPF_LPF2);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: set out selection failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: set out selection LPF1-HPF-LPF2.\n");
    res = l3gd20h_get_out_selection(&gs_handle, &selection);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: get out selection failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: check out selection %s.\n", selection == L3GD20H_SELECTION_LPF1_HPF_LPF2 ? "ok" : "error");
    
    /* l3gd20h_set_high_pass_filter_reference/l3gd20h_get_high_pass_filter_reference test */
    l3gd20h_interface_debug_print("l3gd20h: l3gd20h_set_high_pass_filter_reference/l3gd20h_get_high_pass_filter_reference test.\n");
    
    /* set high pass filter reference */
    value = rand() % 256;
    res = l3gd20h_set_high_pass_filter_reference(&gs_handle, value);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: set high pass filter reference failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: set high pass filter reference 0x%02X.\n", value);
    res = l3gd20h_get_high_pass_filter_reference(&gs_handle, (uint8_t *)&value_check);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: get high pass filter reference failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: check high pass filter reference %s.\n", value_check == value ? "ok" : "error");
    
    /* if spi interface */
    if (interface == L3GD20H_INTERFACE_SPI)
    {
        /* l3gd20h_set_iic/l3gd20h_get_iic test */
        l3gd20h_interface_debug_print("l3gd20h: l3gd20h_set_iic/l3gd20h_get_iic test.\n");
        
        /* disable */
        res  = l3gd20h_set_iic(&gs_handle, L3GD20H_BOOL_FALSE);
        if (res != 0)
        {
            l3gd20h_interface_debug_print("l3gd20h: set iic failed.\n");
            (void)l3gd20h_deinit(&gs_handle);
            
            return 1;
        }
        l3gd20h_interface_debug_print("l3gd20h: disable iic.\n");
        res = l3gd20h_get_iic(&gs_handle, &enable);
        if (res != 0)
        {
            l3gd20h_interface_debug_print("l3gd20h: get iic failed.\n");
            (void)l3gd20h_deinit(&gs_handle);
            
            return 1;
        }
        l3gd20h_interface_debug_print("l3gd20h: check iic %s.\n", enable == L3GD20H_BOOL_FALSE ? "ok" : "error");
        
        /* enable */
        res  = l3gd20h_set_iic(&gs_handle, L3GD20H_BOOL_TRUE);
        if (res != 0)
        {
            l3gd20h_interface_debug_print("l3gd20h: set iic failed.\n");
            (void)l3gd20h_deinit(&gs_handle);
            
            return 1;
        }
        l3gd20h_interface_debug_print("l3gd20h: enable iic.\n");
        res = l3gd20h_get_iic(&gs_handle, &enable);
        if (res != 0)
        {
            l3gd20h_interface_debug_print("l3gd20h: get iic failed.\n");
            (void)l3gd20h_deinit(&gs_handle);
            
            return 1;
        }
        l3gd20h_interface_debug_print("l3gd20h: check iic %s.\n", enable == L3GD20H_BOOL_TRUE ? "ok" : "error");
    }
    
    /* l3gd20h_set_interrupt1/l3gd20h_get_interrupt1 test */
    l3gd20h_interface_debug_print("l3gd20h: l3gd20h_set_interrupt1/l3gd20h_get_interrupt1 test.\n");

    /* enable */
    res = l3gd20h_set_interrupt1(&gs_handle, L3GD20H_BOOL_TRUE);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: set interrupt1 failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: enable interrupt1.\n");
    res = l3gd20h_get_interrupt1(&gs_handle, &enable);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: get interrupt1 failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: check interrupt1 %s.\n", enable == L3GD20H_BOOL_TRUE ? "ok" : "error");
    
    /* disable */
    res = l3gd20h_set_interrupt1(&gs_handle, L3GD20H_BOOL_FALSE);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: set interrupt1 failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: disable interrupt1.\n");
    res = l3gd20h_get_interrupt1(&gs_handle, &enable);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: get interrupt1 failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: check interrupt1 %s.\n", enable == L3GD20H_BOOL_FALSE ? "ok" : "error");
    
    /* l3gd20h_set_boot_on_interrupt1/l3gd20h_get_boot_on_interrupt1 test */
    l3gd20h_interface_debug_print("l3gd20h: l3gd20h_set_boot_on_interrupt1/l3gd20h_get_boot_on_interrupt1 test.\n");
    
    /* enable */
    res = l3gd20h_set_boot_on_interrupt1(&gs_handle, L3GD20H_BOOL_TRUE);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: set boot on interrupt1 failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: enable boot on interrupt1.\n");
    res = l3gd20h_get_boot_on_interrupt1(&gs_handle, &enable);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: get boot on interrupt1 failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: check boot on interrupt1 %s.\n", enable == L3GD20H_BOOL_TRUE ? "ok" : "error");
    
    /* disable */
    res = l3gd20h_set_boot_on_interrupt1(&gs_handle, L3GD20H_BOOL_FALSE);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: set boot on interrupt1 failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: disable boot on interrupt1.\n");
    res = l3gd20h_get_boot_on_interrupt1(&gs_handle, &enable);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: get boot on interrupt1 failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: check boot on interrupt1 %s.\n", enable == L3GD20H_BOOL_FALSE ? "ok" : "error");
    
    /* l3gd20h_set_interrupt_active_level/l3gd20h_get_interrupt_active_level test */
    l3gd20h_interface_debug_print("l3gd20h: l3gd20h_set_interrupt_active_level/l3gd20h_get_interrupt_active_level test.\n");
    
    /* high */
    res = l3gd20h_set_interrupt_active_level(&gs_handle, L3GD20H_INTERRUPT_ACTIVE_LEVEL_HIGH);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: set interrupt active level failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: set interrupt active level high.\n");
    res = l3gd20h_get_interrupt_active_level(&gs_handle, &level);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: get interrupt active level failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: check interrupt active level %s.\n", level == L3GD20H_INTERRUPT_ACTIVE_LEVEL_HIGH ? "ok" : "error");
    
    /* low */
    res = l3gd20h_set_interrupt_active_level(&gs_handle, L3GD20H_INTERRUPT_ACTIVE_LEVEL_LOW);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: set interrupt active level failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: set interrupt active level low.\n");
    res = l3gd20h_get_interrupt_active_level(&gs_handle, &level);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: get interrupt active level failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: check interrupt active level %s.\n", level == L3GD20H_INTERRUPT_ACTIVE_LEVEL_LOW ? "ok" : "error");
    
    /* l3gd20h_set_interrupt_pin_type/l3gd20h_get_interrupt_pin_type test */
    l3gd20h_interface_debug_print("l3gd20h: l3gd20h_set_interrupt_pin_type/l3gd20h_get_interrupt_pin_type test.\n");
    
    /* push-pull */
    res = l3gd20h_set_interrupt_pin_type(&gs_handle, L3GD20H_PIN_PUSH_PULL);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: set interrupt pin type failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: set push pull pin type.\n");
    res = l3gd20h_get_interrupt_pin_type(&gs_handle, &pin_type);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: get interrupt pin type failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: check interrupt pin type %s.\n", pin_type == L3GD20H_PIN_PUSH_PULL ? "ok" : "error");
    
    /* open drain */
    res = l3gd20h_set_interrupt_pin_type(&gs_handle, L3GD20H_PIN_OPEN_DRAIN);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: set interrupt pin type failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: set open drain pin type.\n");
    res = l3gd20h_get_interrupt_pin_type(&gs_handle, &pin_type);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: get interrupt pin type failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: check interrupt pin type %s.\n", pin_type == L3GD20H_PIN_OPEN_DRAIN ? "ok" : "error");
    
    /* l3gd20h_set_data_ready_on_interrupt2/l3gd20h_get_data_ready_on_interrupt2 test */
    l3gd20h_interface_debug_print("l3gd20h: l3gd20h_set_data_ready_on_interrupt2/l3gd20h_get_data_ready_on_interrupt2 test.\n");
    
    /* disable */
    res = l3gd20h_set_data_ready_on_interrupt2(&gs_handle, L3GD20H_BOOL_FALSE);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: set data ready on interrupt2 failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: disable data ready on interrupt2.\n");
    res = l3gd20h_get_data_ready_on_interrupt2(&gs_handle, &enable);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: get data ready on interrupt2 failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: check data ready on interrupt2 %s.\n", enable == L3GD20H_BOOL_FALSE ? "ok" : "error");
    
    /* enable */
    res = l3gd20h_set_data_ready_on_interrupt2(&gs_handle, L3GD20H_BOOL_TRUE);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: set data ready on interrupt2 failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: enable data ready on interrupt2.\n");
    res = l3gd20h_get_data_ready_on_interrupt2(&gs_handle, &enable);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: get data ready on interrupt2 failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: check data ready on interrupt2 %s.\n", enable == L3GD20H_BOOL_TRUE ? "ok" : "error");
    
    /* l3gd20h_set_fifo_threshold_on_interrupt2/l3gd20h_get_fifo_threshold_on_interrupt2 test */
    l3gd20h_interface_debug_print("l3gd20h: l3gd20h_set_fifo_threshold_on_interrupt2/l3gd20h_get_fifo_threshold_on_interrupt2 test.\n");
    
    /* disable */
    res = l3gd20h_set_fifo_threshold_on_interrupt2(&gs_handle, L3GD20H_BOOL_FALSE);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: set fifo threshold on interrupt2 failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: disable fifo threshold on interrupt2.\n");
    res = l3gd20h_get_fifo_threshold_on_interrupt2(&gs_handle, &enable);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: get fifo threshold on interrupt2 failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: check fifo threshold on interrupt2 %s.\n", enable == L3GD20H_BOOL_FALSE ? "ok" : "error");
    
    /* enable */
    res = l3gd20h_set_fifo_threshold_on_interrupt2(&gs_handle, L3GD20H_BOOL_TRUE);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: set fifo threshold on interrupt2 failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: enable fifo threshold on interrupt2.\n");
    res = l3gd20h_get_fifo_threshold_on_interrupt2(&gs_handle, &enable);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: get fifo threshold on interrupt2 failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: check fifo threshold on interrupt2 %s.\n", enable == L3GD20H_BOOL_TRUE ? "ok" : "error");
    
    /* l3gd20h_set_fifo_threshold_on_interrupt2/l3gd20h_get_fifo_threshold_on_interrupt2 test */
    l3gd20h_interface_debug_print("l3gd20h: l3gd20h_set_fifo_overrun_on_interrupt2/l3gd20h_get_fifo_overrun_on_interrupt2 test.\n");
    
    /* enable */
    res = l3gd20h_set_fifo_overrun_on_interrupt2(&gs_handle, L3GD20H_BOOL_TRUE);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: set fifo overrun on interrupt2 failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: enable fifo overrun on interrupt2.\n");
    res = l3gd20h_get_fifo_overrun_on_interrupt2(&gs_handle, &enable);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: get fifo overrun on interrupt2 failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: check fifo overrun on interrupt2 %s.\n", enable == L3GD20H_BOOL_TRUE ? "ok" : "error");
    
    /* disable */
    res = l3gd20h_set_fifo_overrun_on_interrupt2(&gs_handle, L3GD20H_BOOL_FALSE);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: set fifo overrun on interrupt2 failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: disable fifo overrun on interrupt2.\n");
    res = l3gd20h_get_fifo_overrun_on_interrupt2(&gs_handle, &enable);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: get fifo overrun on interrupt2 failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: check fifo overrun on interrupt2 %s.\n", enable == L3GD20H_BOOL_FALSE ? "ok" : "error");
    
    /* l3gd20h_set_fifo_empty_on_interrupt2/l3gd20h_get_fifo_empty_on_interrupt2 test */
    l3gd20h_interface_debug_print("l3gd20h: l3gd20h_set_fifo_empty_on_interrupt2/l3gd20h_get_fifo_empty_on_interrupt2 test.\n");
    
    /* enable */
    res = l3gd20h_set_fifo_empty_on_interrupt2(&gs_handle, L3GD20H_BOOL_TRUE);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: set fifo empty on interrupt2 failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: enable fifo empty on interrupt2.\n");
    res = l3gd20h_get_fifo_empty_on_interrupt2(&gs_handle, &enable);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: get fifo empty on interrupt2 failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: check fifo empty on interrupt2 %s.\n", enable == L3GD20H_BOOL_TRUE ? "ok" : "error");
    
    /* disable */
    res = l3gd20h_set_fifo_empty_on_interrupt2(&gs_handle, L3GD20H_BOOL_FALSE);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: set fifo empty on interrupt2 failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: disable fifo empty on interrupt2.\n");
    res = l3gd20h_get_fifo_empty_on_interrupt2(&gs_handle, &enable);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: get fifo empty on interrupt2 failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: check fifo empty on interrupt2 %s.\n", enable == L3GD20H_BOOL_FALSE ? "ok" : "error");

    /* l3gd20h_set_interrupt_selection/l3gd20h_get_interrupt_selection test */
    l3gd20h_interface_debug_print("l3gd20h: l3gd20h_set_interrupt_selection/l3gd20h_get_interrupt_selection test.\n");
    
    /* LPF1 */
    res = l3gd20h_set_interrupt_selection(&gs_handle, L3GD20H_SELECTION_LPF1);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: set interrupt selection failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: set interrupt selection LPF1.\n");
    res = l3gd20h_get_interrupt_selection(&gs_handle, &selection);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: get interrupt selection failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: check interrupt selection %s.\n", selection == L3GD20H_SELECTION_LPF1 ? "ok" : "error");
    
    /* LPF1 HPF */
    res = l3gd20h_set_interrupt_selection(&gs_handle, L3GD20H_SELECTION_LPF1_HPF);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: set interrupt selection failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: set interrupt selection LPF1-HPF.\n");
    res = l3gd20h_get_interrupt_selection(&gs_handle, &selection);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: get interrupt selection failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: check interrupt selection %s.\n", selection == L3GD20H_SELECTION_LPF1_HPF ? "ok" : "error");
    
    /* LPF1 HPF LPF2 */
    res = l3gd20h_set_interrupt_selection(&gs_handle, L3GD20H_SELECTION_LPF1_HPF_LPF2);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: set interrupt selection failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: set interrupt selection LPF1-HPF-LPF2.\n");
    res = l3gd20h_get_interrupt_selection(&gs_handle, &selection);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: get interrupt selection failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: check interrupt selection %s.\n", selection == L3GD20H_SELECTION_LPF1_HPF_LPF2 ? "ok" : "error");
    
    /* l3gd20h_set_interrupt_event/l3gd20h_get_interrupt_event test */
    l3gd20h_interface_debug_print("l3gd20h: l3gd20h_set_interrupt_event/l3gd20h_get_interrupt_event test.\n");
    
    /* L3GD20H_INTERRUPT_EVENT_AND_OR_COMBINATION enable */
    res = l3gd20h_set_interrupt_event(&gs_handle, L3GD20H_INTERRUPT_EVENT_AND_OR_COMBINATION, L3GD20H_BOOL_TRUE);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: set interrupt event failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: enable interrupt event L3GD20H_INTERRUPT_EVENT_AND_OR_COMBINATION.\n");
    res = l3gd20h_get_interrupt_event(&gs_handle, L3GD20H_INTERRUPT_EVENT_AND_OR_COMBINATION, &enable);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: get interrupt event failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: check interrupt event %s.\n", enable == L3GD20H_BOOL_TRUE ? "ok" : "error");
    
    /* L3GD20H_INTERRUPT_EVENT_AND_OR_COMBINATION disable */
    res = l3gd20h_set_interrupt_event(&gs_handle, L3GD20H_INTERRUPT_EVENT_AND_OR_COMBINATION, L3GD20H_BOOL_FALSE);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: set interrupt event failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: disable interrupt event L3GD20H_INTERRUPT_EVENT_AND_OR_COMBINATION.\n");
    res = l3gd20h_get_interrupt_event(&gs_handle, L3GD20H_INTERRUPT_EVENT_AND_OR_COMBINATION, &enable);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: get interrupt event failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: check interrupt event %s.\n", enable == L3GD20H_BOOL_FALSE ? "ok" : "error");
    
    /* L3GD20H_INTERRUPT_EVENT_LATCH enable */
    res = l3gd20h_set_interrupt_event(&gs_handle, L3GD20H_INTERRUPT_EVENT_LATCH, L3GD20H_BOOL_TRUE);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: set interrupt event failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: enable interrupt event L3GD20H_INTERRUPT_EVENT_LATCH.\n");
    res = l3gd20h_get_interrupt_event(&gs_handle, L3GD20H_INTERRUPT_EVENT_LATCH, &enable);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: get interrupt event failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: check interrupt event %s.\n", enable == L3GD20H_BOOL_TRUE ? "ok" : "error");
    
    /* L3GD20H_INTERRUPT_EVENT_LATCH disable */
    res = l3gd20h_set_interrupt_event(&gs_handle, L3GD20H_INTERRUPT_EVENT_LATCH, L3GD20H_BOOL_FALSE);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: set interrupt event failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: disable interrupt event L3GD20H_INTERRUPT_EVENT_LATCH.\n");
    res = l3gd20h_get_interrupt_event(&gs_handle, L3GD20H_INTERRUPT_EVENT_LATCH, &enable);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: get interrupt event failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: check interrupt event %s.\n", enable == L3GD20H_BOOL_FALSE ? "ok" : "error");
    
    /* L3GD20H_INTERRUPT_EVENT_Z_HIGH_EVENT enable */
    res = l3gd20h_set_interrupt_event(&gs_handle, L3GD20H_INTERRUPT_EVENT_Z_HIGH_EVENT, L3GD20H_BOOL_TRUE);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: set interrupt event failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: enable interrupt event L3GD20H_INTERRUPT_EVENT_Z_HIGH_EVENT.\n");
    res = l3gd20h_get_interrupt_event(&gs_handle, L3GD20H_INTERRUPT_EVENT_Z_HIGH_EVENT, &enable);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: get interrupt event failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: check interrupt event %s.\n", enable == L3GD20H_BOOL_TRUE ? "ok" : "error");
    
    /* L3GD20H_INTERRUPT_EVENT_Z_HIGH_EVENT disable */
    res = l3gd20h_set_interrupt_event(&gs_handle, L3GD20H_INTERRUPT_EVENT_Z_HIGH_EVENT, L3GD20H_BOOL_FALSE);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: set interrupt event failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: disable interrupt event L3GD20H_INTERRUPT_EVENT_Z_HIGH_EVENT.\n");
    res = l3gd20h_get_interrupt_event(&gs_handle, L3GD20H_INTERRUPT_EVENT_Z_HIGH_EVENT, &enable);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: get interrupt event failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: check interrupt event %s.\n", enable == L3GD20H_BOOL_FALSE ? "ok" : "error");

    /* L3GD20H_INTERRUPT_EVENT_Z_LOW_EVENT enable */
    res = l3gd20h_set_interrupt_event(&gs_handle, L3GD20H_INTERRUPT_EVENT_Z_LOW_EVENT, L3GD20H_BOOL_TRUE);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: set interrupt event failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: enable interrupt event L3GD20H_INTERRUPT_EVENT_Z_LOW_EVENT.\n");
    res = l3gd20h_get_interrupt_event(&gs_handle, L3GD20H_INTERRUPT_EVENT_Z_LOW_EVENT, &enable);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: get interrupt event failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: check interrupt event %s.\n", enable == L3GD20H_BOOL_TRUE ? "ok" : "error");
    
    /* L3GD20H_INTERRUPT_EVENT_Z_LOW_EVENT disable */
    res = l3gd20h_set_interrupt_event(&gs_handle, L3GD20H_INTERRUPT_EVENT_Z_LOW_EVENT, L3GD20H_BOOL_FALSE);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: set interrupt event failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: disable interrupt event L3GD20H_INTERRUPT_EVENT_Z_LOW_EVENT.\n");
    res = l3gd20h_get_interrupt_event(&gs_handle, L3GD20H_INTERRUPT_EVENT_Z_LOW_EVENT, &enable);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: get interrupt event failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: check interrupt event %s.\n", enable == L3GD20H_BOOL_FALSE ? "ok" : "error");

    /* L3GD20H_INTERRUPT_EVENT_Y_HIGH_EVENT enable */
    res = l3gd20h_set_interrupt_event(&gs_handle, L3GD20H_INTERRUPT_EVENT_Y_HIGH_EVENT, L3GD20H_BOOL_TRUE);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: set interrupt event failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: enable interrupt event L3GD20H_INTERRUPT_EVENT_Y_HIGH_EVENT.\n");
    res = l3gd20h_get_interrupt_event(&gs_handle, L3GD20H_INTERRUPT_EVENT_Y_HIGH_EVENT, &enable);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: get interrupt event failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: check interrupt event %s.\n", enable == L3GD20H_BOOL_TRUE ? "ok" : "error");
    
    /* L3GD20H_INTERRUPT_EVENT_Y_HIGH_EVENT disable */
    res = l3gd20h_set_interrupt_event(&gs_handle, L3GD20H_INTERRUPT_EVENT_Y_HIGH_EVENT, L3GD20H_BOOL_FALSE);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: set interrupt event failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: disable interrupt event L3GD20H_INTERRUPT_EVENT_Y_HIGH_EVENT.\n");
    res = l3gd20h_get_interrupt_event(&gs_handle, L3GD20H_INTERRUPT_EVENT_Y_HIGH_EVENT, &enable);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: get interrupt event failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: check interrupt event %s.\n", enable == L3GD20H_BOOL_FALSE ? "ok" : "error");
    
    /* L3GD20H_INTERRUPT_EVENT_Y_LOW_EVENT enable */
    res = l3gd20h_set_interrupt_event(&gs_handle, L3GD20H_INTERRUPT_EVENT_Y_LOW_EVENT, L3GD20H_BOOL_TRUE);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: set interrupt event failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: enable interrupt event L3GD20H_INTERRUPT_EVENT_Y_LOW_EVENT.\n");
    res = l3gd20h_get_interrupt_event(&gs_handle, L3GD20H_INTERRUPT_EVENT_Y_LOW_EVENT, &enable);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: get interrupt event failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: check interrupt event %s.\n", enable == L3GD20H_BOOL_TRUE ? "ok" : "error");
    
    /* L3GD20H_INTERRUPT_EVENT_Y_LOW_EVENT disable */
    res = l3gd20h_set_interrupt_event(&gs_handle, L3GD20H_INTERRUPT_EVENT_Y_LOW_EVENT, L3GD20H_BOOL_FALSE);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: set interrupt event failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: disable interrupt event L3GD20H_INTERRUPT_EVENT_Y_LOW_EVENT.\n");
    res = l3gd20h_get_interrupt_event(&gs_handle, L3GD20H_INTERRUPT_EVENT_Y_LOW_EVENT, &enable);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: get interrupt event failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: check interrupt event %s.\n", enable == L3GD20H_BOOL_FALSE ? "ok" : "error");

    /* L3GD20H_INTERRUPT_EVENT_X_HIGH_EVENT enable */
    res = l3gd20h_set_interrupt_event(&gs_handle, L3GD20H_INTERRUPT_EVENT_X_HIGH_EVENT, L3GD20H_BOOL_TRUE);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: set interrupt event failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: enable interrupt event L3GD20H_INTERRUPT_EVENT_X_HIGH_EVENT.\n");
    res = l3gd20h_get_interrupt_event(&gs_handle, L3GD20H_INTERRUPT_EVENT_X_HIGH_EVENT, &enable);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: get interrupt event failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: check interrupt event %s.\n", enable == L3GD20H_BOOL_TRUE ? "ok" : "error");
    
    /* L3GD20H_INTERRUPT_EVENT_X_HIGH_EVENT disable */
    res = l3gd20h_set_interrupt_event(&gs_handle, L3GD20H_INTERRUPT_EVENT_X_HIGH_EVENT, L3GD20H_BOOL_FALSE);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: set interrupt event failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: disable interrupt event L3GD20H_INTERRUPT_EVENT_X_HIGH_EVENT.\n");
    res = l3gd20h_get_interrupt_event(&gs_handle, L3GD20H_INTERRUPT_EVENT_X_HIGH_EVENT, &enable);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: get interrupt event failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: check interrupt event %s.\n", enable == L3GD20H_BOOL_FALSE ? "ok" : "error");
    
    /* L3GD20H_INTERRUPT_EVENT_X_LOW_EVENT enable */
    res = l3gd20h_set_interrupt_event(&gs_handle, L3GD20H_INTERRUPT_EVENT_X_LOW_EVENT, L3GD20H_BOOL_TRUE);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: set interrupt event failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: enable interrupt event L3GD20H_INTERRUPT_EVENT_X_LOW_EVENT.\n");
    res = l3gd20h_get_interrupt_event(&gs_handle, L3GD20H_INTERRUPT_EVENT_X_LOW_EVENT, &enable);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: get interrupt event failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: check interrupt event %s.\n", enable == L3GD20H_BOOL_TRUE ? "ok" : "error");
    
    /* L3GD20H_INTERRUPT_EVENT_X_LOW_EVENT disable */
    res = l3gd20h_set_interrupt_event(&gs_handle, L3GD20H_INTERRUPT_EVENT_X_LOW_EVENT, L3GD20H_BOOL_FALSE);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: set interrupt event failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: disable interrupt event L3GD20H_INTERRUPT_EVENT_X_LOW_EVENT.\n");
    res = l3gd20h_get_interrupt_event(&gs_handle, L3GD20H_INTERRUPT_EVENT_X_LOW_EVENT, &enable);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: get interrupt event failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: check interrupt event %s.\n", enable == L3GD20H_BOOL_FALSE ? "ok" : "error");
    
    /* l3gd20h_set_x_interrupt_threshold/l3gd20h_get_x_interrupt_threshold test */
    l3gd20h_interface_debug_print("l3gd20h: l3gd20h_set_x_interrupt_threshold/l3gd20h_get_x_interrupt_threshold test.\n");
    threshold = rand() % 32768;
    res = l3gd20h_set_x_interrupt_threshold(&gs_handle, threshold);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: set x interrupt threshold failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: set x interrupt threshold %d.\n", threshold);
    res = l3gd20h_get_x_interrupt_threshold(&gs_handle, (uint16_t *)&threshold_check);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: get x interrupt threshold failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: check x interrupt threshold %s.\n", threshold == threshold_check ? "ok" : "error");
    
    /* l3gd20h_set_y_interrupt_threshold/l3gd20h_get_y_interrupt_threshold test */
    l3gd20h_interface_debug_print("l3gd20h: l3gd20h_set_y_interrupt_threshold/l3gd20h_get_y_interrupt_threshold test.\n");
    threshold = rand() % 32768;
    res = l3gd20h_set_y_interrupt_threshold(&gs_handle, threshold);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: set y interrupt threshold failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: set y interrupt threshold %d.\n", threshold);
    res = l3gd20h_get_y_interrupt_threshold(&gs_handle, (uint16_t *)&threshold_check);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: get y interrupt threshold failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: check y interrupt threshold %s.\n", threshold == threshold_check ? "ok" : "error");
    
    /* l3gd20h_set_z_interrupt_threshold/l3gd20h_get_z_interrupt_threshold test */
    l3gd20h_interface_debug_print("l3gd20h: l3gd20h_set_z_interrupt_threshold/l3gd20h_get_z_interrupt_threshold test.\n");
    threshold = rand() % 32768;
    res = l3gd20h_set_z_interrupt_threshold(&gs_handle, threshold);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: set z interrupt threshold failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: set z interrupt threshold %d.\n", threshold);
    res = l3gd20h_get_z_interrupt_threshold(&gs_handle, (uint16_t *)&threshold_check);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: get z interrupt threshold failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: check z interrupt threshold %s.\n", threshold == threshold_check ? "ok" : "error");
    
    /* l3gd20h_interrupt_threshold_convert_to_register/l3gd20h_interrupt_threshold_convert_to_data test */
    l3gd20h_interface_debug_print("l3gd20h: l3gd20h_interrupt_threshold_convert_to_register/l3gd20h_interrupt_threshold_convert_to_data test.\n");
    
    /* ±245 dps */
    res = l3gd20h_set_full_scale(&gs_handle, L3GD20H_FULL_SCALE_245_DPS);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: set full scale failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: ±245 dps full scale.\n");
    dps = (float)(rand() % 2000) / 10.0f;
    res = l3gd20h_interrupt_threshold_convert_to_register(&gs_handle, dps, (uint16_t *)&reg);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: interrupt threshold convert to register failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    res = l3gd20h_interrupt_threshold_convert_to_data(&gs_handle, reg, (float *)&dps_check);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: interrupt threshold convert to data failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: %0.4f to %0.4f with reg 0x%04d.\n", dps, dps_check, reg);
    
    /* ±500 dps */
    res = l3gd20h_set_full_scale(&gs_handle, L3GD20H_FULL_SCALE_500_DPS);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: set full scale failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: ±500 dps full scale.\n");
    dps = (float)(rand() % 2000) / 10.0f;
    res = l3gd20h_interrupt_threshold_convert_to_register(&gs_handle, dps, (uint16_t *)&reg);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: interrupt threshold convert to register failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    res = l3gd20h_interrupt_threshold_convert_to_data(&gs_handle, reg, (float *)&dps_check);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: interrupt threshold convert to data failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: %0.4f to %0.4f with reg 0x%04d.\n", dps, dps_check, reg);
    
    /* ±2000 dps */
    res = l3gd20h_set_full_scale(&gs_handle, L3GD20H_FULL_SCALE_2000_DPS);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: set full scale failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: ±2000 dps full scale.\n");
    dps = (float)(rand() % 2000) / 10.0f;
    res = l3gd20h_interrupt_threshold_convert_to_register(&gs_handle, dps, (uint16_t *)&reg);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: interrupt threshold convert to register failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    res = l3gd20h_interrupt_threshold_convert_to_data(&gs_handle, reg, (float *)&dps_check);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: interrupt threshold convert to data failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: %0.4f to %0.4f with reg 0x%04d.\n", dps, dps_check, reg);
    
    /* l3gd20h_set_counter_mode/l3gd20h_get_counter_mode test */
    l3gd20h_interface_debug_print("l3gd20h: l3gd20h_set_counter_mode/l3gd20h_get_counter_mode test.\n");
    
    /* decrement mode */
    res = l3gd20h_set_counter_mode(&gs_handle, L3GD20H_COUNTER_MODE_DECREMENT);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: set counter mode failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: set decrement counter mode.\n");
    res = l3gd20h_get_counter_mode(&gs_handle, &counter_mode);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: get counter mode failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: check counter mode %s.\n", counter_mode == L3GD20H_COUNTER_MODE_DECREMENT ? "ok" : "error");
    
    /* reset mode */
    res = l3gd20h_set_counter_mode(&gs_handle, L3GD20H_COUNTER_MODE_RESET);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: set counter mode failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: set reset counter mode.\n");
    res = l3gd20h_get_counter_mode(&gs_handle, &counter_mode);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: get counter mode failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: check counter mode %s.\n", counter_mode == L3GD20H_COUNTER_MODE_RESET ? "ok" : "error");
    
    /* l3gd20h_set_wait/l3gd20h_get_wait test */
    l3gd20h_interface_debug_print("l3gd20h: l3gd20h_set_wait/l3gd20h_get_wait test.\n");
    
    /* enable */
    res = l3gd20h_set_wait(&gs_handle, L3GD20H_BOOL_TRUE);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: set wait failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: enable wait.\n");
    res = l3gd20h_get_wait(&gs_handle, &enable);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: get wait failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: check wait %s.\n", enable == L3GD20H_BOOL_TRUE ? "ok" : "error");
    
    /* disable */
    res = l3gd20h_set_wait(&gs_handle, L3GD20H_BOOL_FALSE);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: set wait failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: disable wait.\n");
    res = l3gd20h_get_wait(&gs_handle, &enable);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: get wait failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: check wait %s.\n", enable == L3GD20H_BOOL_FALSE ? "ok" : "error");
    
    /* l3gd20h_set_duration/l3gd20h_get_duration test */
    l3gd20h_interface_debug_print("l3gd20h: l3gd20h_set_duration/l3gd20h_get_duration test.\n");
    duration = rand() % 128;
    res = l3gd20h_set_duration(&gs_handle, duration);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: set duration failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: set duration 0x%02X.\n", duration);
    res = l3gd20h_get_duration(&gs_handle, (uint8_t *)&duration_check);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: get duration failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: check duration %s.\n", duration_check == duration ? "ok" : "error");
    
    /* l3gd20h_set_data_ready_active_level/l3gd20h_get_data_ready_active_level test */
    l3gd20h_interface_debug_print("l3gd20h: l3gd20h_set_data_ready_active_level/l3gd20h_get_data_ready_active_level test.\n");
    
    /* active level high */
    res = l3gd20h_set_data_ready_active_level(&gs_handle, L3GD20H_INTERRUPT_ACTIVE_LEVEL_HIGH);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: set data ready active level failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: set data ready active level high.\n");
    res = l3gd20h_get_data_ready_active_level(&gs_handle, &active_level);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: get data ready active level failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: check data ready active level %s.\n", active_level == L3GD20H_INTERRUPT_ACTIVE_LEVEL_HIGH ? "ok" : "error");
    
    /* active level low */
    res = l3gd20h_set_data_ready_active_level(&gs_handle, L3GD20H_INTERRUPT_ACTIVE_LEVEL_LOW);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: set data ready active level failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: set data ready active level low.\n");
    res = l3gd20h_get_data_ready_active_level(&gs_handle, &active_level);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: get data ready active level failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: check data ready active level %s.\n", active_level == L3GD20H_INTERRUPT_ACTIVE_LEVEL_LOW ? "ok" : "error");
    
    /* l3gd20h_set_fifo/l3gd20h_get_fifo test */
    l3gd20h_interface_debug_print("l3gd20h: l3gd20h_set_fifo/l3gd20h_get_fifo test.\n");
    
    /* enable */
    res = l3gd20h_set_fifo(&gs_handle, L3GD20H_BOOL_TRUE);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: set fifo failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: enable fifo.\n");
    res = l3gd20h_get_fifo(&gs_handle, &enable);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: get fifo failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: check fifo %s.\n", enable == L3GD20H_BOOL_TRUE ? "ok" : "error");
    
    /* disable */
    res = l3gd20h_set_fifo(&gs_handle, L3GD20H_BOOL_FALSE);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: set fifo failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: disable fifo.\n");
    res = l3gd20h_get_fifo(&gs_handle, &enable);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: get fifo failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: check fifo %s.\n", enable == L3GD20H_BOOL_FALSE ? "ok" : "error");
    
    /* l3gd20h_set_stop_on_fifo_threshold/l3gd20h_get_stop_on_fifo_threshold test */
    l3gd20h_interface_debug_print("l3gd20h: l3gd20h_set_stop_on_fifo_threshold/l3gd20h_get_stop_on_fifo_threshold test.\n");
    
    /* enable */
    res = l3gd20h_set_stop_on_fifo_threshold(&gs_handle, L3GD20H_BOOL_TRUE);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: set stop on fifo threshold failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: enable stop on fifo threshold.\n");
    res = l3gd20h_get_stop_on_fifo_threshold(&gs_handle, &enable);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: get stop on fifo threshold failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: check stop on fifo threshold %s.\n", enable == L3GD20H_BOOL_TRUE ? "ok" : "error");
    
    /* disable */
    res = l3gd20h_set_stop_on_fifo_threshold(&gs_handle, L3GD20H_BOOL_FALSE);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: set stop on fifo threshold failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: disable stop on fifo threshold.\n");
    res = l3gd20h_get_stop_on_fifo_threshold(&gs_handle, &enable);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: get stop on fifo threshold failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: check stop on fifo threshold %s.\n", enable == L3GD20H_BOOL_FALSE ? "ok" : "error");
    
    /* l3gd20h_set_fifo_mode/l3gd20h_get_fifo_mode test */
    l3gd20h_interface_debug_print("l3gd20h: l3gd20h_set_fifo_mode/l3gd20h_get_fifo_mode test.\n");
    
    /* bypass fifo mode */
    res = l3gd20h_set_fifo_mode(&gs_handle, L3GD20H_FIFO_MODE_BYPASS);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: set fifo mode failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: set fifo mode bypass.\n");
    res = l3gd20h_get_fifo_mode(&gs_handle, &fifo_mode);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: get fifo mode failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: check fifo mode %s.\n", fifo_mode == L3GD20H_FIFO_MODE_BYPASS ? "ok" : "error");
    
    /* fifo fifo mode */
    res = l3gd20h_set_fifo_mode(&gs_handle, L3GD20H_FIFO_MODE_FIFO);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: set fifo mode failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: set fifo mode fifo.\n");
    res = l3gd20h_get_fifo_mode(&gs_handle, &fifo_mode);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: get fifo mode failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: check fifo mode %s.\n", fifo_mode == L3GD20H_FIFO_MODE_FIFO ? "ok" : "error");
    
    /* stream fifo mode */
    res = l3gd20h_set_fifo_mode(&gs_handle, L3GD20H_FIFO_MODE_STREAM);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: set fifo mode failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: set fifo mode stream.\n");
    res = l3gd20h_get_fifo_mode(&gs_handle, &fifo_mode);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: get fifo mode failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: check fifo mode %s.\n", fifo_mode == L3GD20H_FIFO_MODE_STREAM ? "ok" : "error");
    
    /* stream to fifo mode */
    res = l3gd20h_set_fifo_mode(&gs_handle, L3GD20H_FIFO_MODE_STREAM_TO_FIFO);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: set fifo mode failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: set fifo mode stream to fifo.\n");
    res = l3gd20h_get_fifo_mode(&gs_handle, &fifo_mode);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: get fifo mode failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: check fifo mode %s.\n", fifo_mode == L3GD20H_FIFO_MODE_STREAM_TO_FIFO ? "ok" : "error");
    
    /* bypass to stream fifo mode */
    res = l3gd20h_set_fifo_mode(&gs_handle, L3GD20H_FIFO_MODE_BYPASS_TO_STREAM);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: set fifo mode failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: set fifo mode bypass to stream.\n");
    res = l3gd20h_get_fifo_mode(&gs_handle, &fifo_mode);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: get fifo mode failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: check fifo mode %s.\n", fifo_mode == L3GD20H_FIFO_MODE_BYPASS_TO_STREAM ? "ok" : "error");
    
    /* dynamic stream fifo mode */
    res = l3gd20h_set_fifo_mode(&gs_handle, L3GD20H_FIFO_MODE_DYNAMIC_STREAM);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: set fifo mode failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: set fifo mode dynamic stream.\n");
    res = l3gd20h_get_fifo_mode(&gs_handle, &fifo_mode);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: get fifo mode failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: check fifo mode %s.\n", fifo_mode == L3GD20H_FIFO_MODE_DYNAMIC_STREAM ? "ok" : "error");
    
    /* bypass to fifo mode */
    res = l3gd20h_set_fifo_mode(&gs_handle, L3GD20H_FIFO_MODE_BYPASS_TO_FIFO);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: set fifo mode failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: set fifo mode bypass to fifo.\n");
    res = l3gd20h_get_fifo_mode(&gs_handle, &fifo_mode);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: get fifo mode failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: check fifo mode %s.\n", fifo_mode == L3GD20H_FIFO_MODE_BYPASS_TO_FIFO ? "ok" : "error");
    
    /* l3gd20h_set_fifo_threshold/l3gd20h_get_fifo_threshold test */
    l3gd20h_interface_debug_print("l3gd20h: l3gd20h_set_fifo_threshold/l3gd20h_get_fifo_threshold test.\n");
    value = rand() % 31;
    res = l3gd20h_set_fifo_threshold(&gs_handle, value);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: set fifo threshold failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: set fifo threshold %d.\n", value);
    res = l3gd20h_get_fifo_threshold(&gs_handle, (uint8_t *)&value_check);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: get fifo threshold failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: check fifo threshold %s.\n", value == value_check ? "ok" : "error");

    /* l3gd20h_get_fifo_level test */
    l3gd20h_interface_debug_print("l3gd20h: l3gd20h_get_fifo_level test.\n");
    
    /* get fifo level */
    res = l3gd20h_get_fifo_level(&gs_handle, (uint8_t *)&value);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: get fifo level failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: fifo level is %d.\n", value);
    
    /* l3gd20h_get_status test */
    l3gd20h_interface_debug_print("l3gd20h: l3gd20h_get_status test.\n");

    /* get status */
    res = l3gd20h_get_status(&gs_handle, (uint8_t *)&value);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: get status failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: status is %d.\n", value);

    /* l3gd20h_get_interrupt_source test */
    l3gd20h_interface_debug_print("l3gd20h: l3gd20h_get_interrupt_source test.\n");
    
    /* get interrupt source */
    res = l3gd20h_get_interrupt_source(&gs_handle, (uint8_t *)&value);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: get interrupt source failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: interrupt source is 0x%02X.\n", value);

    /* l3gd20h_set_boot/l3gd20h_get_boot test */
    l3gd20h_interface_debug_print("l3gd20h: l3gd20h_set_boot/l3gd20h_get_boot test.\n");
    
    /* normal */
    res = l3gd20h_set_boot(&gs_handle, L3GD20H_BOOT_NORMAL);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: set boot failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: set normal boot.\n");
    res = l3gd20h_get_boot(&gs_handle, &boot);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: get boot failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: check boot %s.\n", boot == L3GD20H_BOOT_NORMAL ? "ok" : "error");
    
    /* reboot */
    res = l3gd20h_set_boot(&gs_handle, L3GD20H_BOOT_REBOOT);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: set boot failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: set reboot.\n");
    l3gd20h_interface_delay_ms(100);
    res = l3gd20h_get_boot(&gs_handle, &boot);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: get boot failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: check boot %s.\n", boot == L3GD20H_BOOT_NORMAL ? "ok" : "error");
    
    /* l3gd20h_soft_reset test */
    l3gd20h_interface_debug_print("l3gd20h: l3gd20h_soft_reset test.\n");
    
    l3gd20h_interface_debug_print("l3gd20h: soft reset.\n");
    
    /* soft reset */
    res = l3gd20h_soft_reset(&gs_handle);
    if (res != 0)
    {
        l3gd20h_interface_debug_print("l3gd20h: soft reset failed.\n");
        (void)l3gd20h_deinit(&gs_handle);
        
        return 1;
    }
    l3gd20h_interface_delay_ms(100);
    
    /* finish register test */
    l3gd20h_interface_debug_print("l3gd20h: finish register test.\n");
    (void)l3gd20h_deinit(&gs_handle);
    
    return 0;
}
