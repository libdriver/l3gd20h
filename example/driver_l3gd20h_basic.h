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
 * @file      driver_l3gd20h_basic.h
 * @brief     driver l3gd20h basic header file
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

#ifndef DRIVER_L3GD20H_BASIC_H
#define DRIVER_L3GD20H_BASIC_H

#include "driver_l3gd20h_interface.h"

#ifdef __cplusplus
extern "C"{
#endif

/**
 * @defgroup l3gd20h_example_driver l3gd20h example driver function
 * @brief    l3gd20h example driver modules
 * @ingroup  l3gd20h_driver
 * @{
 */

/**
 * @brief l3gd20h basic example default definition
 */
#define L3GD20H_BASIC_DEFAULT_BOOT                                      L3GD20H_BOOT_NORMAL                                 /**< normal boot */
#define L3GD20H_BASIC_DEFAULT_AXIS_X                                    L3GD20H_BOOL_TRUE                                   /**< enable */
#define L3GD20H_BASIC_DEFAULT_AXIS_Y                                    L3GD20H_BOOL_TRUE                                   /**< enable */
#define L3GD20H_BASIC_DEFAULT_AXIS_Z                                    L3GD20H_BOOL_TRUE                                   /**< enable */
#define L3GD20H_BASIC_DEFAULT_RATE_BANDWIDTH                            L3GD20H_LOW_ODR_0_ODR_100HZ_BW_0_12P5HZ             /**< 100 Hz rate bandwidth 12.5 Hz */
#define L3GD20H_BASIC_DEFAULT_EDGE_TRIGGER                              L3GD20H_BOOL_FALSE                                  /**< disable edge trigger */
#define L3GD20H_BASIC_DEFAULT_LEVEL_TRIGGER                             L3GD20H_BOOL_FALSE                                  /**< disable level trigger */
#define L3GD20H_BASIC_DEFAULT_LEVEL_SENSITIVE_LATCHED                   L3GD20H_BOOL_TRUE                                   /**< enable latched */
#define L3GD20H_BASIC_DEFAULT_SELF_TEST                                 L3GD20H_SELF_TEST_NORMAL                            /**< normal mode */
#define L3GD20H_BASIC_DEFAULT_HIGH_PASS_FILTER_MODE                     L3GD20H_HIGH_PASS_FILTER_MODE_NORMAL                /**< normal mode */
#define L3GD20H_BASIC_DEFAULT_HIGH_PASS_FILTER_CUT_OFF                  L3GD20H_HIGH_PASS_FILTER_CUT_OFF_FREQUENCY_0        /**< cut off frequency 0 */
#define L3GD20H_BASIC_DEFAULT_HIGH_PASS_FILTER                          L3GD20H_BOOL_TRUE                                   /**< enable high pass filter */
#define L3GD20H_BASIC_DEFAULT_BLOCK_DATA_UPDATE                         L3GD20H_BOOL_FALSE                                  /**< disable block data update */
#define L3GD20H_BASIC_DEFAULT_OUT_SELECTION                             L3GD20H_SELECTION_LPF1_HPF_LPF2                     /**< LPF1-HPF-LPF2 */
#define L3GD20H_BASIC_DEFAULT_INTERRUPT_SELECTION                       L3GD20H_SELECTION_LPF1_HPF_LPF2                     /**< LPF1-HPF-LPF2 */
#define L3GD20H_BASIC_DEFAULT_HIGH_PASS_FILTER_REFERENCE                0x00                                                /**< reference 0x00 */
#define L3GD20H_BASIC_DEFAULT_INTERRUPT1                                L3GD20H_BOOL_FALSE                                  /**< disable */
#define L3GD20H_BASIC_DEFAULT_BOOT_ON_INTERRUPT1                        L3GD20H_BOOL_FALSE                                  /**< disable */
#define L3GD20H_BASIC_DEFAULT_INTERRUPT_ACTIVE_LEVEL                    L3GD20H_INTERRUPT_ACTIVE_LEVEL_LOW                  /**< active level low */
#define L3GD20H_BASIC_DEFAULT_INTERRUPT_PIN_TYPE                        L3GD20H_PIN_PUSH_PULL                               /**< push pull pin type */
#define L3GD20H_BASIC_DEFAULT_DATA_READY_ON_INTERRUPT2                  L3GD20H_BOOL_FALSE                                  /**< disable data ready on interrupt2 */
#define L3GD20H_BASIC_DEFAULT_FIFO_THRESHOLD_ON_INTERRUPT2              L3GD20H_BOOL_FALSE                                  /**< disable fifo threshold on interrupt2 */
#define L3GD20H_BASIC_DEFAULT_FIFO_OVERRUN_ON_INTERRUPT2                L3GD20H_BOOL_FALSE                                  /**< disable fifo overrun on interrupt2 */
#define L3GD20H_BASIC_DEFAULT_FIFO_EMPTY_ON_INTERRUPT2                  L3GD20H_BOOL_FALSE                                  /**< disable fifo empty on interrupt2 */
#define L3GD20H_BASIC_DEFAULT_INTERRUPT_EVENT_AND_OR_COMBINATION        L3GD20H_BOOL_FALSE                                  /**< disable L3GD20H_INTERRUPT_EVENT_AND_OR_COMBINATION */
#define L3GD20H_BASIC_DEFAULT_INTERRUPT_EVENT_LATCH                     L3GD20H_BOOL_FALSE                                  /**< disable L3GD20H_INTERRUPT_EVENT_LATCH */
#define L3GD20H_BASIC_DEFAULT_INTERRUPT_EVENT_Z_HIGH_EVENT              L3GD20H_BOOL_FALSE                                  /**< disable L3GD20H_INTERRUPT_EVENT_Z_HIGH_EVENT */
#define L3GD20H_BASIC_DEFAULT_INTERRUPT_EVENT_Z_LOW_EVENT               L3GD20H_BOOL_FALSE                                  /**< disable L3GD20H_INTERRUPT_EVENT_Z_LOW_EVENT */
#define L3GD20H_BASIC_DEFAULT_INTERRUPT_EVENT_Y_HIGH_EVENT              L3GD20H_BOOL_FALSE                                  /**< disable L3GD20H_INTERRUPT_EVENT_Y_HIGH_EVENT */
#define L3GD20H_BASIC_DEFAULT_INTERRUPT_EVENT_Y_LOW_EVENT               L3GD20H_BOOL_FALSE                                  /**< disable L3GD20H_INTERRUPT_EVENT_Y_LOW_EVENT */
#define L3GD20H_BASIC_DEFAULT_INTERRUPT_EVENT_X_HIGH_EVENT              L3GD20H_BOOL_FALSE                                  /**< disable L3GD20H_INTERRUPT_EVENT_X_HIGH_EVENT */
#define L3GD20H_BASIC_DEFAULT_INTERRUPT_EVENT_X_LOW_EVENT               L3GD20H_BOOL_FALSE                                  /**< disable L3GD20H_INTERRUPT_EVENT_X_LOW_EVENT */
#define L3GD20H_BASIC_DEFAULT_INTERRUPT_THRESHOLD                       0.0f                                                /**< 0 INTERRUPT_THRESHOLD */
#define L3GD20H_BASIC_DEFAULT_COUNTER_MODE                              L3GD20H_COUNTER_MODE_RESET                          /**< counter mode reset */
#define L3GD20H_BASIC_DEFAULT_WAIT                                      L3GD20H_BOOL_TRUE                                   /**< enable wait */
#define L3GD20H_BASIC_DEFAULT_DURATION                                  0x01                                                /**< duration 0x01 */
#define L3GD20H_BASIC_DEFAULT_DATA_READY_ACTIVE_LEVEL                   L3GD20H_INTERRUPT_ACTIVE_LEVEL_LOW                  /**< set data ready active level low */
#define L3GD20H_BASIC_DEFAULT_STOP_ON_FIFO_THRESHOLD                    L3GD20H_BOOL_TRUE                                   /**< enable stop on fifo threshold */
#define L3GD20H_BASIC_DEFAULT_FIFO_THRESHOLD                            16                                                  /**< fifo threshold 16 */
#define L3GD20H_BASIC_DEFAULT_DATA_FORMAT                               L3GD20H_DATA_FORMAT_LITTLE_ENDIAN                   /**< little endian */
#define L3GD20H_BASIC_DEFAULT_FULL_SCALE                                L3GD20H_FULL_SCALE_245_DPS                          /**< ±245 dps */

/**
 * @brief     basic example init
 * @param[in] interface is the chip interface
 * @param[in] addr_pin is the iic device address
 * @return    status code
 *            - 0 success
 *            - 1 init failed
 * @note      none
 */
uint8_t l3gd20h_basic_init(l3gd20h_interface_t interface, l3gd20h_address_t addr_pin);

/**
 * @brief  basic example deinit
 * @return status code
 *         - 0 success
 *         - 1 deinit failed
 * @note   none
 */
uint8_t l3gd20h_basic_deinit(void);

/**
 * @brief      basic example read
 * @param[out] *dps points to a converted data buffer
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 * @note       none
 */
uint8_t l3gd20h_basic_read(float dps[3]);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif
