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
 * @file      driver_l3gd20h_interface_template.c
 * @brief     driver l3gd20h interface template source file
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

#include "driver_l3gd20h_interface.h"

/**
 * @brief  interface iic bus init
 * @return status code
 *         - 0 success
 *         - 1 iic init failed
 * @note   none
 */
uint8_t l3gd20h_interface_iic_init(void)
{
    return 0;
}

/**
 * @brief  interface iic bus deinit
 * @return status code
 *         - 0 success
 *         - 1 iic deinit failed
 * @note   none
 */
uint8_t l3gd20h_interface_iic_deinit(void)
{
    return 0;
}

/**
 * @brief      interface iic bus read
 * @param[in]  addr iic device write address
 * @param[in]  reg iic register address
 * @param[out] *buf pointer to a data buffer
 * @param[in]  len length of the data buffer
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 * @note       none
 */
uint8_t l3gd20h_interface_iic_read(uint8_t addr, uint8_t reg, uint8_t *buf, uint16_t len)
{
    return 0;
}

/**
 * @brief     interface iic bus write
 * @param[in] addr iic device write address
 * @param[in] reg iic register address
 * @param[in] *buf pointer to a data buffer
 * @param[in] len length of the data buffer
 * @return    status code
 *            - 0 success
 *            - 1 write failed
 * @note      none
 */
uint8_t l3gd20h_interface_iic_write(uint8_t addr, uint8_t reg, uint8_t *buf, uint16_t len)
{
    return 0;
}

/**
 * @brief  interface spi bus init
 * @return status code
 *         - 0 success
 *         - 1 spi init failed
 * @note   none
 */
uint8_t l3gd20h_interface_spi_init(void)
{
    return 0;
}

/**
 * @brief  interface spi bus deinit
 * @return status code
 *         - 0 success
 *         - 1 spi deinit failed
 * @note   none
 */
uint8_t l3gd20h_interface_spi_deinit(void)
{   
    return 0;
}

/**
 * @brief      interface spi bus read
 * @param[in]  reg register address
 * @param[out] *buf pointer to a data buffer
 * @param[in]  len length of data buffer
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 * @note       none
 */
uint8_t l3gd20h_interface_spi_read(uint8_t reg, uint8_t *buf, uint16_t len)
{
    return 0;
}

/**
 * @brief     interface spi bus write
 * @param[in] reg register address
 * @param[in] *buf pointer to a data buffer
 * @param[in] len length of data buffer
 * @return    status code
 *            - 0 success
 *            - 1 write failed
 * @note      none
 */
uint8_t l3gd20h_interface_spi_write(uint8_t reg, uint8_t *buf, uint16_t len)
{
    return 0;
}

/**
 * @brief     interface delay ms
 * @param[in] ms time
 * @note      none
 */
void l3gd20h_interface_delay_ms(uint32_t ms)
{
    
}

/**
 * @brief     interface print format data
 * @param[in] fmt format data
 * @note      none
 */
void l3gd20h_interface_debug_print(const char *const fmt, ...)
{
    
}

/**
 * @brief     interface receive callback
 * @param[in] type irq type
 * @note      none
 */
void l3gd20h_interface_receive_callback(uint8_t type)
{
    switch (type)
    {
        case L3GD20H_INTERRUPT1_INTERRUPT_ACTIVE :
        {
            l3gd20h_interface_debug_print("l3gd20h: irq active.\n");
            
            break;
        }
        case L3GD20H_INTERRUPT1_Z_HIGH :
        {
            l3gd20h_interface_debug_print("l3gd20h: irq z high threshold.\n");
            
            break;
        }
        case L3GD20H_INTERRUPT1_Z_LOW :
        {
            l3gd20h_interface_debug_print("l3gd20h: irq z low threshold.\n");
            
            break;
        }
        case L3GD20H_INTERRUPT1_Y_HIGH :
        {
            l3gd20h_interface_debug_print("l3gd20h: irq y high threshold.\n");
            
            break;
        }
        case L3GD20H_INTERRUPT1_Y_LOW :
        {
            l3gd20h_interface_debug_print("l3gd20h: irq y low threshold.\n");
            
            break;
        }
        case L3GD20H_INTERRUPT1_X_HIGH :
        {
            l3gd20h_interface_debug_print("l3gd20h: irq x high threshold.\n");
            
            break;
        }
        case L3GD20H_INTERRUPT1_X_LOW :
        {
            l3gd20h_interface_debug_print("l3gd20h: irq x low threshold.\n");
            
            break;
        }
        case L3GD20H_INTERRUPT2_XYZ_OVERRUN :
        {
            l3gd20h_interface_debug_print("l3gd20h: irq xyz overrun.\n");
            
            break;
        }
        case L3GD20H_INTERRUPT2_Z_OVERRUN :
        {
            l3gd20h_interface_debug_print("l3gd20h: irq z overrun.\n");
            
            break;
        }
        case L3GD20H_INTERRUPT2_Y_OVERRUN :
        {
            l3gd20h_interface_debug_print("l3gd20h: irq y overrun.\n");
            
            break;
        }
        case L3GD20H_INTERRUPT2_X_OVERRUN :
        {
            l3gd20h_interface_debug_print("l3gd20h: irq x overrun.\n");
            
            break;
        }
        case L3GD20H_INTERRUPT2_XYZ_DATA_READY :
        {
            l3gd20h_interface_debug_print("l3gd20h: irq xyz data ready.\n");
            
            break;
        }
        case L3GD20H_INTERRUPT2_Z_DATA_READY :
        {
            l3gd20h_interface_debug_print("l3gd20h: irq z data ready.\n");
            
            break;
        }
        case L3GD20H_INTERRUPT2_Y_DATA_READY :
        {
            l3gd20h_interface_debug_print("l3gd20h: irq y data ready.\n");
            
            break;
        }
        case L3GD20H_INTERRUPT2_X_DATA_READY :
        {
            l3gd20h_interface_debug_print("l3gd20h: irq x data ready.\n");
            
            break;
        }
        case L3GD20H_INTERRUPT2_FIFO_THRESHOLD :
        {
            l3gd20h_interface_debug_print("l3gd20h: irq fifo threshold.\n");
            
            break;
        }
        case L3GD20H_INTERRUPT2_FIFO_OVERRRUN :
        {
            l3gd20h_interface_debug_print("l3gd20h: irq fifo overrun.\n");
            
            break;
        }
        case L3GD20H_INTERRUPT2_FIFO_EMPTY :
        {
            l3gd20h_interface_debug_print("l3gd20h: irq fifo empty.\n");
            
            break;
        }
        default :
        {
            l3gd20h_interface_debug_print("l3gd20h: unknown code.\n");
            
            break;
        }
    }
}
