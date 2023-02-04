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
 * @file      driver_l3gd20h.h
 * @brief     driver l3gd20h header file
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

#ifndef DRIVER_L3GD20H_H
#define DRIVER_L3GD20H_H

#include <stdint.h>
#include <stdio.h>
#include <string.h>

#ifdef __cplusplus
extern "C"{
#endif

/**
 * @defgroup l3gd20h_driver l3gd20h driver function
 * @brief    l3gd20h driver modules
 * @{
 */

/**
 * @addtogroup l3gd20h_basic_driver
 * @{
 */

/**
 * @brief l3gd20h interface enumeration definition
 */
typedef enum
{
    L3GD20H_INTERFACE_IIC = 0x00,        /**< iic interface function */
    L3GD20H_INTERFACE_SPI = 0x01,        /**< spi interface function */
} l3gd20h_interface_t;

/**
 * @brief l3gd20h address enumeration definition
 */
typedef enum
{
    L3GD20H_ADDRESS_SDO_0 = 0xD4,        /**< addr pin connected to the GND */
    L3GD20H_ADDRESS_SDO_1 = 0xD6,        /**< addr pin connected to the VCC */
} l3gd20h_address_t;

/**
 * @brief l3gd20h bool enumeration definition
 */
typedef enum
{
    L3GD20H_BOOL_FALSE = 0x00,        /**< false */
    L3GD20H_BOOL_TRUE  = 0x01,        /**< true */
} l3gd20h_bool_t;

/**
 * @brief l3gd20h axis enumeration definition
 */
typedef enum
{
    L3GD20H_AXIS_X = 0x01,        /**< axis x */
    L3GD20H_AXIS_Y = 0x00,        /**< axis y */
    L3GD20H_AXIS_Z = 0x02,        /**< axis z */
} l3gd20h_axis_t;

/**
 * @brief l3gd20h mode enumeration definition
 */
typedef enum
{
    L3GD20H_MODE_POWER_DOWN = 0x00,        /**< power down mode */
    L3GD20H_MODE_NORMAL     = 0x01,        /**< normal mode */
    L3GD20H_MODE_SLEEP      = 0x02,        /**< sleep mode */
} l3gd20h_mode_t;

/**
 * @brief l3gd20h high pass filter mode enumeration definition
 */
typedef enum
{
    L3GD20H_HIGH_PASS_FILTER_MODE_NORMAL_RESET     = 0x00,        /**< normal reset mode */
    L3GD20H_HIGH_PASS_FILTER_MODE_REFERENCE_SIGNAL = 0x01,        /**< reference signal mode */
    L3GD20H_HIGH_PASS_FILTER_MODE_NORMAL           = 0x02,        /**< normal mode */
    L3GD20H_HIGH_PASS_FILTER_MODE_AUTORESET_ON_INT = 0x03,        /**< auto reset on int mode */
} l3gd20h_high_pass_filter_mode_t;

/**
 * @brief l3gd20h high pass filter cut off frequency enumeration definition
 *
 *       ODR=12.5  ODR=25  ODR=50  ODR=100 ODR=200 ODR=400 ODR=800
 * 0x0      1Hz     2Hz     4Hz     8Hz     15Hz    30Hz    56Hz
 * 0x1    0.5Hz     1Hz     2Hz     4Hz     8Hz     15Hz    30Hz
 * 0x2    0.2Hz     0.5Hz   1Hz     2Hz     4Hz     8Hz     15Hz
 * 0x3    0.1Hz     0.2Hz   0.5Hz   1Hz     2Hz     4Hz     8Hz
 * 0x4    0.05Hz    0.1Hz   0.2Hz   0.5Hz   1Hz     2Hz     4Hz
 * 0x5    0.02Hz    0.05Hz  0.1Hz   0.2Hz   0.5Hz   1Hz     2Hz
 * 0x6    0.01Hz    0.02Hz  0.05Hz  0.1Hz   0.2Hz   0.5Hz   1Hz
 * 0x7    0.005Hz   0.01Hz  0.02Hz  0.05Hz  0.1Hz   0.2Hz   0.5Hz
 * 0x8    0.002Hz   0.005Hz 0.01Hz  0.02Hz  0.05Hz  0.1Hz   0.2Hz
 * 0x9    0.001Hz   0.002Hz 0.005Hz 0.01Hz  0.02Hz  0.05Hz  0.1Hz
 */
typedef enum
{
    L3GD20H_HIGH_PASS_FILTER_CUT_OFF_FREQUENCY_0 = 0x00,        /**< frequency 0 */
    L3GD20H_HIGH_PASS_FILTER_CUT_OFF_FREQUENCY_1 = 0x01,        /**< frequency 1 */
    L3GD20H_HIGH_PASS_FILTER_CUT_OFF_FREQUENCY_2 = 0x02,        /**< frequency 2 */
    L3GD20H_HIGH_PASS_FILTER_CUT_OFF_FREQUENCY_3 = 0x03,        /**< frequency 3 */
    L3GD20H_HIGH_PASS_FILTER_CUT_OFF_FREQUENCY_4 = 0x04,        /**< frequency 4 */
    L3GD20H_HIGH_PASS_FILTER_CUT_OFF_FREQUENCY_5 = 0x05,        /**< frequency 5 */
    L3GD20H_HIGH_PASS_FILTER_CUT_OFF_FREQUENCY_6 = 0x06,        /**< frequency 6 */
    L3GD20H_HIGH_PASS_FILTER_CUT_OFF_FREQUENCY_7 = 0x07,        /**< frequency 7 */
    L3GD20H_HIGH_PASS_FILTER_CUT_OFF_FREQUENCY_8 = 0x08,        /**< frequency 8 */
    L3GD20H_HIGH_PASS_FILTER_CUT_OFF_FREQUENCY_9 = 0x09,        /**< frequency 9 */
} l3gd20h_high_pass_filter_cut_off_frequency_t;

/**
 * @brief l3gd20h low odr odr bw enumeration definition
 */
typedef enum 
{
    L3GD20H_LOW_ODR_0_ODR_100HZ_BW_0_12P5HZ = 0x00,        /**< 100Hz bandwidth 12.5 Hz */
    L3GD20H_LOW_ODR_0_ODR_100HZ_BW_1_25HZ   = 0x01,        /**< 100Hz bandwidth 25 Hz */
    L3GD20H_LOW_ODR_0_ODR_100HZ_BW_2_25HZ   = 0x02,        /**< 100Hz bandwidth 25 Hz */
    L3GD20H_LOW_ODR_0_ODR_100HZ_BW_3_25HZ   = 0x03,        /**< 100Hz bandwidth 25 Hz */
    L3GD20H_LOW_ODR_0_ODR_200HZ_BW_0_12P5HZ = 0x04,        /**< 200Hz bandwidth 12.5 Hz */
    L3GD20H_LOW_ODR_0_ODR_200HZ_BW_1_NA     = 0x05,        /**< 200Hz bandwidth NA */
    L3GD20H_LOW_ODR_0_ODR_200HZ_BW_2_NA     = 0x06,        /**< 200Hz bandwidth NA */
    L3GD20H_LOW_ODR_0_ODR_200HZ_BW_3_70HZ   = 0x07,        /**< 200Hz bandwidth 70 Hz */
    L3GD20H_LOW_ODR_0_ODR_400HZ_BW_0_20HZ   = 0x08,        /**< 400Hz bandwidth 20 Hz */
    L3GD20H_LOW_ODR_0_ODR_400HZ_BW_1_25HZ   = 0x09,        /**< 400Hz bandwidth 25 Hz */
    L3GD20H_LOW_ODR_0_ODR_400HZ_BW_2_50HZ   = 0x0A,        /**< 400Hz bandwidth 50 Hz */
    L3GD20H_LOW_ODR_0_ODR_400HZ_BW_3_110HZ  = 0x0B,        /**< 400Hz bandwidth 110 Hz */
    L3GD20H_LOW_ODR_0_ODR_800HZ_BW_0_30HZ   = 0x0C,        /**< 800Hz bandwidth 30 Hz */
    L3GD20H_LOW_ODR_0_ODR_800HZ_BW_1_35HZ   = 0x0D,        /**< 800Hz bandwidth 35 Hz */
    L3GD20H_LOW_ODR_0_ODR_800HZ_BW_2_NA     = 0x0E,        /**< 800Hz bandwidth NA */
    L3GD20H_LOW_ODR_0_ODR_800HZ_BW_3_100HZ  = 0x0F,        /**< 800Hz bandwidth 100 Hz */
    L3GD20H_LOW_ODR_1_ODR_12P5HZ_BW_0_NA    = 0x10,        /**< low power 12.5 Hz bandwidth NA */
    L3GD20H_LOW_ODR_1_ODR_12P5HZ_BW_1_NA    = 0x11,        /**< low power 12.5 Hz bandwidth NA */
    L3GD20H_LOW_ODR_1_ODR_12P5HZ_BW_2_NA    = 0x12,        /**< low power 12.5 Hz bandwidth NA */
    L3GD20H_LOW_ODR_1_ODR_12P5HZ_BW_3_NA    = 0x13,        /**< low power 12.5 Hz bandwidth NA */
    L3GD20H_LOW_ODR_1_ODR_25HZ_BW_0_NA      = 0x14,        /**< low power 25 Hz bandwidth NA */
    L3GD20H_LOW_ODR_1_ODR_25HZ_BW_1_NA      = 0x15,        /**< low power 25 Hz bandwidth NA */
    L3GD20H_LOW_ODR_1_ODR_25HZ_BW_2_NA      = 0x16,        /**< low power 25 Hz bandwidth NA */
    L3GD20H_LOW_ODR_1_ODR_25HZ_BW_3_NA      = 0x17,        /**< low power 25 Hz bandwidth NA */
    L3GD20H_LOW_ODR_1_ODR_50HZ_BW_0_16P6HZ  = 0x18,        /**< low power 50 Hz bandwidth 16.6 Hz */
    L3GD20H_LOW_ODR_1_ODR_50HZ_BW_1_16P6HZ  = 0x19,        /**< low power 50 Hz bandwidth 16.6 Hz */
    L3GD20H_LOW_ODR_1_ODR_50HZ_BW_2_16P6HZ  = 0x1A,        /**< low power 50 Hz bandwidth 16.6 Hz */
    L3GD20H_LOW_ODR_1_ODR_50HZ_BW_3_16P6HZ  = 0x1B,        /**< low power 50 Hz bandwidth 16.6 Hz */
} l3gd20h_lodr_odr_bw_t;

/**
 * @brief l3gd20h data format enumeration definition
 */
typedef enum
{
    L3GD20H_DATA_FORMAT_BIG_ENDIAN    = 0x00,        /**< big endian */
    L3GD20H_DATA_FORMAT_LITTLE_ENDIAN = 0x01,        /**< little endian */
} l3gd20h_data_format_t;

/**
 * @brief l3gd20h full scale type enumeration definition
 */
typedef enum
{
    L3GD20H_FULL_SCALE_245_DPS  = 0x00,        /**< ±245 dps */
    L3GD20H_FULL_SCALE_500_DPS  = 0x01,        /**< ±500 dps */
    L3GD20H_FULL_SCALE_2000_DPS = 0x02,        /**< ±2000 dps */
} l3gd20h_full_scale_t;

/**
 * @brief l3gd20h self test type enumeration definition
 */
typedef enum
{
    L3GD20H_SELF_TEST_NORMAL = 0x00,        /**< normal mode */
    L3GD20H_SELF_TEST_0      = 0x01,        /**< test 0 mode */
    L3GD20H_SELF_TEST_1      = 0x03,        /**< test 1 mode */
} l3gd20h_self_test_t;

/**
 * @brief l3gd20h spi wire enumeration definition
 */
typedef enum
{
    L3GD20H_SPI_WIRE_4 = 0x00,        /**< spi 4 wire */
    L3GD20H_SPI_WIRE_3 = 0x01,        /**< spi 3 wire */
} l3gd20h_spi_wire_t;

/**
 * @brief l3gd20h boot enumeration definition
 */
typedef enum
{
    L3GD20H_BOOT_NORMAL = 0x00,        /**< boot normal */
    L3GD20H_BOOT_REBOOT = 0x01,        /**< boot reboot */
} l3gd20h_boot_t;

/**
 * @brief l3gd20h selection enumeration definition
 */
typedef enum
{
    L3GD20H_SELECTION_LPF1          = 0x00,        /**< LPF1 */
    L3GD20H_SELECTION_LPF1_HPF      = 0x01,        /**< LPF1->HPF */
    L3GD20H_SELECTION_LPF1_HPF_LPF2 = 0x02,        /**< LPF1->HPF->LPF2 */
} l3gd20h_selection_t;

/**
 * @brief l3gd20h status enumeration definition
 */
typedef enum
{
    L3GD20H_STATUS_XYZ_OVERRUN    = 0x07,        /**< xyz overrun */
    L3GD20H_STATUS_Z_OVERRUN      = 0x06,        /**< z overrun */
    L3GD20H_STATUS_Y_OVERRUN      = 0x05,        /**< y overrun */
    L3GD20H_STATUS_X_OVERRUN      = 0x04,        /**< x overrun */
    L3GD20H_STATUS_XYZ_DATA_READY = 0x03,        /**< xyz data ready */
    L3GD20H_STATUS_Z_DATA_READY   = 0x02,        /**< reset mode */
    L3GD20H_STATUS_Y_DATA_READY   = 0x01,        /**< reset mode */
    L3GD20H_STATUS_X_DATA_READY   = 0x00,        /**< reset mode */
} l3gd20h_status_t;

/**
 * @}
 */

/**
 * @addtogroup l3gd20h_interrupt_driver
 * @{
 */

/**
 * @brief l3gd20h interrupt active level enumeration definition
 */
typedef enum
{
    L3GD20H_INTERRUPT_ACTIVE_LEVEL_HIGH = 0x00,        /**< active level high */
    L3GD20H_INTERRUPT_ACTIVE_LEVEL_LOW  = 0x01,        /**< active level low */
} l3gd20h_interrupt_active_level_t;

/**
 * @brief l3gd20h interrupt pin type enumeration definition
 */
typedef enum
{
    L3GD20H_PIN_PUSH_PULL  = 0x00,        /**< push pull */
    L3GD20H_PIN_OPEN_DRAIN = 0x01,        /**< open drain */
} l3gd20h_pin_type_t;

/**
 * @brief l3gd20h counter mode enumeration definition
 */
typedef enum
{
    L3GD20H_COUNTER_MODE_RESET     = 0x00,        /**< reset mode */
    L3GD20H_COUNTER_MODE_DECREMENT = 0x01,        /**< decrement mode */
} l3gd20h_counter_mode_t;

/**
 * @brief l3gd20h interrupt event enumeration definition
 */
typedef enum
{
    L3GD20H_INTERRUPT_EVENT_AND_OR_COMBINATION = 0x07,        /**< and or combination */
    L3GD20H_INTERRUPT_EVENT_LATCH              = 0x06,        /**< latch */
    L3GD20H_INTERRUPT_EVENT_Z_HIGH_EVENT       = 0x05,        /**< z high event */
    L3GD20H_INTERRUPT_EVENT_Z_LOW_EVENT        = 0x04,        /**< z low event */
    L3GD20H_INTERRUPT_EVENT_Y_HIGH_EVENT       = 0x03,        /**< y high event */
    L3GD20H_INTERRUPT_EVENT_Y_LOW_EVENT        = 0x02,        /**< y low event */
    L3GD20H_INTERRUPT_EVENT_X_HIGH_EVENT       = 0x01,        /**< x high event */
    L3GD20H_INTERRUPT_EVENT_X_LOW_EVENT        = 0x00,        /**< x low event */
} l3gd20h_interrupt_event_t;

/**
 * @brief l3gd20h interrupt enumeration definition
 */
typedef enum
{
    L3GD20H_INTERRUPT1_INTERRUPT_ACTIVE = 0x10 | (1 << 6),                             /**< active */
    L3GD20H_INTERRUPT1_Z_HIGH           = 0x10 | (1 << 5),                             /**< z high */
    L3GD20H_INTERRUPT1_Z_LOW            = 0x10 | (1 << 4),                             /**< z low */
    L3GD20H_INTERRUPT1_Y_HIGH           = 0x10 | (1 << 3),                             /**< y high */
    L3GD20H_INTERRUPT1_Y_LOW            = 0x10 | (1 << 2),                             /**< y low */
    L3GD20H_INTERRUPT1_X_HIGH           = 0x10 | (1 << 1),                             /**< x high */
    L3GD20H_INTERRUPT1_X_LOW            = 0x10 | (1 << 0),                             /**< x low */
    L3GD20H_INTERRUPT2_XYZ_OVERRUN      = 0x20 | L3GD20H_STATUS_XYZ_OVERRUN,           /**< xyz overrun */
    L3GD20H_INTERRUPT2_Z_OVERRUN        = 0x20 | L3GD20H_STATUS_Z_OVERRUN,             /**< z overrun */
    L3GD20H_INTERRUPT2_Y_OVERRUN        = 0x20 | L3GD20H_STATUS_Y_OVERRUN,             /**< y overrun */
    L3GD20H_INTERRUPT2_X_OVERRUN        = 0x20 | L3GD20H_STATUS_X_OVERRUN,             /**< x overrun */
    L3GD20H_INTERRUPT2_XYZ_DATA_READY   = 0x20 | L3GD20H_STATUS_XYZ_DATA_READY,        /**< xyz data ready */
    L3GD20H_INTERRUPT2_Z_DATA_READY     = 0x20 | L3GD20H_STATUS_Z_DATA_READY,          /**< z data ready */
    L3GD20H_INTERRUPT2_Y_DATA_READY     = 0x20 | L3GD20H_STATUS_Y_DATA_READY,          /**< y data ready */
    L3GD20H_INTERRUPT2_X_DATA_READY     = 0x20 | L3GD20H_STATUS_X_DATA_READY,          /**< x data ready */
    L3GD20H_INTERRUPT2_FIFO_THRESHOLD   = 0x41,                                        /**< fifo threshold */
    L3GD20H_INTERRUPT2_FIFO_OVERRRUN    = 0x42,                                        /**< fifo overrun */
    L3GD20H_INTERRUPT2_FIFO_EMPTY       = 0x43,                                        /**< fifo empty */
} l3gd20h_interrupt_t;

/**
 * @}
 */

/**
 * @addtogroup l3gd20h_fifo_driver
 * @{
 */

/**
 * @brief l3gd20h fifo mode enumeration definition
 */
typedef enum
{
    L3GD20H_FIFO_MODE_BYPASS           = 0x00,        /**< bypass mode */
    L3GD20H_FIFO_MODE_FIFO             = 0x01,        /**< fifo mode */
    L3GD20H_FIFO_MODE_STREAM           = 0x02,        /**< stream mode */
    L3GD20H_FIFO_MODE_STREAM_TO_FIFO   = 0x03,        /**< stream to fifo mode */
    L3GD20H_FIFO_MODE_BYPASS_TO_STREAM = 0x04,        /**< bypass to stream mode */
    L3GD20H_FIFO_MODE_DYNAMIC_STREAM   = 0x06,        /**< dynamic stream mode */
    L3GD20H_FIFO_MODE_BYPASS_TO_FIFO   = 0x07,        /**< bypass to fifo mode */
} l3gd20h_fifo_mode_t;

/**
 * @}
 */

/**
 * @addtogroup l3gd20h_basic_driver
 * @{
 */

/**
 * @brief l3gd20h handle structure definition
 */
typedef struct l3gd20h_handle_s
{
    uint8_t iic_addr;                                                                   /**< iic address */
    uint8_t (*iic_init)(void);                                                          /**< point to an iic_init function address */
    uint8_t (*iic_deinit)(void);                                                        /**< point to an iic_deinit function address */
    uint8_t (*iic_read)(uint8_t addr, uint8_t reg, uint8_t *buf, uint16_t len);         /**< point to an iic_read function address */
    uint8_t (*iic_write)(uint8_t addr, uint8_t reg, uint8_t *buf, uint16_t len);        /**< point to an iic_write function address */
    uint8_t (*spi_init)(void);                                                          /**< point to a spi_init function address */
    uint8_t (*spi_deinit)(void);                                                        /**< point to a spi_deinit function address */
    uint8_t (*spi_read)(uint8_t reg, uint8_t *buf, uint16_t len);                       /**< point to a spi_read function address */
    uint8_t (*spi_write)(uint8_t reg, uint8_t *buf, uint16_t len);                      /**< point to a spi_write function address */
    void (*receive_callback)(uint8_t type);                                             /**< point to a receive_callback function address */
    void (*delay_ms)(uint32_t ms);                                                      /**< point to a delay_ms function address */
    void (*debug_print)(const char *const fmt, ...);                                    /**< point to a debug_print function address */
    uint8_t inited;                                                                     /**< inited flag */
    uint8_t iic_spi;                                                                    /**< iic spi interface type */
} l3gd20h_handle_t;

/**
 * @brief l3gd20h information structure definition
 */
typedef struct l3gd20h_info_s
{
    char chip_name[32];                /**< chip name */
    char manufacturer_name[32];        /**< manufacturer name */
    char interface[8];                 /**< chip interface name */
    float supply_voltage_min_v;        /**< chip min supply voltage */
    float supply_voltage_max_v;        /**< chip max supply voltage */
    float max_current_ma;              /**< chip max current */
    float temperature_min;             /**< chip min operating temperature */
    float temperature_max;             /**< chip max operating temperature */
    uint32_t driver_version;           /**< driver version */
} l3gd20h_info_t;

/**
 * @}
 */

/**
 * @defgroup l3gd20h_link_driver l3gd20h link driver function
 * @brief    l3gd20h link driver modules
 * @ingroup  l3gd20h_driver
 * @{
 */

/**
 * @brief     initialize l3gd20h_handle_t structure
 * @param[in] HANDLE points to an l3gd20h handle structure
 * @param[in] STRUCTURE is l3gd20h_handle_t
 * @note      none
 */
#define DRIVER_L3GD20H_LINK_INIT(HANDLE, STRUCTURE)               memset(HANDLE, 0, sizeof(STRUCTURE))

/**
 * @brief     link iic_init function
 * @param[in] HANDLE points to an l3gd20h handle structure
 * @param[in] FUC points to an iic_init function address
 * @note      none
 */
#define DRIVER_L3GD20H_LINK_IIC_INIT(HANDLE, FUC)                (HANDLE)->iic_init = FUC

/**
 * @brief     link iic_deinit function
 * @param[in] HANDLE points to an l3gd20h handle structure
 * @param[in] FUC points to an iic_deinit function address
 * @note      none
 */
#define DRIVER_L3GD20H_LINK_IIC_DEINIT(HANDLE, FUC)              (HANDLE)->iic_deinit = FUC

/**
 * @brief     link iic_read function
 * @param[in] HANDLE points to an l3gd20h handle structure
 * @param[in] FUC points to an iic_read function address
 * @note      none
 */
#define DRIVER_L3GD20H_LINK_IIC_READ(HANDLE, FUC)                (HANDLE)->iic_read = FUC

/**
 * @brief     link iic_write function
 * @param[in] HANDLE points to an l3gd20h handle structure
 * @param[in] FUC points to an iic_write function address
 * @note      none
 */
#define DRIVER_L3GD20H_LINK_IIC_WRITE(HANDLE, FUC)               (HANDLE)->iic_write = FUC

/**
 * @brief     link spi_init function
 * @param[in] HANDLE points to an l3gd20h handle structure
 * @param[in] FUC points to a spi_init function address
 * @note      none
 */
#define DRIVER_L3GD20H_LINK_SPI_INIT(HANDLE, FUC)                (HANDLE)->spi_init = FUC

/**
 * @brief     link spi_deinit function
 * @param[in] HANDLE points to an l3gd20h handle structure
 * @param[in] FUC points to a spi_deinit function address
 * @note      none
 */
#define DRIVER_L3GD20H_LINK_SPI_DEINIT(HANDLE, FUC)              (HANDLE)->spi_deinit = FUC

/**
 * @brief     link spi_read function
 * @param[in] HANDLE points to an l3gd20h handle structure
 * @param[in] FUC points to a spi_read function address
 * @note      none
 */
#define DRIVER_L3GD20H_LINK_SPI_READ(HANDLE, FUC)                (HANDLE)->spi_read = FUC

/**
 * @brief     link spi_write function
 * @param[in] HANDLE points to an l3gd20h handle structure
 * @param[in] FUC points to a spi_write function address
 * @note      none
 */
#define DRIVER_L3GD20H_LINK_SPI_WRITE(HANDLE, FUC)               (HANDLE)->spi_write = FUC

/**
 * @brief     link delay_ms function
 * @param[in] HANDLE points to an l3gd20h handle structure
 * @param[in] FUC points to a delay_ms function address
 * @note      none
 */
#define DRIVER_L3GD20H_LINK_DELAY_MS(HANDLE, FUC)                (HANDLE)->delay_ms = FUC

/**
 * @brief     link debug_print function
 * @param[in] HANDLE points to an l3gd20h handle structure
 * @param[in] FUC points to a debug_print function address
 * @note      none
 */
#define DRIVER_L3GD20H_LINK_DEBUG_PRINT(HANDLE, FUC)             (HANDLE)->debug_print = FUC

/**
 * @brief     link receive_callback function
 * @param[in] HANDLE points to an l3gd20h handle structure
 * @param[in] FUC points to a receive_callback function address
 * @note      none
 */
#define DRIVER_L3GD20H_LINK_RECEIVE_CALLBACK(HANDLE, FUC)        (HANDLE)->receive_callback = FUC

/**
 * @}
 */

/**
 * @defgroup l3gd20h_basic_driver l3gd20h basic driver function
 * @brief    l3gd20h basic driver modules
 * @ingroup  l3gd20h_driver
 * @{
 */

/**
 * @brief      get chip's information
 * @param[out] *info points to an l3gd20h info structure
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 * @note       none
 */
uint8_t l3gd20h_info(l3gd20h_info_t *info);

/**
 * @brief     set the chip interface
 * @param[in] *handle points to an l3gd20h handle structure
 * @param[in] interface is the chip interface
 * @return    status code
 *            - 0 success
 *            - 2 handle is NULL
 * @note      none
 */
uint8_t l3gd20h_set_interface(l3gd20h_handle_t *handle, l3gd20h_interface_t interface);

/**
 * @brief      get the chip interface
 * @param[in]  *handle points to an l3gd20h handle structure
 * @param[out] *interface points to a chip interface buffer
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 * @note       none
 */
uint8_t l3gd20h_get_interface(l3gd20h_handle_t *handle, l3gd20h_interface_t *interface);

/**
 * @brief     set the iic address pin
 * @param[in] *handle points to an l3gd20h handle structure
 * @param[in] addr_pin is the address pin
 * @return    status code
 *            - 0 success
 *            - 2 handle is NULL
 * @note      none
 */
uint8_t l3gd20h_set_addr_pin(l3gd20h_handle_t *handle, l3gd20h_address_t addr_pin);

/**
 * @brief      get the iic address pin
 * @param[in]  *handle points to an l3gd20h handle structure
 * @param[out] *addr_pin points to an address pin buffer
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 * @note       none
 */
uint8_t l3gd20h_get_addr_pin(l3gd20h_handle_t *handle, l3gd20h_address_t *addr_pin);

/**
 * @brief     initialize the chip
 * @param[in] *handle points to an l3gd20h handle structure
 * @return    status code
 *            - 0 success
 *            - 1 spi or iic initialization failed
 *            - 2 handle is NULL
 *            - 3 linked functions is NULL
 *            - 4 id is invalid
 * @note      none
 */
uint8_t l3gd20h_init(l3gd20h_handle_t *handle);

/**
 * @brief     close the chip
 * @param[in] *handle points to an l3gd20h handle structure
 * @return    status code
 *            - 0 success
 *            - 1 iic or spi deinit failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 power down failed
 * @note      none
 */
uint8_t l3gd20h_deinit(l3gd20h_handle_t *handle);

/**
 * @brief         read the data
 * @param[in]     *handle points to an l3gd20h handle structure
 * @param[out]    **raw points to a raw data buffer
 * @param[out]    **dps points to a converted data buffer
 * @param[in,out] *len points to a date length buffer
 * @return        status code
 *                - 0 success
 *                - 1 read failed
 *                - 2 handle is NULL
 *                - 3 handle is not initialized
 *                - 4 len is invalid
 * @note          none
 */
uint8_t l3gd20h_read(l3gd20h_handle_t *handle, int16_t (*raw)[3], float (*dps)[3], uint16_t *len);

/**
 * @brief     interrupt handler
 * @param[in] *handle points to an l3gd20h handle structure
 * @param[in] num is the interrupt number
 * @return    status code
 *            - 0 success
 *            - 1 run failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t l3gd20h_irq_handler(l3gd20h_handle_t *handle, uint8_t num);

/**
 * @brief      read the temperature
 * @param[in]  *handle points to an l3gd20h handle structure
 * @param[out] *raw points to a raw temperature buffer
 * @param[out] *temp points to a converted temperature buffer
 * @return     status code
 *             - 0 success
 *             - 1 read temperature failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t l3gd20h_read_temperature(l3gd20h_handle_t *handle, int8_t *raw, float *temp);

/**
 * @brief     set the chip mode
 * @param[in] *handle points to an l3gd20h handle structure
 * @param[in] mode is the chip mode
 * @return    status code
 *            - 0 success
 *            - 1 set mode failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t l3gd20h_set_mode(l3gd20h_handle_t *handle, l3gd20h_mode_t mode);

/**
 * @brief      get the chip mode
 * @param[in]  *handle points to an l3gd20h handle structure
 * @param[out] *mode points to a chip mode buffer
 * @return     status code
 *             - 0 success
 *             - 1 get mode failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t l3gd20h_get_mode(l3gd20h_handle_t *handle, l3gd20h_mode_t *mode);

/**
 * @brief     set the axis
 * @param[in] *handle points to an l3gd20h handle structure
 * @param[in] axis is the set axis
 * @param[in] enable is the bool value
 * @return    status code
 *            - 0 success
 *            - 1 set axis failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t l3gd20h_set_axis(l3gd20h_handle_t *handle, l3gd20h_axis_t axis, l3gd20h_bool_t enable);

/**
 * @brief      get the axis
 * @param[in]  *handle points to an l3gd20h handle structure
 * @param[in]  axis is the set axis
 * @param[out] *enable points to a bool value buffer
 * @return     status code
 *             - 0 success
 *             - 1 get axis failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t l3gd20h_get_axis(l3gd20h_handle_t *handle, l3gd20h_axis_t axis, l3gd20h_bool_t *enable);

/**
 * @brief     set the rate bandwidth
 * @param[in] *handle points to an l3gd20h handle structure
 * @param[in] rate_bandwidth is the rate bandwidth
 * @return    status code
 *            - 0 success
 *            - 1 set rate bandwidth failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t l3gd20h_set_rate_bandwidth(l3gd20h_handle_t *handle, l3gd20h_lodr_odr_bw_t rate_bandwidth);

/**
 * @brief      get the rate bandwidth
 * @param[in]  *handle points to an l3gd20h handle structure
 * @param[out] *rate_bandwidth points to a rate bandwidth buffer
 * @return     status code
 *             - 0 success
 *             - 1 get rate bandwidth failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t l3gd20h_get_rate_bandwidth(l3gd20h_handle_t *handle, l3gd20h_lodr_odr_bw_t *rate_bandwidth);

/**
 * @brief     enable or disable the edge trigger
 * @param[in] *handle points to an l3gd20h handle structure
 * @param[in] enable is a bool value
 * @return    status code
 *            - 0 success
 *            - 1 set edge trigger failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t l3gd20h_set_edge_trigger(l3gd20h_handle_t *handle, l3gd20h_bool_t enable);

/**
 * @brief      get the edge trigger status
 * @param[in]  *handle points to an l3gd20h handle structure
 * @param[out] *enable points to a bool value buffer
 * @return     status code
 *             - 0 success
 *             - 1 get edge trigger failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t l3gd20h_get_edge_trigger(l3gd20h_handle_t *handle, l3gd20h_bool_t *enable);

/**
 * @brief     enable or disable the level trigger
 * @param[in] *handle points to an l3gd20h handle structure
 * @param[in] enable is a bool value
 * @return    status code
 *            - 0 success
 *            - 1 set level trigger failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t l3gd20h_set_level_trigger(l3gd20h_handle_t *handle, l3gd20h_bool_t enable);

/**
 * @brief      get the level trigger status
 * @param[in]  *handle points to an l3gd20h handle structure
 * @param[out] *enable points to a bool value buffer
 * @return     status code
 *             - 0 success
 *             - 1 get level trigger failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t l3gd20h_get_level_trigger(l3gd20h_handle_t *handle, l3gd20h_bool_t *enable);

/**
 * @brief     set the high pass filter mode
 * @param[in] *handle points to an l3gd20h handle structure
 * @param[in] mode is the high pass filter mode
 * @return    status code
 *            - 0 success
 *            - 1 set high pass filter mode failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t l3gd20h_set_high_pass_filter_mode(l3gd20h_handle_t *handle, l3gd20h_high_pass_filter_mode_t mode);

/**
 * @brief      get the high pass filter mode
 * @param[in]  *handle points to an l3gd20h handle structure
 * @param[out] *mode points to a high pass filter mode buffer
 * @return     status code
 *             - 0 success
 *             - 1 get high pass filter mode failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t l3gd20h_get_high_pass_filter_mode(l3gd20h_handle_t *handle, l3gd20h_high_pass_filter_mode_t *mode);

/**
 * @brief     set the high pass filter cut off frequency
 * @param[in] *handle points to an l3gd20h handle structure
 * @param[in] frequency is the high pass filter cut off frequency
 * @return    status code
 *            - 0 success
 *            - 1 set high pass filter cut off frequency failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t l3gd20h_set_high_pass_filter_cut_off_frequency(l3gd20h_handle_t *handle, l3gd20h_high_pass_filter_cut_off_frequency_t frequency);

/**
 * @brief      get the high pass filter cut off frequency
 * @param[in]  *handle points to an l3gd20h handle structure
 * @param[out] *frequency points to a high pass filter cut off frequency buffer
 * @return     status code
 *             - 0 success
 *             - 1 get high pass filter cut off frequency failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t l3gd20h_get_high_pass_filter_cut_off_frequency(l3gd20h_handle_t *handle, l3gd20h_high_pass_filter_cut_off_frequency_t *frequency);

/**
 * @brief     enable or disable the block data update
 * @param[in] *handle points to an l3gd20h handle structure
 * @param[in] enable is a bool value
 * @return    status code
 *            - 0 success
 *            - 1 set block data update failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t l3gd20h_set_block_data_update(l3gd20h_handle_t *handle, l3gd20h_bool_t enable);

/**
 * @brief      get the block data update status
 * @param[in]  *handle points to an l3gd20h handle structure
 * @param[out] *enable points to a bool value buffer
 * @return     status code
 *             - 0 success
 *             - 1 get block data update failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t l3gd20h_get_block_data_update(l3gd20h_handle_t *handle, l3gd20h_bool_t *enable);

/**
 * @brief     set the data format
 * @param[in] *handle points to an l3gd20h handle structure
 * @param[in] data_format is the data format
 * @return    status code
 *            - 0 success
 *            - 1 set data format failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t l3gd20h_set_data_format(l3gd20h_handle_t *handle, l3gd20h_data_format_t data_format);

/**
 * @brief      get the data format
 * @param[in]  *handle points to an l3gd20h handle structure
 * @param[out] *data_format points to a data format buffer
 * @return     status code
 *             - 0 success
 *             - 1 get data format failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t l3gd20h_get_data_format(l3gd20h_handle_t *handle, l3gd20h_data_format_t *data_format);

/**
 * @brief     set the full scale
 * @param[in] *handle points to an l3gd20h handle structure
 * @param[in] full_scale is the full scale
 * @return    status code
 *            - 0 success
 *            - 1 set full scale failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t l3gd20h_set_full_scale(l3gd20h_handle_t *handle, l3gd20h_full_scale_t full_scale);

/**
 * @brief      get the full scale
 * @param[in]  *handle points to an l3gd20h handle structure
 * @param[out] *full_scale points to a full scale buffer
 * @return     status code
 *             - 0 success
 *             - 1 get full scale failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t l3gd20h_get_full_scale(l3gd20h_handle_t *handle, l3gd20h_full_scale_t *full_scale);

/**
 * @brief     enable or disable the level sensitive latched
 * @param[in] *handle points to an l3gd20h handle structure
 * @param[in] enable is a bool value
 * @return    status code
 *            - 0 success
 *            - 1 set level sensitive latched failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t l3gd20h_set_level_sensitive_latched(l3gd20h_handle_t *handle, l3gd20h_bool_t enable);

/**
 * @brief      get the level sensitive latched status
 * @param[in]  *handle points to an l3gd20h handle structure
 * @param[out] *enable points to a bool value buffer
 * @return     status code
 *             - 0 success
 *             - 1 get level sensitive latched failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t l3gd20h_get_level_sensitive_latched(l3gd20h_handle_t *handle, l3gd20h_bool_t *enable);

/**
 * @brief     set the self test
 * @param[in] *handle points to an l3gd20h handle structure
 * @param[in] self_test is the self test
 * @return    status code
 *            - 0 success
 *            - 1 set self test failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t l3gd20h_set_self_test(l3gd20h_handle_t *handle, l3gd20h_self_test_t self_test);

/**
 * @brief      get the self test
 * @param[in]  *handle points to an l3gd20h handle structure
 * @param[out] *self_test points to a self test buffer
 * @return     status code
 *             - 0 success
 *             - 1 get self test failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t l3gd20h_get_self_test(l3gd20h_handle_t *handle, l3gd20h_self_test_t *self_test);

/**
 * @brief     set the spi wire
 * @param[in] *handle points to an l3gd20h handle structure
 * @param[in] spi_wire is the spi wire
 * @return    status code
 *            - 0 success
 *            - 1 set spi wire failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t l3gd20h_set_spi_wire(l3gd20h_handle_t *handle, l3gd20h_spi_wire_t spi_wire);

/**
 * @brief      get the spi wire
 * @param[in]  *handle points to an l3gd20h handle structure
 * @param[out] *spi_wire points to a spi wire buffer
 * @return     status code
 *             - 0 success
 *             - 1 get spi wire failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t l3gd20h_get_spi_wire(l3gd20h_handle_t *handle, l3gd20h_spi_wire_t *spi_wire);

/**
 * @brief     set the boot
 * @param[in] *handle points to an l3gd20h handle structure
 * @param[in] boot is the boot mode
 * @return    status code
 *            - 0 success
 *            - 1 set boot failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t l3gd20h_set_boot(l3gd20h_handle_t *handle, l3gd20h_boot_t boot);

/**
 * @brief      get the boot
 * @param[in]  *handle points to an l3gd20h handle structure
 * @param[out] *boot points to a boot mode buffer
 * @return     status code
 *             - 0 success
 *             - 1 get boot failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t l3gd20h_get_boot(l3gd20h_handle_t *handle, l3gd20h_boot_t *boot);

/**
 * @brief     enable or disable high pass filter
 * @param[in] *handle points to an l3gd20h handle structure
 * @param[in] enable is a bool value
 * @return    status code
 *            - 0 success
 *            - 1 set high pass filter failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t l3gd20h_set_high_pass_filter(l3gd20h_handle_t *handle, l3gd20h_bool_t enable);

/**
 * @brief      get high pass filter status
 * @param[in]  *handle points to an l3gd20h handle structure
 * @param[out] *enable points to a bool value buffer
 * @return     status code
 *             - 0 success
 *             - 1 get high pass filter failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t l3gd20h_get_high_pass_filter(l3gd20h_handle_t *handle, l3gd20h_bool_t *enable);

/**
 * @brief     set the out selection
 * @param[in] *handle points to an l3gd20h handle structure
 * @param[in] selection is the out selection
 * @return    status code
 *            - 0 success
 *            - 1 set out selection failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t l3gd20h_set_out_selection(l3gd20h_handle_t *handle, l3gd20h_selection_t selection);

/**
 * @brief      get the out selection
 * @param[in]  *handle points to an l3gd20h handle structure
 * @param[out] *selection points to an out selection buffer
 * @return     status code
 *             - 0 success
 *             - 1 get out selection failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t l3gd20h_get_out_selection(l3gd20h_handle_t *handle, l3gd20h_selection_t *selection);

/**
 * @brief     set the high pass filter reference
 * @param[in] *handle points to an l3gd20h handle structure
 * @param[in] value is the high pass filter reference value
 * @return    status code
 *            - 0 success
 *            - 1 set high pass filter reference failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t l3gd20h_set_high_pass_filter_reference(l3gd20h_handle_t *handle, uint8_t value);

/**
 * @brief      get the high pass filter reference
 * @param[in]  *handle points to an l3gd20h handle structure
 * @param[out] *value points to a high pass filter reference value buffer
 * @return     status code
 *             - 0 success
 *             - 1 get high pass filter reference failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t l3gd20h_get_high_pass_filter_reference(l3gd20h_handle_t *handle, uint8_t *value);

/**
 * @brief      get the chip status
 * @param[in]  *handle points to an l3gd20h handle structure
 * @param[out] *status points to a status buffer
 * @return     status code
 *             - 0 success
 *             - 1 get status failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t l3gd20h_get_status(l3gd20h_handle_t *handle, uint8_t *status);

/**
 * @brief     enable or disable the iic interface
 * @param[in] *handle points to an l3gd20h handle structure
 * @param[in] enable is a bool value
 * @return    status code
 *            - 0 success
 *            - 1 set iic failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t l3gd20h_set_iic(l3gd20h_handle_t *handle, l3gd20h_bool_t enable);

/**
 * @brief      get the iic interface status
 * @param[in]  *handle points to an l3gd20h handle structure
 * @param[out] *enable points to a bool value buffer
 * @return     status code
 *             - 0 success
 *             - 1 get iic failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t l3gd20h_get_iic(l3gd20h_handle_t *handle, l3gd20h_bool_t *enable);

/**
 * @brief     soft reset the device
 * @param[in] *handle points to an l3gd20h handle structure
 * @return    status code
 *            - 0 success
 *            - 1 soft reset failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t l3gd20h_soft_reset(l3gd20h_handle_t *handle);

/**
 * @}
 */

/**
 * @defgroup l3gd20h_interrupt_driver l3gd20h interrupt driver function
 * @brief    l3gd20h interrupt driver modules
 * @ingroup  l3gd20h_driver
 * @{
 */

/**
 * @brief     enable or disable the interrupt1
 * @param[in] *handle points to an l3gd20h handle structure
 * @param[in] enable is a bool value
 * @return    status code
 *            - 0 success
 *            - 1 set interrupt1 failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t l3gd20h_set_interrupt1(l3gd20h_handle_t *handle, l3gd20h_bool_t enable);

/**
 * @brief      get the interrupt1 status
 * @param[in]  *handle points to an l3gd20h handle structure
 * @param[out] *enable points to a bool value buffer
 * @return     status code
 *             - 0 success
 *             - 1 get interrupt1 failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t l3gd20h_get_interrupt1(l3gd20h_handle_t *handle, l3gd20h_bool_t *enable);

/**
 * @brief     enable or disable boot on the interrupt1
 * @param[in] *handle points to an l3gd20h handle structure
 * @param[in] enable is a bool value
 * @return    status code
 *            - 0 success
 *            - 1 set boot on interrupt1 failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t l3gd20h_set_boot_on_interrupt1(l3gd20h_handle_t *handle, l3gd20h_bool_t enable);

/**
 * @brief      get the boot on the interrupt1 status
 * @param[in]  *handle points to an l3gd20h handle structure
 * @param[out] *enable points to a bool value buffer
 * @return     status code
 *             - 0 success
 *             - 1 get boot on interrupt1 failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t l3gd20h_get_boot_on_interrupt1(l3gd20h_handle_t *handle, l3gd20h_bool_t *enable);

/**
 * @brief     set the interrupt active level
 * @param[in] *handle points to an l3gd20h handle structure
 * @param[in] level is the interrupt active level
 * @return    status code
 *            - 0 success
 *            - 1 set interrupt active level failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t l3gd20h_set_interrupt_active_level(l3gd20h_handle_t *handle, l3gd20h_interrupt_active_level_t level);

/**
 * @brief      get the interrupt active level
 * @param[in]  *handle points to an l3gd20h handle structure
 * @param[out] *level points to an interrupt active level buffer
 * @return     status code
 *             - 0 success
 *             - 1 get interrupt active level failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t l3gd20h_get_interrupt_active_level(l3gd20h_handle_t *handle, l3gd20h_interrupt_active_level_t *level);

/**
 * @brief     set the interrupt pin type
 * @param[in] *handle points to an l3gd20h handle structure
 * @param[in] pin_type is the interrupt pin type
 * @return    status code
 *            - 0 success
 *            - 1 set interrupt pin type failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t l3gd20h_set_interrupt_pin_type(l3gd20h_handle_t *handle, l3gd20h_pin_type_t pin_type);

/**
 * @brief      get the interrupt pin type
 * @param[in]  *handle points to an l3gd20h handle structure
 * @param[out] *pin_type points to an interrupt pin type buffer
 * @return     status code
 *             - 0 success
 *             - 1 get interrupt pin type failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t l3gd20h_get_interrupt_pin_type(l3gd20h_handle_t *handle, l3gd20h_pin_type_t *pin_type);

/**
 * @brief     enable or disable the data ready on interrupt2
 * @param[in] *handle points to an l3gd20h handle structure
 * @param[in] enable is a bool value
 * @return    status code
 *            - 0 success
 *            - 1 set data ready on interrupt2 failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t l3gd20h_set_data_ready_on_interrupt2(l3gd20h_handle_t *handle, l3gd20h_bool_t enable);

/**
 * @brief      get the data ready on interrupt2 status
 * @param[in]  *handle points to an l3gd20h handle structure
 * @param[out] *enable points to a bool value buffer
 * @return     status code
 *             - 0 success
 *             - 1 get data ready on interrupt2 failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t l3gd20h_get_data_ready_on_interrupt2(l3gd20h_handle_t *handle, l3gd20h_bool_t *enable);

/**
 * @brief     enable or disable the fifo threshold on interrupt2
 * @param[in] *handle points to an l3gd20h handle structure
 * @param[in] enable is a bool value
 * @return    status code
 *            - 0 success
 *            - 1 set fifo threshold on interrupt2 failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t l3gd20h_set_fifo_threshold_on_interrupt2(l3gd20h_handle_t *handle, l3gd20h_bool_t enable);

/**
 * @brief      get the fifo threshold on interrupt2 status
 * @param[in]  *handle points to an l3gd20h handle structure
 * @param[out] *enable points to a bool value buffer
 * @return     status code
 *             - 0 success
 *             - 1 get fifo threshold on interrupt2 failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t l3gd20h_get_fifo_threshold_on_interrupt2(l3gd20h_handle_t *handle, l3gd20h_bool_t *enable);

/**
 * @brief     enable or disable the fifo overrun on interrupt2
 * @param[in] *handle points to an l3gd20h handle structure
 * @param[in] enable is a bool value
 * @return    status code
 *            - 0 success
 *            - 1 set fifo overrun on interrupt2 failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t l3gd20h_set_fifo_overrun_on_interrupt2(l3gd20h_handle_t *handle, l3gd20h_bool_t enable);

/**
 * @brief      get the fifo overrun on interrupt2 status
 * @param[in]  *handle points to an l3gd20h handle structure
 * @param[out] *enable points to a bool value buffer
 * @return     status code
 *             - 0 success
 *             - 1 get fifo overrun on interrupt2 failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t l3gd20h_get_fifo_overrun_on_interrupt2(l3gd20h_handle_t *handle, l3gd20h_bool_t *enable);

/**
 * @brief     enable or disable the fifo empty on interrupt2
 * @param[in] *handle points to an l3gd20h handle structure
 * @param[in] enable is a bool value
 * @return    status code
 *            - 0 success
 *            - 1 set fifo empty on interrupt2 failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t l3gd20h_set_fifo_empty_on_interrupt2(l3gd20h_handle_t *handle, l3gd20h_bool_t enable);

/**
 * @brief      get the fifo empty on interrupt2 status
 * @param[in]  *handle points to an l3gd20h handle structure
 * @param[out] *enable points to a bool value buffer
 * @return     status code
 *             - 0 success
 *             - 1 get fifo empty on interrupt2 failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t l3gd20h_get_fifo_empty_on_interrupt2(l3gd20h_handle_t *handle, l3gd20h_bool_t *enable);

/**
 * @brief     set the interrupt selection
 * @param[in] *handle points to an l3gd20h handle structure
 * @param[in] selection is the interrupt selection
 * @return    status code
 *            - 0 success
 *            - 1 set interrupt selection failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t l3gd20h_set_interrupt_selection(l3gd20h_handle_t *handle, l3gd20h_selection_t selection);

/**
 * @brief      get the interrupt selection
 * @param[in]  *handle points to an l3gd20h handle structure
 * @param[out] *selection points to an interrupt selection buffer
 * @return     status code
 *             - 0 success
 *             - 1 get interrupt selection failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t l3gd20h_get_interrupt_selection(l3gd20h_handle_t *handle, l3gd20h_selection_t *selection);

/**
 * @brief     set the interrupt event
 * @param[in] *handle points to an l3gd20h handle structure
 * @param[in] interrupt_event is the chip interrupt event
 * @param[in] enable is a bool value
 * @return    status code
 *            - 0 success
 *            - 1 set interrupt event failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t l3gd20h_set_interrupt_event(l3gd20h_handle_t *handle, l3gd20h_interrupt_event_t interrupt_event, l3gd20h_bool_t enable);

/**
 * @brief      get the interrupt event
 * @param[in]  *handle points to an l3gd20h handle structure
 * @param[in]  interrupt_event is the chip interrupt event
 * @param[out] *enable points to a bool value buffer
 * @return     status code
 *             - 0 success
 *             - 1 get interrupt event failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t l3gd20h_get_interrupt_event(l3gd20h_handle_t *handle, l3gd20h_interrupt_event_t interrupt_event, l3gd20h_bool_t *enable);

/**
 * @brief      get the interrupt source
 * @param[in]  *handle points to an l3gd20h handle structure
 * @param[out] *src points to an interrupt source buffer
 * @return     status code
 *             - 0 success
 *             - 1 get interrupt source failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t l3gd20h_get_interrupt_source(l3gd20h_handle_t *handle, uint8_t *src);

/**
 * @brief     set the x interrupt threshold
 * @param[in] *handle points to an l3gd20h handle structure
 * @param[in] threshold is the interrupt threshold
 * @return    status code
 *            - 0 success
 *            - 1 set x interrupt threshold failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 threshold is invalid
 * @note      none
 */
uint8_t l3gd20h_set_x_interrupt_threshold(l3gd20h_handle_t *handle, uint16_t threshold);

/**
 * @brief      get the x interrupt threshold
 * @param[in]  *handle points to an l3gd20h handle structure
 * @param[out] *threshold points to an interrupt threshold buffer
 * @return     status code
 *             - 0 success
 *             - 1 get x interrupt threshold failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t l3gd20h_get_x_interrupt_threshold(l3gd20h_handle_t *handle, uint16_t *threshold);

/**
 * @brief     set the y interrupt threshold
 * @param[in] *handle points to an l3gd20h handle structure
 * @param[in] threshold is the interrupt threshold
 * @return    status code
 *            - 0 success
 *            - 1 set y interrupt threshold failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 threshold is invalid
 * @note      none
 */
uint8_t l3gd20h_set_y_interrupt_threshold(l3gd20h_handle_t *handle, uint16_t threshold);

/**
 * @brief      get the y interrupt threshold
 * @param[in]  *handle points to an l3gd20h handle structure
 * @param[out] *threshold points to an interrupt threshold buffer
 * @return     status code
 *             - 0 success
 *             - 1 get y interrupt threshold failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t l3gd20h_get_y_interrupt_threshold(l3gd20h_handle_t *handle, uint16_t *threshold);

/**
 * @brief     set the z interrupt threshold
 * @param[in] *handle points to an l3gd20h handle structure
 * @param[in] threshold is the interrupt threshold
 * @return    status code
 *            - 0 success
 *            - 1 set z interrupt threshold failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 threshold is invalid
 * @note      none
 */
uint8_t l3gd20h_set_z_interrupt_threshold(l3gd20h_handle_t *handle, uint16_t threshold);

/**
 * @brief      get the z interrupt threshold
 * @param[in]  *handle points to an l3gd20h handle structure
 * @param[out] *threshold points to an interrupt threshold buffer
 * @return     status code
 *             - 0 success
 *             - 1 get z interrupt threshold failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t l3gd20h_get_z_interrupt_threshold(l3gd20h_handle_t *handle, uint16_t *threshold);

/**
 * @brief      convert the interrupt threshold real data to the register raw data
 * @param[in]  *handle points to an l3gd20h handle structure
 * @param[in]  dps is the interrupt threshold real data
 * @param[out] *reg points to an interrupt threshold raw data buffer
 * @return     status code
 *             - 0 success
 *             - 1 convert to register failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t l3gd20h_interrupt_threshold_convert_to_register(l3gd20h_handle_t *handle, float dps, uint16_t *reg);

/**
 * @brief      convert the interrupt threshold register raw data to the real data
 * @param[in]  *handle points to an l3gd20h handle structure
 * @param[in]  reg is the interrupt threshold raw data
 * @param[out] *dps points to an interrupt threshold real data buffer
 * @return     status code
 *             - 0 success
 *             - 1 convert to data failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t l3gd20h_interrupt_threshold_convert_to_data(l3gd20h_handle_t *handle, uint16_t reg, float *dps);

/**
 * @brief     set the counter mode
 * @param[in] *handle points to an l3gd20h handle structure
 * @param[in] counter_mode is the counter mode
 * @return    status code
 *            - 0 success
 *            - 1 set counter mode failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t l3gd20h_set_counter_mode(l3gd20h_handle_t *handle, l3gd20h_counter_mode_t counter_mode);

/**
 * @brief      get the counter mode
 * @param[in]  *handle points to an l3gd20h handle structure
 * @param[out] *counter_mode points to a counter mode buffer
 * @return     status code
 *             - 0 success
 *             - 1 get counter mode failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t l3gd20h_get_counter_mode(l3gd20h_handle_t *handle, l3gd20h_counter_mode_t *counter_mode);

/**
 * @brief     enable or disable the wait
 * @param[in] *handle points to an l3gd20h handle structure
 * @param[in] enable is a bool value
 * @return    status code
 *            - 0 success
 *            - 1 set wait failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t l3gd20h_set_wait(l3gd20h_handle_t *handle, l3gd20h_bool_t enable);

/**
 * @brief      get the wait status
 * @param[in]  *handle points to an l3gd20h handle structure
 * @param[out] *enable points to a bool value buffer
 * @return     status code
 *             - 0 success
 *             - 1 get wait failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t l3gd20h_get_wait(l3gd20h_handle_t *handle, l3gd20h_bool_t *enable);

/**
 * @brief     set the wait duration
 * @param[in] *handle points to an l3gd20h handle structure
 * @param[in] duration is the wait duration
 * @return    status code
 *            - 0 success
 *            - 1 set duration failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t l3gd20h_set_duration(l3gd20h_handle_t *handle, uint8_t duration);

/**
 * @brief      get the wait duration
 * @param[in]  *handle points to an l3gd20h handle structure
 * @param[out] *duration points to a wait duration buffer
 * @return     status code
 *             - 0 success
 *             - 1 get duration failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t l3gd20h_get_duration(l3gd20h_handle_t *handle, uint8_t *duration);

/**
 * @brief     set the data ready active level
 * @param[in] *handle points to an l3gd20h handle structure
 * @param[in] level is the interrupt active level
 * @return    status code
 *            - 0 success
 *            - 1 set data ready active level failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t l3gd20h_set_data_ready_active_level(l3gd20h_handle_t *handle, l3gd20h_interrupt_active_level_t level);

/**
 * @brief      get the data ready active level
 * @param[in]  *handle points to an l3gd20h handle structure
 * @param[out] *level points to an interrupt active level buffer
 * @return     status code
 *             - 0 success
 *             - 1 get data ready active level failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t l3gd20h_get_data_ready_active_level(l3gd20h_handle_t *handle, l3gd20h_interrupt_active_level_t *level);

/**
 * @}
 */

/**
 * @defgroup l3gd20h_fifo_driver l3gd20h fifo driver function
 * @brief    l3gd20h fifo driver modules
 * @ingroup  l3gd20h_driver
 * @{
 */

/**
 * @brief     enable or disable the fifo
 * @param[in] *handle points to an l3gd20h handle structure
 * @param[in] enable is a bool value
 * @return    status code
 *            - 0 success
 *            - 1 set fifo failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t l3gd20h_set_fifo(l3gd20h_handle_t *handle, l3gd20h_bool_t enable);

/**
 * @brief      get the fifo status
 * @param[in]  *handle points to an l3gd20h handle structure
 * @param[out] *enable points to a bool value buffer
 * @return     status code
 *             - 0 success
 *             - 1 get fifo failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t l3gd20h_get_fifo(l3gd20h_handle_t *handle, l3gd20h_bool_t *enable);

/**
 * @brief     enable or disable stop on fifo threshold
 * @param[in] *handle points to an l3gd20h handle structure
 * @param[in] enable is a bool value
 * @return    status code
 *            - 0 success
 *            - 1 set stop on fifo threshold failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t l3gd20h_set_stop_on_fifo_threshold(l3gd20h_handle_t *handle, l3gd20h_bool_t enable);

/**
 * @brief      get stop on fifo threshold status
 * @param[in]  *handle points to an l3gd20h handle structure
 * @param[out] *enable points to a bool value buffer
 * @return     status code
 *             - 0 success
 *             - 1 get stop on fifo threshold failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t l3gd20h_get_stop_on_fifo_threshold(l3gd20h_handle_t *handle, l3gd20h_bool_t *enable);

/**
 * @brief     set the fifo mode
 * @param[in] *handle points to an l3gd20h handle structure
 * @param[in] fifo_mode is the chip fifo working mode
 * @return    status code
 *            - 0 success
 *            - 1 set fifo mode failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t l3gd20h_set_fifo_mode(l3gd20h_handle_t *handle, l3gd20h_fifo_mode_t fifo_mode);

/**
 * @brief      get the fifo mode
 * @param[in]  *handle points to an l3gd20h handle structure
 * @param[out] *fifo_mode points to a chip fifo working mode buffer
 * @return     status code
 *             - 0 success
 *             - 1 get fifo mode failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t l3gd20h_get_fifo_mode(l3gd20h_handle_t *handle, l3gd20h_fifo_mode_t *fifo_mode);

/**
 * @brief     set the fifo threshold
 * @param[in] *handle points to an l3gd20h handle structure
 * @param[in] threshold is the fifo threshold
 * @return    status code
 *            - 0 success
 *            - 1 set fifo threshold failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 threshold is invalid
 * @note      threshold <= 31
 */
uint8_t l3gd20h_set_fifo_threshold(l3gd20h_handle_t *handle, uint8_t threshold);

/**
 * @brief      get the fifo threshold
 * @param[in]  *handle points to an l3gd20h handle structure
 * @param[out] *threshold points to a fifo threshold buffer
 * @return     status code
 *             - 0 success
 *             - 1 get fifo threshold failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t l3gd20h_get_fifo_threshold(l3gd20h_handle_t *handle, uint8_t *threshold);

/**
 * @brief      get the fifo level
 * @param[in]  *handle points to an l3gd20h handle structure
 * @param[out] *level points to a fifo level buffer
 * @return     status code
 *             - 0 success
 *             - 1 get fifo level failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t l3gd20h_get_fifo_level(l3gd20h_handle_t *handle, uint8_t *level);

/**
 * @}
 */

/**
 * @defgroup l3gd20h_extern_driver l3gd20h extern driver function
 * @brief    l3gd20h extern driver modules
 * @ingroup  l3gd20h_driver
 * @{
 */

/**
 * @brief     set the chip register
 * @param[in] *handle points to an l3gd20h handle structure
 * @param[in] reg is the register address
 * @param[in] *buf points to a data buffer
 * @param[in] len is the data buffer length
 * @return    status code
 *            - 0 success
 *            - 1 write failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t l3gd20h_set_reg(l3gd20h_handle_t *handle, uint8_t reg, uint8_t *buf, uint16_t len);

/**
 * @brief      get the chip register
 * @param[in]  *handle points to an l3gd20h handle structure
 * @param[in]  reg is the register address
 * @param[out] *buf points to a data buffer
 * @param[in]  len is the data buffer length
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t l3gd20h_get_reg(l3gd20h_handle_t *handle, uint8_t reg, uint8_t *buf, uint16_t len);

/**
 * @}
 */

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif
