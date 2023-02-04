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
 * @file      driver_l3gd20h.c
 * @brief     driver l3gd20h source file
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

#include "driver_l3gd20h.h"

/**
 * @brief chip register definition
 */
#define CHIP_NAME             "STMicroelectronic L3GD20H"        /**< chip name */
#define MANUFACTURER_NAME     "STMicroelectronic"                /**< manufacturer name */
#define SUPPLY_VOLTAGE_MIN    2.2f                               /**< chip min supply voltage */
#define SUPPLY_VOLTAGE_MAX    3.6f                               /**< chip max supply voltage */
#define MAX_CURRENT           5.0f                               /**< chip max current */
#define TEMPERATURE_MIN       -40.0f                             /**< chip min operating temperature */
#define TEMPERATURE_MAX       85.0f                              /**< chip max operating temperature */
#define DRIVER_VERSION        2000                               /**< driver version */

/**
 * @brief chip register definition
 */
#define L3GD20H_REG_WHO_AM_I           0x0F        /**< who am i register */
#define L3GD20H_REG_CTRL1              0x20        /**< control 1 register */
#define L3GD20H_REG_CTRL2              0x21        /**< control 2 register */
#define L3GD20H_REG_CTRL3              0x22        /**< control 3 register */
#define L3GD20H_REG_CTRL4              0x23        /**< control 4 register */
#define L3GD20H_REG_CTRL5              0x24        /**< control 5 register */
#define L3GD20H_REG_REFERENCE          0x25        /**< reference register */
#define L3GD20H_REG_OUT_TEMP           0x26        /**< output temperature register */
#define L3GD20H_REG_STATUS             0x27        /**< status register */
#define L3GD20H_REG_OUT_X_L            0x28        /**< out x low register */
#define L3GD20H_REG_OUT_X_H            0x29        /**< out x high register */
#define L3GD20H_REG_OUT_Y_L            0x2A        /**< out y low register */
#define L3GD20H_REG_OUT_Y_H            0x2B        /**< out y high register */
#define L3GD20H_REG_OUT_Z_L            0x2C        /**< out z low register */
#define L3GD20H_REG_OUT_Z_H            0x2D        /**< out z high register */
#define L3GD20H_REG_FIFO_CTRL          0x2E        /**< fifo control register */
#define L3GD20H_REG_FIFO_SRC           0x2F        /**< fifo source register */
#define L3GD20H_REG_IG_CFG             0x30        /**< interrupt configure register */
#define L3GD20H_REG_IG_SRC             0x31        /**< interrupt source register */
#define L3GD20H_REG_IG_THS_XH          0x32        /**< threshold x high register */
#define L3GD20H_REG_IG_THS_XL          0x33        /**< threshold x low register */
#define L3GD20H_REG_IG_THS_YH          0x34        /**< threshold y high register */
#define L3GD20H_REG_IG_THS_YL          0x35        /**< threshold y low register */
#define L3GD20H_REG_IG_THS_ZH          0x36        /**< threshold z high register */
#define L3GD20H_REG_IG_THS_ZL          0x37        /**< threshold z low register */
#define L3GD20H_REG_IG_DURATION        0x38        /**< interrupt duration register */
#define L3GD20H_REG_LOW_ODR            0x39        /**< low power output data rate register */

/**
 * @brief      iic or spi interface read bytes
 * @param[in]  *handle points to an l3gd20h handle structure
 * @param[in]  reg is the iic register address
 * @param[out] *buf points to a data buffer
 * @param[in]  len is the data length
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 * @note       none
 */
static uint8_t a_l3gd20h_iic_spi_read(l3gd20h_handle_t *handle, uint8_t reg, uint8_t *buf, uint16_t len)
{
    if (handle->iic_spi == L3GD20H_INTERFACE_IIC)                        /* iic interface */
    {
        if (len > 1)                                                     /* len > 1 */
        {
            reg |= 1 << 7;                                               /* flag bit 7 */
        }
        
        if (handle->iic_read(handle->iic_addr, reg, buf, len) != 0)      /* read data */
        {
            return 1;                                                    /* return error */
        }
        else
        {
            return 0;                                                    /* success return 0 */
        }
    }
    else                                                                 /* spi interface */
    {
        if (len > 1)                                                     /* len > 1*/
        {
            reg |= 1 << 6;                                               /* flag address increment */
        }
        reg |= 1 << 7;                                                   /* set read bit */
        
        if (handle->spi_read(reg, buf, len) != 0)                        /* read data */
        {
            return 1;                                                    /* return error */
        }
        else
        {
            return 0;                                                    /* success return 0 */
        }
    }
}

/**
 * @brief     iic or spi interface write bytes
 * @param[in] *handle points to an l3gd20h handle structure
 * @param[in] reg is the iic register address
 * @param[in] *buf points to a data buffer
 * @param[in] len is the data length
 * @return    status code
 *            - 0 success
 *            - 1 write failed
 * @note      none
 */
static uint8_t a_l3gd20h_iic_spi_write(l3gd20h_handle_t *handle, uint8_t reg, uint8_t *buf, uint16_t len)
{
    if (handle->iic_spi == L3GD20H_INTERFACE_IIC)                         /* iic interface */
    {
        if (handle->iic_write(handle->iic_addr, reg, buf, len) != 0)      /* write data */
        {
            return 1;                                                     /* return error */
        }
        else
        {
            return 0;                                                     /* success return 0 */
        }
    }
    else                                                                  /* spi interface */
    {
        if (len > 1)                                                      /* len > 1 */
        {
            reg |= 1 << 6;                                                /* flag address increment */
        }
        reg &= ~(1 << 7);                                                 /* set write bit */
        
        if (handle->spi_write(reg, buf, len) != 0)                        /* write data */
        {
            return 1;                                                     /* return error */
        }
        else
        {
            return 0;                                                     /* success return 0 */
        }
    }
}

/**
 * @brief     set the chip interface
 * @param[in] *handle points to an l3gd20h handle structure
 * @param[in] interface is the chip interface
 * @return    status code
 *            - 0 success
 *            - 2 handle is NULL
 * @note      none
 */
uint8_t l3gd20h_set_interface(l3gd20h_handle_t *handle, l3gd20h_interface_t interface)
{
    if (handle == NULL)                          /* check handle */
    {
        return 2;                                /* return error */
    }
    
    handle->iic_spi = (uint8_t)interface;        /* set the interface */
    
    return 0;                                    /* success return 0 */
}

/**
 * @brief      get the chip interface
 * @param[in]  *handle points to an l3gd20h handle structure
 * @param[out] *interface points to a chip interface buffer
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 * @note       none
 */
uint8_t l3gd20h_get_interface(l3gd20h_handle_t *handle, l3gd20h_interface_t *interface)
{
    if (handle == NULL)                                         /* check handle */
    {
        return 2;                                               /* return error */
    }
    
    *interface = (l3gd20h_interface_t)(handle->iic_spi);        /* get the interface */
    
    return 0;                                                   /* success return 0 */
}

/**
 * @brief     set the iic address pin
 * @param[in] *handle points to an l3gd20h handle structure
 * @param[in] addr_pin is the address pin
 * @return    status code
 *            - 0 success
 *            - 2 handle is NULL
 * @note      none
 */
uint8_t l3gd20h_set_addr_pin(l3gd20h_handle_t *handle, l3gd20h_address_t addr_pin)
{
    if (handle == NULL)                          /* check handle */
    {
        return 2;                                /* return error */
    }
    
    handle->iic_addr = (uint8_t)addr_pin;        /* set the iic address */
    
    return 0;                                    /* success return 0 */
}

/**
 * @brief      get the iic address pin
 * @param[in]  *handle points to an l3gd20h handle structure
 * @param[out] *addr_pin points to an address pin buffer
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 * @note       none
 */
uint8_t l3gd20h_get_addr_pin(l3gd20h_handle_t *handle, l3gd20h_address_t *addr_pin)
{
    if (handle == NULL)                                       /* check handle */
    {
        return 2;                                             /* return error */
    }
    
    *addr_pin = (l3gd20h_address_t)(handle->iic_addr);        /* get the iic address */
    
    return 0;                                                 /* success return 0 */
}

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
uint8_t l3gd20h_set_mode(l3gd20h_handle_t *handle, l3gd20h_mode_t mode)
{
    uint8_t res, prev;
    
    if (handle == NULL)                                                                       /* check handle */
    {
        return 2;                                                                             /* return error */
    }
    if (handle->inited != 1)                                                                  /* check handle initialization */
    {
        return 3;                                                                             /* return error */
    }
    
    res = a_l3gd20h_iic_spi_read(handle, L3GD20H_REG_CTRL1, (uint8_t *)&prev, 1);             /* read config */
    if (res != 0)                                                                             /* check result */
    {
        handle->debug_print("l3gd20h: read ctrl1 failed.\n");                                 /* read ctrl1 failed */
        
        return 1;                                                                             /* return error */
    }
    if (mode == L3GD20H_MODE_SLEEP)                                                           /* if sleep mode */
    {
        prev |= (1 << 3);                                                                     /* set pd */
        prev &= ~(0x07);                                                                      /* clear config */
        
        return a_l3gd20h_iic_spi_write(handle, L3GD20H_REG_CTRL1, (uint8_t *)&prev, 1);       /* write config */
    }
    else
    {
        prev &= ~(1 << 3);                                                                    /* clear pd */
        prev |= (mode << 3);                                                                  /* set mode */
        prev |= 0x07;                                                                         /* set x,y,z enable */
        
        return a_l3gd20h_iic_spi_write(handle, L3GD20H_REG_CTRL1, (uint8_t *)&prev, 1);       /* write config */
    }
}

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
uint8_t l3gd20h_get_mode(l3gd20h_handle_t *handle, l3gd20h_mode_t *mode)
{
    uint8_t res, prev;
    
    if (handle == NULL)                                                                 /* check handle */
    {
        return 2;                                                                       /* return error */
    }
    if (handle->inited != 1)                                                            /* check handle initialization */
    {
        return 3;                                                                       /* return error */
    }
    
    res = a_l3gd20h_iic_spi_read(handle, L3GD20H_REG_CTRL1, (uint8_t *)&prev, 1);       /* read config */
    if (res != 0)                                                                       /* check result */
    {
        handle->debug_print("l3gd20h: read ctrl1 failed.\n");                           /* read ctrl1 failed */
        
        return 1;                                                                       /* return error */
    }
    if ((prev & 0x07) != 0)                                                             /* if x,y,z valid */
    {
        prev &= 1 << 3;                                                                 /* set pd */
        *mode  = (l3gd20h_mode_t)((prev >> 3) & 0x01);                                  /* normal or power down mode */
    }
    else
    {
        if ((prev & (1 << 3)) != 0)                                                     /* if pd == 1 */
        {
            *mode = L3GD20H_MODE_SLEEP;                                                 /* sleep mode*/
        }
        else
        {
            prev &= 1 << 3;                                                             /* set pd */
            *mode  = (l3gd20h_mode_t)((prev >> 3) & 0x01);                              /* set power down mode */
        }
    }
    
    return 0;                                                                           /* success return 0 */
}

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
uint8_t l3gd20h_set_axis(l3gd20h_handle_t *handle, l3gd20h_axis_t axis, l3gd20h_bool_t enable)
{
    uint8_t res, prev;
    
    if (handle == NULL)                                                                   /* check handle */
    {
        return 2;                                                                         /* return error */
    }
    if (handle->inited != 1)                                                              /* check handle initialization */
    {
        return 3;                                                                         /* return error */
    }
    
    res = a_l3gd20h_iic_spi_read(handle, L3GD20H_REG_CTRL1, (uint8_t *)&prev, 1);         /* read config */
    if (res != 0)                                                                         /* check result */
    {
        handle->debug_print("l3gd20h: read ctrl1 failed.\n");                             /* read ctrl1 failed */
        
        return 1;                                                                         /* return error */
    }
    prev &= ~(1 << axis);                                                                 /* clear enable bit */
    prev |= enable << axis;                                                               /* set enable */
    
    return a_l3gd20h_iic_spi_write(handle, L3GD20H_REG_CTRL1, (uint8_t *)&prev, 1);       /* write config */
}

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
uint8_t l3gd20h_get_axis(l3gd20h_handle_t *handle, l3gd20h_axis_t axis, l3gd20h_bool_t *enable) 
{
    uint8_t res, prev;
    
    if (handle == NULL)                                                                 /* check handle */
    {
        return 2;                                                                       /* return error */
    }
    if (handle->inited != 1)                                                            /* check handle initialization */
    {
        return 3;                                                                       /* return error */
    }
    
    res = a_l3gd20h_iic_spi_read(handle, L3GD20H_REG_CTRL1, (uint8_t *)&prev, 1);       /* read config */
    if (res != 0)                                                                       /* check result */
    {
        handle->debug_print("l3gd20h: read ctrl1 failed.\n");                           /* read ctrl1 failed */
        
        return 1;                                                                       /* return error */
    }
    prev &= (1 << axis);                                                                /* get enable bit */
    *enable = (l3gd20h_bool_t)(prev >> axis);                                           /* get bool */
    
    return 0;                                                                           /* success return 0 */
}

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
uint8_t l3gd20h_set_rate_bandwidth(l3gd20h_handle_t *handle, l3gd20h_lodr_odr_bw_t rate_bandwidth) 
{
    uint8_t res, prev;
    
    if (handle == NULL)                                                                   /* check handle */
    {
        return 2;                                                                         /* return error */
    }
    if (handle->inited != 1)                                                              /* check handle initialization */
    {
        return 3;                                                                         /* return error */
    }
    
    res = a_l3gd20h_iic_spi_read(handle, L3GD20H_REG_CTRL1, (uint8_t *)&prev, 1);         /* read config */
    if (res != 0)                                                                         /* check result */
    {
        handle->debug_print("l3gd20h: read ctrl1 failed.\n");                             /* read ctrl1 failed */
        
        return 1;                                                                         /* return error */
    }
    prev &= ~(0xF << 4);                                                                  /* clear rate and bandwidth bits */
    prev |= (rate_bandwidth & 0xF) << 4;                                                  /* set rate and bandwidth */
    if (a_l3gd20h_iic_spi_write(handle, L3GD20H_REG_CTRL1, (uint8_t *)&prev, 1) != 0)     /* write config */
    {
        handle->debug_print("l3gd20h: write ctrl1 failed.\n");                            /* write ctrl1 failed */
        
        return 1;                                                                         /* return error */
    }

    res = a_l3gd20h_iic_spi_read(handle, L3GD20H_REG_LOW_ODR, (uint8_t *)&prev, 1);       /* read low odr */
    if (res != 0)                                                                         /* check result */
    {
        handle->debug_print("l3gd20h: read low odr failed.\n");                           /* read low odr failed */
        
        return 1;                                                                         /* return error */
    }
    prev &= ~(1 << 0);                                                                    /* clear odr bit */
    prev |= ((rate_bandwidth & 0x10) >> 4) & 0x01;                                        /* set odr */
    if (a_l3gd20h_iic_spi_write(handle, L3GD20H_REG_LOW_ODR, (uint8_t *)&prev, 1) != 0)   /* write config */
    {
        handle->debug_print("l3gd20h: write low odr failed.\n");                          /* write low odr failed */
        
        return 1;                                                                         /* return error */
    }
    
    return 0;                                                                             /* success return 0 */
}

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
uint8_t l3gd20h_get_rate_bandwidth(l3gd20h_handle_t *handle, l3gd20h_lodr_odr_bw_t *rate_bandwidth)
{
    uint8_t res, prev1, prev2;
    
    if (handle == NULL)                                                                     /* check handle */
    {
        return 2;                                                                           /* return error */
    }
    if (handle->inited != 1)                                                                /* check handle initialization */
    {
        return 3;                                                                           /* return error */
    }
    
    res = a_l3gd20h_iic_spi_read(handle, L3GD20H_REG_CTRL1, (uint8_t *)&prev1, 1);          /* read config */
    if (res != 0)                                                                           /* check result */
    {
        handle->debug_print("l3gd20h: read ctrl1 failed.\n");                               /* read ctrl1 failed */
        
        return 1;                                                                           /* return error */
    }
    res = a_l3gd20h_iic_spi_read(handle, L3GD20H_REG_LOW_ODR, (uint8_t *)&prev2, 1);        /* read low odr */
    if (res != 0)                                                                           /* check result */
    {
        handle->debug_print("l3gd20h: read low odr failed.\n");                             /* read low odr failed */
        
        return 1;                                                                           /* return error */
    }
    *rate_bandwidth = (l3gd20h_lodr_odr_bw_t)(((prev2 & 0x01) << 4) | (prev1 >> 4));        /* get rate and bandwidth */
    
    return 0;                                                                               /* success return 0 */
}

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
uint8_t l3gd20h_set_edge_trigger(l3gd20h_handle_t *handle, l3gd20h_bool_t enable)
{
    uint8_t res, prev;
    
    if (handle == NULL)                                                                   /* check handle */
    {
        return 2;                                                                         /* return error */
    }
    if (handle->inited != 1)                                                              /* check handle initialization */
    {
        return 3;                                                                         /* return error */
    }
    
    res = a_l3gd20h_iic_spi_read(handle, L3GD20H_REG_CTRL2, (uint8_t *)&prev, 1);         /* read config */
    if (res != 0)                                                                         /* check result */
    {
        handle->debug_print("l3gd20h: read ctrl2 failed.\n");                             /* read ctrl2 failed */
        
        return 1;                                                                         /* return error */
    }
    prev &= ~(1 << 7);                                                                    /* clear enable bit */
    prev |= enable << 7;                                                                  /* set enable */
    
    return a_l3gd20h_iic_spi_write(handle, L3GD20H_REG_CTRL2, (uint8_t *)&prev, 1);       /* write config */
}

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
uint8_t l3gd20h_get_edge_trigger(l3gd20h_handle_t *handle, l3gd20h_bool_t *enable)
{
    uint8_t res, prev;
    
    if (handle == NULL)                                                                 /* check handle */
    {
        return 2;                                                                       /* return error */
    }
    if (handle->inited != 1)                                                            /* check handle initialization */
    {
        return 3;                                                                       /* return error */
    }
    
    res = a_l3gd20h_iic_spi_read(handle, L3GD20H_REG_CTRL2, (uint8_t *)&prev, 1);       /* read config */
    if (res != 0)                                                                       /* check result */
    {
        handle->debug_print("l3gd20h: read ctrl2 failed.\n");                           /* read ctrl2 failed */
        
        return 1;                                                                       /* return error */
    }
    
    prev &= (1 << 7);                                                                   /* get enable bit */
    *enable = (l3gd20h_bool_t)(prev >> 7);                                              /* get bool */
    
    return 0;                                                                           /* success return 0 */
}

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
uint8_t l3gd20h_set_level_trigger(l3gd20h_handle_t *handle, l3gd20h_bool_t enable)
{
    uint8_t res, prev;
    
    if (handle == NULL)                                                                   /* check handle */
    {
        return 2;                                                                         /* return error */
    }
    if (handle->inited != 1)                                                              /* check handle initialization */
    {
        return 3;                                                                         /* return error */
    }
    
    res = a_l3gd20h_iic_spi_read(handle, L3GD20H_REG_CTRL2, (uint8_t *)&prev, 1);         /* read config */
    if (res != 0)                                                                         /* check result */
    {
        handle->debug_print("l3gd20h: read ctrl2 failed.\n");                             /* read ctrl2 failed */
        
        return 1;                                                                         /* return error */
    }
    prev &= ~(1 << 6);                                                                    /* get bool */
    prev |= enable << 6;                                                                  /* set enable */
    
    return a_l3gd20h_iic_spi_write(handle, L3GD20H_REG_CTRL2, (uint8_t *)&prev, 1);       /* write config */
}

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
uint8_t l3gd20h_get_level_trigger(l3gd20h_handle_t *handle, l3gd20h_bool_t *enable)
{
    uint8_t res, prev;
    
    if (handle == NULL)                                                                   /* check handle */
    {
        return 2;                                                                         /* return error */
    }
    if (handle->inited != 1)                                                              /* check handle initialization */
    {
        return 3;                                                                         /* return error */
    }
    
    res = a_l3gd20h_iic_spi_read(handle, L3GD20H_REG_CTRL2, (uint8_t *)&prev, 1);         /* read config */
    if (res != 0)                                                                         /* check result */
    {
        handle->debug_print("l3gd20h: read ctrl2 failed.\n");                             /* read ctrl2 failed */
        
        return 1;                                                                         /* return error */
    }
    prev &= (1 << 6);                                                                     /* get enable */
    *enable = (l3gd20h_bool_t)(prev >> 6);                                                /* get bool */
    
    return 0;                                                                             /* success return 0 */
}

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
uint8_t l3gd20h_set_high_pass_filter_mode(l3gd20h_handle_t *handle, l3gd20h_high_pass_filter_mode_t mode)
{
    uint8_t res, prev;
    
    if (handle == NULL)                                                                   /* check handle */
    {
        return 2;                                                                         /* return error */
    }
    if (handle->inited != 1)                                                              /* check handle initialization */
    {
        return 3;                                                                         /* return error */
    }
    
    res = a_l3gd20h_iic_spi_read(handle, L3GD20H_REG_CTRL2, (uint8_t *)&prev, 1);         /* read config */
    if (res != 0)                                                                         /* check result */
    {
        handle->debug_print("l3gd20h: read ctrl2 failed.\n");                             /* read ctrl2 failed */
        
        return 1;                                                                         /* return error */
    }
    prev &= ~(3 << 4);                                                                    /* clear the mode */
    prev |= mode << 4;                                                                    /* set the mode */
    
    return a_l3gd20h_iic_spi_write(handle, L3GD20H_REG_CTRL2, (uint8_t *)&prev, 1);       /* write config */
}

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
uint8_t l3gd20h_get_high_pass_filter_mode(l3gd20h_handle_t *handle, l3gd20h_high_pass_filter_mode_t *mode)
{
    uint8_t res, prev;
    
    if (handle == NULL)                                                                   /* check handle */
    {
        return 2;                                                                         /* return error */
    }
    if (handle->inited != 1)                                                              /* check handle initialization */
    {
        return 3;                                                                         /* return error */
    }
    
    res = a_l3gd20h_iic_spi_read(handle, L3GD20H_REG_CTRL2, (uint8_t *)&prev, 1);         /* read config */
    if (res != 0)                                                                         /* check result */
    {
        handle->debug_print("l3gd20h: read ctrl2 failed.\n");                             /* read ctrl2 failed */
        
        return 1;                                                                         /* return error */
    }
    prev &= (3 << 4);                                                                     /* get the mode bits */
    *mode = (l3gd20h_high_pass_filter_mode_t)(prev >> 4);                                 /* get the mode */
    
    return 0;                                                                             /* success return 0 */
}

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
uint8_t l3gd20h_set_high_pass_filter_cut_off_frequency(l3gd20h_handle_t *handle, l3gd20h_high_pass_filter_cut_off_frequency_t frequency)
{
    uint8_t res, prev;
    
    if (handle == NULL)                                                                   /* check handle */
    {
        return 2;                                                                         /* return error */
    }
    if (handle->inited != 1)                                                              /* check handle initialization */
    {
        return 3;                                                                         /* return error */
    }
    
    res = a_l3gd20h_iic_spi_read(handle, L3GD20H_REG_CTRL2, (uint8_t *)&prev, 1);         /* read config */
    if (res != 0)                                                                         /* check result */
    {
        handle->debug_print("l3gd20h: read ctrl2 failed.\n");                             /* read ctrl2 failed */
        
        return 1;                                                                         /* return error */
    }
    prev &= ~(0x0F);                                                                      /* clear the cut-off frequency */
    prev |= frequency;                                                                    /* set the frequency */
    
    return a_l3gd20h_iic_spi_write(handle, L3GD20H_REG_CTRL2, (uint8_t *)&prev, 1);       /* write config */
}

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
uint8_t l3gd20h_get_high_pass_filter_cut_off_frequency(l3gd20h_handle_t *handle, l3gd20h_high_pass_filter_cut_off_frequency_t *frequency)
{
    uint8_t res, prev;
    
    if (handle == NULL)                                                                   /* check handle */
    {
        return 2;                                                                         /* return error */
    }
    if (handle->inited != 1)                                                              /* check handle initialization */
    {
        return 3;                                                                         /* return error */
    }
    
    res = a_l3gd20h_iic_spi_read(handle, L3GD20H_REG_CTRL2, (uint8_t *)&prev, 1);         /* read config */
    if (res != 0)                                                                         /* check result */
    {
        handle->debug_print("l3gd20h: read ctrl2 failed.\n");                             /* read ctrl2 failed */
        
        return 1;                                                                         /* return error */
    }
    prev &= 0x0F;                                                                         /* get the cut-off frequency */
    *frequency = (l3gd20h_high_pass_filter_cut_off_frequency_t)(prev >> 0);               /* get the frequency */
    
    return 0;                                                                             /* success return 0 */
}

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
uint8_t l3gd20h_set_interrupt1(l3gd20h_handle_t *handle, l3gd20h_bool_t enable)
{
    uint8_t res, prev;
    
    if (handle == NULL)                                                                   /* check handle */
    {
        return 2;                                                                         /* return error */
    }
    if (handle->inited != 1)                                                              /* check handle initialization */
    {
        return 3;                                                                         /* return error */
    }
    
    res = a_l3gd20h_iic_spi_read(handle, L3GD20H_REG_CTRL3, (uint8_t *)&prev, 1);         /* read config */
    if (res != 0)                                                                         /* check result */
    {
        handle->debug_print("l3gd20h: read ctrl3 failed.\n");                             /* read ctrl3 failed */
        
        return 1;                                                                         /* return error */
    }
    prev &= ~(1 << 7);                                                                    /* clear enable */
    prev |= enable << 7;                                                                  /* set enable */
    
    return a_l3gd20h_iic_spi_write(handle, L3GD20H_REG_CTRL3, (uint8_t *)&prev, 1);       /* write config */
}

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
uint8_t l3gd20h_get_interrupt1(l3gd20h_handle_t *handle, l3gd20h_bool_t *enable)
{
    uint8_t res, prev;
    
    if (handle == NULL)                                                                 /* check handle */
    {
        return 2;                                                                       /* return error */
    }
    if (handle->inited != 1)                                                            /* check handle initialization */
    {
        return 3;                                                                       /* return error */
    }
    
    res = a_l3gd20h_iic_spi_read(handle, L3GD20H_REG_CTRL3, (uint8_t *)&prev, 1);       /* read config */
    if (res != 0)                                                                       /* check result */
    {
        handle->debug_print("l3gd20h: read ctrl3 failed.\n");                           /* read ctrl3 failed */
        
        return 1;                                                                       /* return error */
    }
    prev &= (1 << 7);                                                                   /* get enable bit */
    *enable = (l3gd20h_bool_t)(prev >> 7);                                              /* get bool */
    
    return 0;                                                                           /* success return 0 */
}

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
uint8_t l3gd20h_set_boot_on_interrupt1(l3gd20h_handle_t *handle, l3gd20h_bool_t enable)
{
    uint8_t res, prev;
    
    if (handle == NULL)                                                                   /* check handle */
    {
        return 2;                                                                         /* return error */
    }
    if (handle->inited != 1)                                                              /* check handle initialization */
    {
        return 3;                                                                         /* return error */
    }
    
    res = a_l3gd20h_iic_spi_read(handle, L3GD20H_REG_CTRL3, (uint8_t *)&prev, 1);         /* read config */
    if (res != 0)                                                                         /* check result */
    {
        handle->debug_print("l3gd20h: read ctrl3 failed.\n");                             /* read ctrl3 failed */
        
        return 1;                                                                         /* return error */
    }
    prev &= ~(1 << 6);                                                                    /* clear enable bit */
    prev |= enable << 6;                                                                  /* set enable */
    
    return a_l3gd20h_iic_spi_write(handle, L3GD20H_REG_CTRL3, (uint8_t *)&prev, 1);       /* write config */
}

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
uint8_t l3gd20h_get_boot_on_interrupt1(l3gd20h_handle_t *handle, l3gd20h_bool_t *enable)
{
    uint8_t res, prev;
    
    if (handle == NULL)                                                                 /* check handle */
    {
        return 2;                                                                       /* return error */
    }
    if (handle->inited != 1)                                                            /* check handle initialization */
    {
        return 3;                                                                       /* return error */
    }
    
    res = a_l3gd20h_iic_spi_read(handle, L3GD20H_REG_CTRL3, (uint8_t *)&prev, 1);       /* read config */
    if (res != 0)                                                                       /* check result */
    {
        handle->debug_print("l3gd20h: read ctrl3 failed.\n");                           /* read ctrl3 failed */
        
        return 1;                                                                       /* return error */
    }
    prev &= (1 << 6);                                                                   /* get enable bit */
    *enable = (l3gd20h_bool_t)(prev >> 6);                                              /* get bool */
    
    return 0;                                                                           /* success return 0 */
}

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
uint8_t l3gd20h_set_interrupt_active_level(l3gd20h_handle_t *handle, l3gd20h_interrupt_active_level_t level)
{
    uint8_t res, prev;
    
    if (handle == NULL)                                                                   /* check handle */
    {
        return 2;                                                                         /* return error */
    }
    if (handle->inited != 1)                                                              /* check handle initialization */
    {
        return 3;                                                                         /* return error */
    }
    
    res = a_l3gd20h_iic_spi_read(handle, L3GD20H_REG_CTRL3, (uint8_t *)&prev, 1);         /* read config */
    if (res != 0)                                                                         /* check result */
    {
        handle->debug_print("l3gd20h: read ctrl3 failed.\n");                             /* read ctrl3 failed */
        
        return 1;                                                                         /* return error */
    }
    prev &= ~(1 << 5);                                                                    /* clear level bit */
    prev |= level << 5;                                                                   /* set level bit */
    
    return a_l3gd20h_iic_spi_write(handle, L3GD20H_REG_CTRL3, (uint8_t *)&prev, 1);       /* write config */
}

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
uint8_t l3gd20h_get_interrupt_active_level(l3gd20h_handle_t *handle, l3gd20h_interrupt_active_level_t *level)
{
    uint8_t res, prev;
    
    if (handle == NULL)                                                                 /* check handle */
    {
        return 2;                                                                       /* return error */
    }
    if (handle->inited != 1)                                                            /* check handle initialization */
    {
        return 3;                                                                       /* return error */
    }
    
    res = a_l3gd20h_iic_spi_read(handle, L3GD20H_REG_CTRL3, (uint8_t *)&prev, 1);       /* read config */
    if (res != 0)                                                                       /* check result */
    {
        handle->debug_print("l3gd20h: read ctrl3 failed.\n");                           /* read ctrl3 failed */
        
        return 1;                                                                       /* return error */
    }
    prev &= (1 << 5);                                                                   /* get the level bit */
    *level = (l3gd20h_interrupt_active_level_t)(prev >> 5);                             /* get the level */
    
    return 0;                                                                           /* success return 0 */
}

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
uint8_t l3gd20h_set_interrupt_pin_type(l3gd20h_handle_t *handle, l3gd20h_pin_type_t pin_type)
{
    uint8_t res, prev;
    
    if (handle == NULL)                                                                   /* check handle */
    {
        return 2;                                                                         /* return error */
    }
    if (handle->inited != 1)                                                              /* check handle initialization */
    {
        return 3;                                                                         /* return error */
    }
    
    res = a_l3gd20h_iic_spi_read(handle, L3GD20H_REG_CTRL3, (uint8_t *)&prev, 1);         /* read config */
    if (res != 0)                                                                         /* check result */
    {
        handle->debug_print("l3gd20h: read ctrl3 failed.\n");                             /* read ctrl3 failed */
        
        return 1;                                                                         /* return error */
    }
    prev &= ~(1 << 4);                                                                    /* clear pin type bit */
    prev |= pin_type << 4;                                                                /* set pin type */
    
    return a_l3gd20h_iic_spi_write(handle, L3GD20H_REG_CTRL3, (uint8_t *)&prev, 1);       /* write config */
}

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
uint8_t l3gd20h_get_interrupt_pin_type(l3gd20h_handle_t *handle, l3gd20h_pin_type_t *pin_type)
{
    uint8_t res, prev;
    
    if (handle == NULL)                                                                 /* check handle */
    {
        return 2;                                                                       /* return error */
    }
    if (handle->inited != 1)                                                            /* check handle initialization */
    {
        return 3;                                                                       /* return error */
    }
    
    res = a_l3gd20h_iic_spi_read(handle, L3GD20H_REG_CTRL3, (uint8_t *)&prev, 1);       /* read config */
    if (res != 0)                                                                       /* check result */
    {
        handle->debug_print("l3gd20h: read ctrl3 failed.\n");                           /* read ctrl3 failed */
        
        return 1;                                                                       /* return error */
    }
    prev &= (1 << 4);                                                                   /* get pin type bit */
    *pin_type = (l3gd20h_pin_type_t)(prev >> 4);                                        /* get pin type */
    
    return 0;                                                                           /* success return 0 */
}

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
uint8_t l3gd20h_set_data_ready_on_interrupt2(l3gd20h_handle_t *handle, l3gd20h_bool_t enable)
{
    uint8_t res, prev;
    
    if (handle == NULL)                                                                   /* check handle */
    {
        return 2;                                                                         /* return error */
    }
    if (handle->inited != 1)                                                              /* check handle initialization */
    {
        return 3;                                                                         /* return error */
    }
    
    res = a_l3gd20h_iic_spi_read(handle, L3GD20H_REG_CTRL3, (uint8_t *)&prev, 1);         /* read config */
    if (res != 0)                                                                         /* check result */
    {
        handle->debug_print("l3gd20h: read ctrl3 failed.\n");                             /* read ctrl3 failed */
        
        return 1;                                                                         /* return error */
    }
    prev &= ~(1 << 3);                                                                    /* set data ready bit */
    prev |= enable << 3;                                                                  /* set data ready */
    
    return a_l3gd20h_iic_spi_write(handle, L3GD20H_REG_CTRL3, (uint8_t *)&prev, 1);       /* write config */
}

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
uint8_t l3gd20h_get_data_ready_on_interrupt2(l3gd20h_handle_t *handle, l3gd20h_bool_t *enable)
{
    uint8_t res, prev;
    
    if (handle == NULL)                                                                 /* check handle */
    {
        return 2;                                                                       /* return error */
    }
    if (handle->inited != 1)                                                            /* check handle initialization */
    {
        return 3;                                                                       /* return error */
    }
    
    res = a_l3gd20h_iic_spi_read(handle, L3GD20H_REG_CTRL3, (uint8_t *)&prev, 1);       /* read config */
    if (res != 0)                                                                       /* check result */
    {
        handle->debug_print("l3gd20h: read ctrl3 failed.\n");                           /* read ctrl3 failed */
        
        return 1;                                                                       /* return error */
    }
    prev &= (1 << 3);                                                                   /* get enable bit */
    *enable = (l3gd20h_bool_t)(prev >> 3);                                              /* get bool */
    
    return 0;                                                                           /* success return 0 */
}

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
uint8_t l3gd20h_set_fifo_threshold_on_interrupt2(l3gd20h_handle_t *handle, l3gd20h_bool_t enable)
{
    uint8_t res, prev;
    
    if (handle == NULL)                                                                   /* check handle */
    {
        return 2;                                                                         /* return error */
    }
    if (handle->inited != 1)                                                              /* check handle initialization */
    {
        return 3;                                                                         /* return error */
    }
    
    res = a_l3gd20h_iic_spi_read(handle, L3GD20H_REG_CTRL3, (uint8_t *)&prev, 1);         /* read config */
    if (res != 0)                                                                         /* check result */
    {
        handle->debug_print("l3gd20h: read ctrl3 failed.\n");                             /* read ctrl3 failed */
        
        return 1;                                                                         /* return error */
    }
    prev &= ~(1 << 2);                                                                    /* clear enable bit */
    prev |= enable << 2;                                                                  /* set enable */
   
    return a_l3gd20h_iic_spi_write(handle, L3GD20H_REG_CTRL3, (uint8_t *)&prev, 1);       /* write config */
}

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
uint8_t l3gd20h_get_fifo_threshold_on_interrupt2(l3gd20h_handle_t *handle, l3gd20h_bool_t *enable)
{
    uint8_t res, prev;
    
    if (handle == NULL)                                                                 /* check handle */
    {
        return 2;                                                                       /* return error */
    }
    if (handle->inited != 1)                                                            /* check handle initialization */
    {
        return 3;                                                                       /* return error */
    }
    
    res = a_l3gd20h_iic_spi_read(handle, L3GD20H_REG_CTRL3, (uint8_t *)&prev, 1);       /* read config */
    if (res != 0)                                                                       /* check result */
    {
        handle->debug_print("l3gd20h: read ctrl3 failed.\n");                           /* read ctrl3 failed */
        
        return 1;                                                                       /* return error */
    }
    prev &= (1 << 2);                                                                   /* get enable bit */
    *enable = (l3gd20h_bool_t)(prev >> 2);                                              /* get bool */
  
    return 0;                                                                           /* success return 0 */
}

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
uint8_t l3gd20h_set_fifo_overrun_on_interrupt2(l3gd20h_handle_t *handle, l3gd20h_bool_t enable)
{
    uint8_t res, prev;
    
    if (handle == NULL)                                                                   /* check handle */
    {
        return 2;                                                                         /* return error */
    }
    if (handle->inited != 1)                                                              /* check handle initialization */
    {
        return 3;                                                                         /* return error */
    }
    
    res = a_l3gd20h_iic_spi_read(handle, L3GD20H_REG_CTRL3, (uint8_t *)&prev, 1);         /* read config */
    if (res != 0)                                                                         /* check result */
    {
        handle->debug_print("l3gd20h: read ctrl3 failed.\n");                             /* read ctrl3 failed */
        
        return 1;                                                                         /* return error */
    }
    prev &= ~(1 << 1);                                                                    /* clear enable bit */
    prev |= enable << 1;                                                                  /* set enable */
    
    return a_l3gd20h_iic_spi_write(handle, L3GD20H_REG_CTRL3, (uint8_t *)&prev, 1);       /* write config */
}

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
uint8_t l3gd20h_get_fifo_overrun_on_interrupt2(l3gd20h_handle_t *handle, l3gd20h_bool_t *enable)
{
    uint8_t res, prev;
    
    if (handle == NULL)                                                                 /* check handle */
    {
        return 2;                                                                       /* return error */
    }
    if (handle->inited != 1)                                                            /* check handle initialization */
    {
        return 3;                                                                       /* return error */
    }
    
    res = a_l3gd20h_iic_spi_read(handle, L3GD20H_REG_CTRL3, (uint8_t *)&prev, 1);       /* read config */
    if (res != 0)                                                                       /* check result */
    {
        handle->debug_print("l3gd20h: read ctrl3 failed.\n");                           /* read ctrl3 failed */
        
        return 1;                                                                       /* return error */
    }
    prev &= (1 << 1);                                                                   /* get enable bit */
    *enable = (l3gd20h_bool_t)(prev >> 1);                                              /* get bool */
    
    return 0;                                                                           /* success return 0 */
}

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
uint8_t l3gd20h_set_fifo_empty_on_interrupt2(l3gd20h_handle_t *handle, l3gd20h_bool_t enable)
{
    uint8_t res, prev;
    
    if (handle == NULL)                                                                   /* check handle */
    {
        return 2;                                                                         /* return error */
    }
    if (handle->inited != 1)                                                              /* check handle initialization */
    {
        return 3;                                                                         /* return error */
    }
    
    res = a_l3gd20h_iic_spi_read(handle, L3GD20H_REG_CTRL3, (uint8_t *)&prev, 1);         /* read config */
    if (res != 0)                                                                         /* check result */
    {
        handle->debug_print("l3gd20h: read ctrl3 failed.\n");                             /* read ctrl3 failed */
        
        return 1;                                                                         /* return error */
    }
    prev &= ~(1 << 0);                                                                    /* clear enable bit */
    prev |= enable << 0;                                                                  /* set enable */
    
    return a_l3gd20h_iic_spi_write(handle, L3GD20H_REG_CTRL3, (uint8_t *)&prev, 1);       /* write config */
}

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
uint8_t l3gd20h_get_fifo_empty_on_interrupt2(l3gd20h_handle_t *handle, l3gd20h_bool_t *enable)
{
    uint8_t res, prev;
    
    if (handle == NULL)                                                                 /* check handle */
    {
        return 2;                                                                       /* return error */
    }
    if (handle->inited != 1)                                                            /* check handle initialization */
    {
        return 3;                                                                       /* return error */
    }
    
    res = a_l3gd20h_iic_spi_read(handle, L3GD20H_REG_CTRL3, (uint8_t *)&prev, 1);       /* read config */
    if (res != 0)                                                                       /* check result */
    {
        handle->debug_print("l3gd20h: read ctrl3 failed.\n");                           /* read ctrl3 failed */
        
        return 1;                                                                       /* return error */
    }
    prev &= (1 << 0);                                                                   /* get enable bit */
    *enable = (l3gd20h_bool_t)(prev >> 0);                                              /* get bool */
    
    return 0;                                                                           /* success return 0 */
}

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
uint8_t l3gd20h_set_block_data_update(l3gd20h_handle_t *handle, l3gd20h_bool_t enable)
{
    uint8_t res, prev;
    
    if (handle == NULL)                                                                   /* check handle */
    {
        return 2;                                                                         /* return error */
    }
    if (handle->inited != 1)                                                              /* check handle initialization */
    {
        return 3;                                                                         /* return error */
    }
    
    res = a_l3gd20h_iic_spi_read(handle, L3GD20H_REG_CTRL4, (uint8_t *)&prev, 1);         /* read config */
    if (res != 0)                                                                         /* check result */
    {
        handle->debug_print("l3gd20h: read ctrl4 failed.\n");                             /* read ctrl4 failed */
        
        return 1;                                                                         /* return error */
    }
    prev &= ~(1 << 7);                                                                    /* clear enable bit */
    prev |= enable << 7;                                                                  /* set enable */
  
    return a_l3gd20h_iic_spi_write(handle, L3GD20H_REG_CTRL4, (uint8_t *)&prev, 1);       /* write config */
}

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
uint8_t l3gd20h_get_block_data_update(l3gd20h_handle_t *handle, l3gd20h_bool_t *enable)
{
    uint8_t res, prev;
    
    if (handle == NULL)                                                                 /* check handle */
    {
        return 2;                                                                       /* return error */
    }
    if (handle->inited != 1)                                                            /* check handle initialization */
    {
        return 3;                                                                       /* return error */
    }
    
    res = a_l3gd20h_iic_spi_read(handle, L3GD20H_REG_CTRL4, (uint8_t *)&prev, 1);       /* read config */
    if (res != 0)                                                                       /* check result */
    {
        handle->debug_print("l3gd20h: read ctrl4 failed.\n");                           /* read ctrl4 failed */
        
        return 1;                                                                       /* return error */
    }
    prev &= (1 << 7);                                                                   /* get enable bit */
    *enable = (l3gd20h_bool_t)(prev >> 7);                                              /* get bool */
    
    return 0;                                                                           /* success return 0 */
}

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
uint8_t l3gd20h_set_data_format(l3gd20h_handle_t *handle, l3gd20h_data_format_t data_format)
{
    uint8_t res, prev;
    
    if (handle == NULL)                                                                   /* check handle */
    {
        return 2;                                                                         /* return error */
    }
    if (handle->inited != 1)                                                              /* check handle initialization */
    {
        return 3;                                                                         /* return error */
    }
    
    res = a_l3gd20h_iic_spi_read(handle, L3GD20H_REG_CTRL4, (uint8_t *)&prev, 1);         /* read config */
    if (res != 0)                                                                         /* check result */
    {
        handle->debug_print("l3gd20h: read ctrl4 failed.\n");                             /* read ctrl4 failed */
        
        return 1;                                                                         /* return error */
    }
    prev &= ~(1 << 6);                                                                    /* clear enable bit */
    prev |= data_format << 6;                                                             /* set enable */
  
    return a_l3gd20h_iic_spi_write(handle, L3GD20H_REG_CTRL4, (uint8_t *)&prev, 1);       /* write config */
}

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
uint8_t l3gd20h_get_data_format(l3gd20h_handle_t *handle, l3gd20h_data_format_t *data_format)
{
    uint8_t res, prev;
    
    if (handle == NULL)                                                                 /* check handle */
    {
        return 2;                                                                       /* return error */
    }
    if (handle->inited != 1)                                                            /* check handle initialization */
    {
        return 3;                                                                       /* return error */
    }
    
    res = a_l3gd20h_iic_spi_read(handle, L3GD20H_REG_CTRL4, (uint8_t *)&prev, 1);       /* read config */
    if (res != 0)                                                                       /* check result */
    {
        handle->debug_print("l3gd20h: read ctrl4 failed.\n");                           /* read ctrl4 failed */
        
        return 1;                                                                       /* return error */
    }
    prev &= (1 << 6);                                                                   /* get enable bit */
    *data_format = (l3gd20h_data_format_t)(prev >> 6);                                  /* set enable */
   
    return 0;                                                                           /* success return 0 */
}

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
uint8_t l3gd20h_set_full_scale(l3gd20h_handle_t *handle, l3gd20h_full_scale_t full_scale)
{
    uint8_t res, prev;
    
    if (handle == NULL)                                                                   /* check handle */
    {
        return 2;                                                                         /* return error */
    }
    if (handle->inited != 1)                                                              /* check handle initialization */
    {
        return 3;                                                                         /* return error */
    }
    
    res = a_l3gd20h_iic_spi_read(handle, L3GD20H_REG_CTRL4, (uint8_t *)&prev, 1);         /* read config */
    if (res != 0)                                                                         /* check result */
    {
        handle->debug_print("l3gd20h: read ctrl4 failed.\n");                             /* read ctrl4 failed */
        
        return 1;                                                                         /* return error */
    }
    prev &= ~(3 << 4);                                                                    /* clear the scale bits */
    prev |= full_scale << 4;                                                              /* set scale */
  
    return a_l3gd20h_iic_spi_write(handle, L3GD20H_REG_CTRL4, (uint8_t *)&prev, 1);       /* write config */
}

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
uint8_t l3gd20h_get_full_scale(l3gd20h_handle_t *handle, l3gd20h_full_scale_t *full_scale)
{
    uint8_t res, prev;
    
    if (handle == NULL)                                                                 /* check handle */
    {
        return 2;                                                                       /* return error */
    }
    if (handle->inited != 1)                                                            /* check handle initialization */
    {
        return 3;                                                                       /* return error */
    }
    
    res = a_l3gd20h_iic_spi_read(handle, L3GD20H_REG_CTRL4, (uint8_t *)&prev, 1);       /* read config */
    if (res != 0)                                                                       /* check result */
    {
        handle->debug_print("l3gd20h: read ctrl4 failed.\n");                           /* read ctrl4 failed */
        
        return 1;                                                                       /* return error */
    }
    prev &= (3 << 4);                                                                   /* get scale bits */
    *full_scale = (l3gd20h_full_scale_t)(prev >> 4);                                    /* get scale */
  
    return 0;                                                                           /* success return 0 */
}

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
uint8_t l3gd20h_set_level_sensitive_latched(l3gd20h_handle_t *handle, l3gd20h_bool_t enable)
{
    uint8_t res, prev;
    
    if (handle == NULL)                                                                   /* check handle */
    {
        return 2;                                                                         /* return error */
    }
    if (handle->inited != 1)                                                              /* check handle initialization */
    {
        return 3;                                                                         /* return error */
    }
    
    res = a_l3gd20h_iic_spi_read(handle, L3GD20H_REG_CTRL4, (uint8_t *)&prev, 1);         /* read config */
    if (res != 0)                                                                         /* check result */
    {
        handle->debug_print("l3gd20h: read ctrl4 failed.\n");                             /* read ctrl4 failed */
        
        return 1;                                                                         /* return error */
    }
    prev &= ~(1 << 3);                                                                    /* clear enable bit */
    prev |= enable << 3;                                                                  /* set enable */
  
    return a_l3gd20h_iic_spi_write(handle, L3GD20H_REG_CTRL4, (uint8_t *)&prev, 1);       /* write config */
}

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
uint8_t l3gd20h_get_level_sensitive_latched(l3gd20h_handle_t *handle, l3gd20h_bool_t *enable)
{
    uint8_t res, prev;
    
    if (handle == NULL)                                                                 /* check handle */
    {
        return 2;                                                                       /* return error */
    }
    if (handle->inited != 1)                                                            /* check handle initialization */
    {
        return 3;                                                                       /* return error */
    }
    
    res = a_l3gd20h_iic_spi_read(handle, L3GD20H_REG_CTRL4, (uint8_t *)&prev, 1);       /* read config */
    if (res != 0)                                                                       /* check result */
    {
        handle->debug_print("l3gd20h: read ctrl4 failed.\n");                           /* read ctrl4 failed */
        
        return 1;                                                                       /* return error */
    }
    prev &= (1 << 3);                                                                   /* get enable bit */
    *enable = (l3gd20h_bool_t)(prev >> 3);                                              /* get bool */
  
    return 0;                                                                           /* success return 0 */
}

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
uint8_t l3gd20h_set_self_test(l3gd20h_handle_t *handle, l3gd20h_self_test_t self_test)
{
    uint8_t res, prev;
    
    if (handle == NULL)                                                                   /* check handle */
    {
        return 2;                                                                         /* return error */
    }
    if (handle->inited != 1)                                                              /* check handle initialization */
    {
        return 3;                                                                         /* return error */
    }
    
    res = a_l3gd20h_iic_spi_read(handle, L3GD20H_REG_CTRL4, (uint8_t *)&prev, 1);         /* read config */
    if (res != 0)                                                                         /* check result */
    {
        handle->debug_print("l3gd20h: read ctrl4 failed.\n");                             /* read ctrl4 failed */
        
        return 1;                                                                         /* return error */
    }
    prev &= ~(3 << 1);                                                                    /* clear self test bits */
    prev |= self_test << 1;                                                               /* set self test */
  
    return a_l3gd20h_iic_spi_write(handle, L3GD20H_REG_CTRL4, (uint8_t *)&prev, 1);       /* write config */
}

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
uint8_t l3gd20h_get_self_test(l3gd20h_handle_t *handle, l3gd20h_self_test_t *self_test)
{
    uint8_t res, prev;
    
    if (handle == NULL)                                                                 /* check handle */
    {
        return 2;                                                                       /* return error */
    }
    if (handle->inited != 1)                                                            /* check handle initialization */
    {
        return 3;                                                                       /* return error */
    }
    
    res = a_l3gd20h_iic_spi_read(handle, L3GD20H_REG_CTRL4, (uint8_t *)&prev, 1);       /* read config */
    if (res != 0)                                                                       /* check result */
    {
        handle->debug_print("l3gd20h: read ctrl4 failed.\n");                           /* read ctrl4 failed */
        
        return 1;                                                                       /* return error */
    }
    prev &= (3 << 1);                                                                   /* get self test bits */
    *self_test = (l3gd20h_self_test_t)(prev >> 1);                                      /* get self test */
  
    return 0;                                                                           /* success return 0 */
}

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
uint8_t l3gd20h_set_spi_wire(l3gd20h_handle_t *handle, l3gd20h_spi_wire_t spi_wire)
{
    uint8_t res, prev;
    
    if (handle == NULL)                                                                   /* check handle */
    {
        return 2;                                                                         /* return error */
    }
    if (handle->inited != 1)                                                              /* check handle initialization */
    {
        return 3;                                                                         /* return error */
    }
    
    res = a_l3gd20h_iic_spi_read(handle, L3GD20H_REG_CTRL4, (uint8_t *)&prev, 1);         /* read config */
    if (res != 0)                                                                         /* check result */
    {
        handle->debug_print("l3gd20h: read ctrl4 failed.\n");                             /* read ctrl4 failed */
        
        return 1;                                                                         /* return error */
    }
    prev &= ~(1 << 0);                                                                    /* clear spi wire bit */
    prev |= spi_wire << 0;                                                                /* set spi wire */
  
    return a_l3gd20h_iic_spi_write(handle, L3GD20H_REG_CTRL4, (uint8_t *)&prev, 1);       /* write config */
}

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
uint8_t l3gd20h_get_spi_wire(l3gd20h_handle_t *handle, l3gd20h_spi_wire_t *spi_wire)
{
    uint8_t res, prev;
    
    if (handle == NULL)                                                                 /* check handle */
    {
        return 2;                                                                       /* return error */
    }
    if (handle->inited != 1)                                                            /* check handle initialization */
    {
        return 3;                                                                       /* return error */
    }
    
    res = a_l3gd20h_iic_spi_read(handle, L3GD20H_REG_CTRL4, (uint8_t *)&prev, 1);       /* read config */
    if (res != 0)                                                                       /* check result */
    {
        handle->debug_print("l3gd20h: read ctrl4 failed.\n");                           /* read ctrl4 failed */
        
        return 1;                                                                       /* return error */
    }
    prev &= (1 << 0);                                                                   /* get spi wire bit */
    *spi_wire = (l3gd20h_spi_wire_t)(prev >> 0);                                        /* get spi wire */
    
    return 0;                                                                           /* success return 0 */
}

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
uint8_t l3gd20h_set_boot(l3gd20h_handle_t *handle, l3gd20h_boot_t boot)
{
    uint8_t res, prev;
    
    if (handle == NULL)                                                                   /* check handle */
    {
        return 2;                                                                         /* return error */
    }
    if (handle->inited != 1)                                                              /* check handle initialization */
    {
        return 3;                                                                         /* return error */
    }
    
    res = a_l3gd20h_iic_spi_read(handle, L3GD20H_REG_CTRL5, (uint8_t *)&prev, 1);         /* read config */
    if (res != 0)                                                                         /* check result */
    {
        handle->debug_print("l3gd20h: read ctrl5 failed.\n");                             /* read ctrl5 failed */
        
        return 1;                                                                         /* return error */
    }
    prev &= ~(1 << 7);                                                                    /* clear boot */
    prev |= boot << 7;                                                                    /* set boot */
  
    return a_l3gd20h_iic_spi_write(handle, L3GD20H_REG_CTRL5, (uint8_t *)&prev, 1);       /* write config */
}

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
uint8_t l3gd20h_get_boot(l3gd20h_handle_t *handle, l3gd20h_boot_t *boot)
{
    uint8_t res, prev;
    
    if (handle == NULL)                                                                 /* check handle */
    {
        return 2;                                                                       /* return error */
    }
    if (handle->inited != 1)                                                            /* check handle initialization */
    {
        return 3;                                                                       /* return error */
    }
    
    res = a_l3gd20h_iic_spi_read(handle, L3GD20H_REG_CTRL5, (uint8_t *)&prev, 1);       /* read config */
    if (res != 0)                                                                       /* check result */
    {
        handle->debug_print("l3gd20h: read ctrl5 failed.\n");                           /* read ctrl5 failed */
        
        return 1;                                                                       /* return error */
    }
    prev &= (1 << 7);                                                                   /* get enable bit */
    *boot = (l3gd20h_boot_t)(prev >> 7);                                                /* get bool */
  
    return 0;                                                                           /* success return 0 */
}

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
uint8_t l3gd20h_set_fifo(l3gd20h_handle_t *handle, l3gd20h_bool_t enable)
{
    uint8_t res, prev;
  
    if (handle == NULL)                                                                   /* check handle */
    {
        return 2;                                                                         /* return error */
    }
    if (handle->inited != 1)                                                              /* check handle initialization */
    {
        return 3;                                                                         /* return error */
    }
    
    res = a_l3gd20h_iic_spi_read(handle, L3GD20H_REG_CTRL5, (uint8_t *)&prev, 1);         /* read config */
    if (res != 0)                                                                         /* check result */
    {
        handle->debug_print("l3gd20h: read ctrl5 failed.\n");                             /* read ctrl5 failed */
        
        return 1;                                                                         /* return error */
    }
    prev &= ~(1 << 6);                                                                    /* clear enable bit */
    prev |= enable << 6;                                                                  /* set enable */
  
    return a_l3gd20h_iic_spi_write(handle, L3GD20H_REG_CTRL5, (uint8_t *)&prev, 1);       /* write config */
}

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
uint8_t l3gd20h_get_fifo(l3gd20h_handle_t *handle, l3gd20h_bool_t *enable)
{
    uint8_t res, prev;
    
    if (handle == NULL)                                                                 /* check handle */
    {
        return 2;                                                                       /* return error */
    }
    if (handle->inited != 1)                                                            /* check handle initialization */
    {
        return 3;                                                                       /* return error */
    }
    
    res = a_l3gd20h_iic_spi_read(handle, L3GD20H_REG_CTRL5, (uint8_t *)&prev, 1);       /* read config */
    if (res != 0)                                                                       /* check result */
    {
        handle->debug_print("l3gd20h: read ctrl5 failed.\n");                           /* read ctrl5 failed */
        
        return 1;                                                                       /* return error */
    }
    prev &= (1 << 6);                                                                   /* get enable bit */
    *enable = (l3gd20h_bool_t)(prev >> 6);                                              /* get bool */
  
    return 0;                                                                           /* success return 0 */
}

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
uint8_t l3gd20h_set_stop_on_fifo_threshold(l3gd20h_handle_t *handle, l3gd20h_bool_t enable)
{
    uint8_t res, prev;
  
    if (handle == NULL)                                                                   /* check handle */
    {
        return 2;                                                                         /* return error */
    }
    if (handle->inited != 1)                                                              /* check handle initialization */
    {
        return 3;                                                                         /* return error */
    }
    
    res = a_l3gd20h_iic_spi_read(handle, L3GD20H_REG_CTRL5, (uint8_t *)&prev, 1);         /* read config */
    if (res != 0)                                                                         /* check result */
    {
        handle->debug_print("l3gd20h: read ctrl5 failed.\n");                             /* read ctrl5 failed */
        
        return 1;                                                                         /* return error */
    }
    prev &= ~(1 << 5);                                                                    /* clear fifo enable bit */
    prev |= enable << 5;                                                                  /* set enable */
  
    return a_l3gd20h_iic_spi_write(handle, L3GD20H_REG_CTRL5, (uint8_t *)&prev, 1);       /* write config */
}

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
uint8_t l3gd20h_get_stop_on_fifo_threshold(l3gd20h_handle_t *handle, l3gd20h_bool_t *enable)
{
    uint8_t res, prev;
    
    if (handle == NULL)                                                                 /* check handle */
    {
        return 2;                                                                       /* return error */
    }
    if (handle->inited != 1)                                                            /* check handle initialization */
    {
        return 3;                                                                       /* return error */
    }
    
    res = a_l3gd20h_iic_spi_read(handle, L3GD20H_REG_CTRL5, (uint8_t *)&prev, 1);       /* read config */
    if (res != 0)                                                                       /* check result */
    {
        handle->debug_print("l3gd20h: read ctrl5 failed.\n");                           /* read ctrl5 failed */
        
        return 1;                                                                       /* return error */
    }
    prev &= (1 << 5);                                                                   /* get enable bit */
    *enable = (l3gd20h_bool_t)(prev >> 5);                                              /* get bool */
  
    return 0;                                                                           /* success return 0 */
}

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
uint8_t l3gd20h_set_high_pass_filter(l3gd20h_handle_t *handle, l3gd20h_bool_t enable)
{
    uint8_t res, prev;
  
    if (handle == NULL)                                                                   /* check handle */
    {
        return 2;                                                                         /* return error */
    }
    if (handle->inited != 1)                                                              /* check handle initialization */
    {
        return 3;                                                                         /* return error */
    }
    
    res = a_l3gd20h_iic_spi_read(handle, L3GD20H_REG_CTRL5, (uint8_t *)&prev, 1);         /* read config */
    if (res != 0)                                                                         /* check result */
    {
        handle->debug_print("l3gd20h: read ctrl5 failed.\n");                             /* read ctrl5 failed */
        
        return 1;                                                                         /* return error */
    }
    prev &= ~(1 << 4);                                                                    /* clear enable bit */
    prev |= enable << 4;                                                                  /* set enable */
  
    return a_l3gd20h_iic_spi_write(handle, L3GD20H_REG_CTRL5, (uint8_t *)&prev, 1);       /* write config */
}

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
uint8_t l3gd20h_get_high_pass_filter(l3gd20h_handle_t *handle, l3gd20h_bool_t *enable)
{
    uint8_t res, prev;
    
    if (handle == NULL)                                                                 /* check handle */
    {
        return 2;                                                                       /* return error */
    }
    if (handle->inited != 1)                                                            /* check handle initialization */
    {
        return 3;                                                                       /* return error */
    }
    
    res = a_l3gd20h_iic_spi_read(handle, L3GD20H_REG_CTRL5, (uint8_t *)&prev, 1);       /* read config */
    if (res != 0)                                                                       /* check result */
    {
        handle->debug_print("l3gd20h: read ctrl5 failed.\n");                           /* read ctrl5 failed */
        
        return 1;                                                                       /* return error */
    }
    prev &= (1 << 4);                                                                   /* get enable bit */
    *enable = (l3gd20h_bool_t)(prev >> 4);                                              /* get bool */
  
    return 0;                                                                           /* success return 0 */
}

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
uint8_t l3gd20h_set_interrupt_selection(l3gd20h_handle_t *handle, l3gd20h_selection_t selection)
{
    uint8_t res, prev;
  
    if (handle == NULL)                                                                   /* check handle */
    {
        return 2;                                                                         /* return error */
    }
    if (handle->inited != 1)                                                              /* check handle initialization */
    {
        return 3;                                                                         /* return error */
    }
    
    res = a_l3gd20h_iic_spi_read(handle, L3GD20H_REG_CTRL5, (uint8_t *)&prev, 1);         /* read config */
    if (res != 0)                                                                         /* check result */
    {
        handle->debug_print("l3gd20h: read ctrl5 failed.\n");                             /* read ctrl5 failed */
        
        return 1;                                                                         /* return error */
    }
    prev &= ~(3 << 2);                                                                    /* clear selection */
    prev |= selection << 2;                                                               /* set selection */
  
    return a_l3gd20h_iic_spi_write(handle, L3GD20H_REG_CTRL5, (uint8_t *)&prev, 1);       /* write config */
}

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
uint8_t l3gd20h_get_interrupt_selection(l3gd20h_handle_t *handle, l3gd20h_selection_t *selection)
{
    uint8_t res, prev;
    
    if (handle == NULL)                                                                 /* check handle */
    {
        return 2;                                                                       /* return error */
    }
    if (handle->inited != 1)                                                            /* check handle initialization */
    {
        return 3;                                                                       /* return error */
    }
    
    res = a_l3gd20h_iic_spi_read(handle, L3GD20H_REG_CTRL5, (uint8_t *)&prev, 1);       /* read config */
    if (res != 0)                                                                       /* check result */
    {
        handle->debug_print("l3gd20h: read ctrl5 failed.\n");                           /* read ctrl5 failed */
        
        return 1;                                                                       /* return error */
    }
    prev &= (3 << 2);                                                                   /* get the selection bits */
    *selection = (l3gd20h_selection_t)(prev >> 2);                                      /* get the selection */
  
    return 0;                                                                           /* success return 0 */
}

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
uint8_t l3gd20h_set_out_selection(l3gd20h_handle_t *handle, l3gd20h_selection_t selection)
{
    uint8_t res, prev;
  
    if (handle == NULL)                                                                   /* check handle */
    {
        return 2;                                                                         /* return error */
    }
    if (handle->inited != 1)                                                              /* check handle initialization */
    {
        return 3;                                                                         /* return error */
    }
    
    res = a_l3gd20h_iic_spi_read(handle, L3GD20H_REG_CTRL5, (uint8_t *)&prev, 1);         /* read config */
    if (res != 0)                                                                         /* check result */
    {
        handle->debug_print("l3gd20h: read ctrl5 failed.\n");                             /* read ctrl5 failed */
        
        return 1;                                                                         /* return error */
    }
    prev &= ~(3 << 0);                                                                    /* clear the selection bits */
    prev |= selection << 0;                                                               /* get the selection */
  
    return a_l3gd20h_iic_spi_write(handle, L3GD20H_REG_CTRL5, (uint8_t *)&prev, 1);       /* write config */
}

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
uint8_t l3gd20h_get_out_selection(l3gd20h_handle_t *handle, l3gd20h_selection_t *selection)
{
    uint8_t res, prev;
    
    if (handle == NULL)                                                                 /* check handle */
    {
        return 2;                                                                       /* return error */
    }
    if (handle->inited != 1)                                                            /* check handle initialization */
    {
        return 3;                                                                       /* return error */
    }
    
    res = a_l3gd20h_iic_spi_read(handle, L3GD20H_REG_CTRL5, (uint8_t *)&prev, 1);       /* read config */
    if (res != 0)                                                                       /* check result */
    {
        handle->debug_print("l3gd20h: read ctrl5 failed.\n");                           /* read ctrl5 failed */
        
        return 1;                                                                       /* return error */
    }
    prev &= (3 << 0);                                                                   /* get the selection bits */
    *selection = (l3gd20h_selection_t)(prev >> 0);                                      /* get the selection */
  
    return 0;                                                                           /* success return 0 */
}

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
uint8_t l3gd20h_set_high_pass_filter_reference(l3gd20h_handle_t *handle, uint8_t value)
{
    if (handle == NULL)                                                                        /* check handle */
    {
        return 2;                                                                              /* return error */
    }
    if (handle->inited != 1)                                                                   /* check handle initialization */
    {
        return 3;                                                                              /* return error */
    }
    
    return a_l3gd20h_iic_spi_write(handle, L3GD20H_REG_REFERENCE, (uint8_t *)&value, 1);       /* write config */
}

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
uint8_t l3gd20h_get_high_pass_filter_reference(l3gd20h_handle_t *handle, uint8_t *value)
{
    if (handle == NULL)                                                                        /* check handle */
    {
        return 2;                                                                              /* return error */
    }
    if (handle->inited != 1)                                                                   /* check handle initialization */
    {
        return 3;                                                                              /* return error */
    }
    
    return a_l3gd20h_iic_spi_read(handle, L3GD20H_REG_REFERENCE, (uint8_t *)value, 1);         /* read config */
}

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
uint8_t l3gd20h_read_temperature(l3gd20h_handle_t *handle, int8_t *raw, float *temp)
{
    uint8_t res;
  
    if (handle == NULL)                                                                  /* check handle */
    {
        return 2;                                                                        /* return error */
    }
    if (handle->inited != 1)                                                             /* check handle initialization */
    {
        return 3;                                                                        /* return error */
    }
    
    res = a_l3gd20h_iic_spi_read(handle, L3GD20H_REG_OUT_TEMP, (uint8_t *)raw, 1);       /* read data */
    if (res != 0)                                                                        /* check result */
    {
        handle->debug_print("l3gd20h: read temperature failed.\n");                      /* read temperature failed */
    
        return 1;                                                                        /* return error */
    }
    *temp = (float)(*raw) * (-1.0f) + 25.0f;                                             /* convert the raw data */
  
    return 0;                                                                            /* success return 0 */
}

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
uint8_t l3gd20h_get_status(l3gd20h_handle_t *handle, uint8_t *status)
{
    if (handle == NULL)                                                                  /* check handle */
    {
        return 2;                                                                        /* return error */
    }
    if (handle->inited != 1)                                                             /* check handle initialization */
    {
        return 3;                                                                        /* return error */
    }
    
    return a_l3gd20h_iic_spi_read(handle, L3GD20H_REG_STATUS, (uint8_t *)status, 1);     /* read config */
}

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
uint8_t l3gd20h_set_fifo_mode(l3gd20h_handle_t *handle, l3gd20h_fifo_mode_t fifo_mode)
{
    uint8_t res, prev;
  
    if (handle == NULL)                                                                       /* check handle */
    {
        return 2;                                                                             /* return error */
    }
    if (handle->inited != 1)                                                                  /* check handle initialization */
    {
        return 3;                                                                             /* return error */
    }
  
    res = a_l3gd20h_iic_spi_read(handle, L3GD20H_REG_FIFO_CTRL, (uint8_t *)&prev, 1);         /* read config */
    if (res != 0)                                                                             /* check result */
    {
        handle->debug_print("l3gd20h: read fifo ctrl failed.\n");                             /* read fifo ctrl failed */
   
        return 1;                                                                             /* return error */
    }
    prev &= ~(7 << 5);
    prev |= fifo_mode << 5;                                                                   /* clear fifo mode bits */
                                                                                              /* set fifo mode */
    return a_l3gd20h_iic_spi_write(handle, L3GD20H_REG_FIFO_CTRL, (uint8_t *)&prev, 1);       /* write config */
}

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
uint8_t l3gd20h_get_fifo_mode(l3gd20h_handle_t *handle, l3gd20h_fifo_mode_t *fifo_mode)
{
    uint8_t res, prev;
  
    if (handle == NULL)                                                                     /* check handle */
    {
        return 2;                                                                           /* return error */
    }
    if (handle->inited != 1)                                                                /* check handle initialization */
    {
        return 3;                                                                           /* return error */
    }
  
    res = a_l3gd20h_iic_spi_read(handle, L3GD20H_REG_FIFO_CTRL, (uint8_t *)&prev, 1);       /* read config */
    if (res != 0)                                                                           /* check result */
    {
        handle->debug_print("l3gd20h: read fifo ctrl failed.\n");                           /* read fifo ctrl failed */
   
        return 1;                                                                           /* return error */
    }
    prev &= (7 << 5);                                                                       /* get fifo mode bits */
    *fifo_mode = (l3gd20h_fifo_mode_t)(prev >> 5);                                          /* get fifo mode */
  
    return 0;                                                                               /* success return 0 */
}

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
uint8_t l3gd20h_set_fifo_threshold(l3gd20h_handle_t *handle, uint8_t threshold)
{
    uint8_t res, prev;
  
    if (handle == NULL)                                                                       /* check handle */
    {
        return 2;                                                                             /* return error */
    }
    if (handle->inited != 1)                                                                  /* check handle initialization */
    {
        return 3;                                                                             /* return error */
    }
    
    if (threshold > 31)                                                                       /* check the threshold */
    {
        handle->debug_print("l3gd20h: threshold is invalid.\n");                              /* threshold is invalid */
    
        return 4;                                                                             /* return error */
    }
    res = a_l3gd20h_iic_spi_read(handle, L3GD20H_REG_FIFO_CTRL, (uint8_t *)&prev, 1);         /* read config */
    if (res != 0)                                                                             /* check result */
    {
        handle->debug_print("l3gd20h: read fifo ctrl failed.\n");                             /* read fifo ctrl failed */
    
        return 1;                                                                             /* return error */
    }
    prev &= ~(0x1F);                                                                          /* clear the threshold bits */
    prev |= threshold;                                                                        /* set the threshold */
  
    return a_l3gd20h_iic_spi_write(handle, L3GD20H_REG_FIFO_CTRL, (uint8_t *)&prev, 1);       /* write config */
}

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
uint8_t l3gd20h_get_fifo_threshold(l3gd20h_handle_t *handle, uint8_t *threshold)
{
    uint8_t res, prev;
  
    if (handle == NULL)                                                                     /* check handle */
    {
        return 2;                                                                           /* return error */
    }
    if (handle->inited != 1)                                                                /* check handle initialization */
    {
        return 3;                                                                           /* return error */
    }
  
    res = a_l3gd20h_iic_spi_read(handle, L3GD20H_REG_FIFO_CTRL, (uint8_t *)&prev, 1);       /* read config */
    if (res != 0)                                                                           /* check result */
    {
        handle->debug_print("l3gd20h: read fifo ctrl failed.\n");                           /* read fifo ctrl failed */
    
        return 1;                                                                           /* return error */
    }
    prev &= (0x1F);                                                                         /* get threshold bits */
    *threshold = prev;                                                                      /* set threshold */
  
    return 0;                                                                               /* success return 0 */
}

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
uint8_t l3gd20h_get_fifo_level(l3gd20h_handle_t *handle, uint8_t *level)
{
    uint8_t res, prev;
  
    if (handle == NULL)                                                                    /* check handle */
    {
        return 2;                                                                          /* return error */
    }
    if (handle->inited != 1)                                                               /* check handle initialization */
    {
        return 3;                                                                          /* return error */
    }
  
    res = a_l3gd20h_iic_spi_read(handle, L3GD20H_REG_FIFO_SRC, (uint8_t *)&prev, 1);       /* read config */
    if (res != 0)                                                                          /* check result */
    {
        handle->debug_print("l3gd20h: read fifo src failed.\n");                           /* read fifo src failed */
    
        return 1;                                                                          /* return error */
    }
    prev &= (0x1F);                                                                        /* get level bits */
    *level = prev;                                                                         /* get level */
  
    return 0;                                                                              /* success return 0 */
}

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
uint8_t l3gd20h_set_interrupt_event(l3gd20h_handle_t *handle, l3gd20h_interrupt_event_t interrupt_event, l3gd20h_bool_t enable)
{
    uint8_t res, prev;
  
    if (handle == NULL)                                                                    /* check handle */
    {
        return 2;                                                                          /* return error */
    }
    if (handle->inited != 1)                                                               /* check handle initialization */
    {
        return 3;                                                                          /* return error */
    }
    
    res = a_l3gd20h_iic_spi_read(handle, L3GD20H_REG_IG_CFG, (uint8_t *)&prev, 1);         /* read config */
    if (res != 0)                                                                          /* check result */
    {
        handle->debug_print("l3gd20h: read interrupt cfg failed.\n");                      /* read interrupt cfg failed */
    
        return 1;                                                                          /* return error */
    }
    prev &= ~(1 << interrupt_event);                                                       /* clear event bit */
    prev |= enable << interrupt_event;                                                     /* set event bit */
  
    return a_l3gd20h_iic_spi_write(handle, L3GD20H_REG_IG_CFG, (uint8_t *)&prev, 1);       /* write config */
}

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
uint8_t l3gd20h_get_interrupt_event(l3gd20h_handle_t *handle, l3gd20h_interrupt_event_t interrupt_event, l3gd20h_bool_t *enable)
{
    uint8_t res, prev;
  
    if (handle == NULL)                                                                  /* check handle */
    {
        return 2;                                                                        /* return error */
    }
    if (handle->inited != 1)                                                             /* check handle initialization */
    {
        return 3;                                                                        /* return error */
    }
    
    res = a_l3gd20h_iic_spi_read(handle, L3GD20H_REG_IG_CFG, (uint8_t *)&prev, 1);       /* read config */
    if (res != 0)                                                                        /* check result */
    {
        handle->debug_print("l3gd20h: read interrupt cfg failed.\n");                    /* read interrupt cfg failed */
    
        return 1;                                                                        /* return error */
    }
    prev &= (1 << interrupt_event);                                                      /* get interrupt bit */
    *enable = (l3gd20h_bool_t)(prev >> interrupt_event);                                 /* get interrupt */
  
    return 0;                                                                            /* success return 0 */
}

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
uint8_t l3gd20h_get_interrupt_source(l3gd20h_handle_t *handle, uint8_t *src)
{
    uint8_t res;
  
    if (handle == NULL)                                                                /* check handle */
    {
        return 2;                                                                      /* return error */
    }
    if (handle->inited != 1)                                                           /* check handle initialization */
    {
        return 3;                                                                      /* return error */
    }
    
    res = a_l3gd20h_iic_spi_read(handle, L3GD20H_REG_IG_SRC, (uint8_t *)src, 1);       /* read config */
    if (res != 0)                                                                      /* check result */
    {
        handle->debug_print("l3gd20h: read interrupt source failed.\n");               /* read interrupt source failed */
    
        return 1;                                                                      /* return error */
    }
    
    return 0;                                                                          /* success return 0 */
}

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
uint8_t l3gd20h_set_x_interrupt_threshold(l3gd20h_handle_t *handle, uint16_t threshold)
{
    uint8_t res;
    uint8_t buf[2];
  
    if (handle == NULL)                                                                        /* check handle */
    {
        return 2;                                                                              /* return error */
    }
    if (handle->inited != 1)                                                                   /* check handle initialization */
    {
        return 3;                                                                              /* return error */
    }
    if (threshold > 0x8000U)                                                                   /* check the threshold */
    {
        handle->debug_print("l3gd20h: threshold is invalid.\n");                               /* threshold is invalid */
    
        return 4;                                                                              /* return error */
    }
    
    res = a_l3gd20h_iic_spi_read(handle, L3GD20H_REG_IG_THS_XH, (uint8_t *)buf, 1);            /* read x interrupt threshold */
    if (res != 0)                                                                              /* check result */
    {
        handle->debug_print("l3gd20h: read x interrupt threshold failed.\n");                  /* read x interrupt threshold failed */
    
        return 1;                                                                              /* return error */
    }
    buf[0] = buf[0] | ((threshold >> 8) & 0x7F);                                               /* set threshold high */
    buf[1] = (threshold) & 0xFF;                                                               /* set threshold low*/
  
    res = a_l3gd20h_iic_spi_write(handle, L3GD20H_REG_IG_THS_XH, (uint8_t *)&buf[0], 1);       /* write config */
    if (res != 0)                                                                              /* check result */
    {
        handle->debug_print("l3gd20h: write x interrupt high threshold failed.\n");            /* write x interrupt high threshold failed */
    
        return 1;                                                                              /* return error */
    }
    
    res = a_l3gd20h_iic_spi_write(handle, L3GD20H_REG_IG_THS_XL, (uint8_t *)&buf[1], 1);       /* write config */
    if (res != 0)                                                                              /* check result */
    {
        handle->debug_print("l3gd20h: write x interrupt low threshold failed.\n");             /* write x interrupt low threshold failed */
    
        return 1;                                                                              /* return error */
    }
    
    return 0;                                                                                  /* success return 0 */
}

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
uint8_t l3gd20h_get_x_interrupt_threshold(l3gd20h_handle_t *handle, uint16_t *threshold)
{
    uint8_t buf[2];
  
    if (handle == NULL)                                                                     /* check handle */
    {
        return 2;                                                                           /* return error */
    }
    if (handle->inited != 1)                                                                /* check handle initialization */
    {
        return 3;                                                                           /* return error */
    }
    
    if (a_l3gd20h_iic_spi_read(handle, L3GD20H_REG_IG_THS_XH, (uint8_t *)&buf[0], 1) != 0)  /* read x interrupt high threshold */
    {
        handle->debug_print("l3gd20h: read x interrupt high threshold failed.\n");          /* read x interrupt high threshold failed */
    
        return 1;                                                                           /* return error */
    }
    if (a_l3gd20h_iic_spi_read(handle, L3GD20H_REG_IG_THS_XL, (uint8_t *)&buf[1], 1) != 0)  /* read x interrupt low threshold */
    {
        handle->debug_print("l3gd20h: read x interrupt low threshold failed.\n");           /* read x interrupt low threshold failed */
    
        return 1;                                                                           /* return error */
    }
    *threshold = (uint16_t)((uint16_t)buf[0] << 8) | buf[1];                                /* get the threshold */
    *threshold &= ~(1 << 15);                                                               /* clear the 15th bit */
  
    return 0;                                                                               /* success return 0 */
}

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
uint8_t l3gd20h_set_y_interrupt_threshold(l3gd20h_handle_t *handle, uint16_t threshold)
{
    uint8_t res;
    uint8_t buf[2];
  
    if (handle == NULL)                                                                        /* check handle */
    {
        return 2;                                                                              /* return error */
    }
    if (handle->inited != 1)                                                                   /* check handle initialization */
    {
        return 3;                                                                              /* return error */
    }
    if (threshold > 0x8000U)                                                                   /* check the threshold */
    {
        handle->debug_print("l3gd20h: threshold is invalid.\n");                               /* threshold is invalid */
    
        return 4;                                                                              /* return error */
    }
    
    buf[0] = (threshold >> 8) & 0xFF;                                                          /* set the high threshold */
    buf[1] = (threshold) & 0xFF;                                                               /* set the low threshold */
  
    res = a_l3gd20h_iic_spi_write(handle, L3GD20H_REG_IG_THS_YH, (uint8_t *)&buf[0], 1);       /* write the config */
    if (res != 0)                                                                              /* check result */
    {
        handle->debug_print("l3gd20h: write y interrupt high threshold failed.\n");            /* write y interrupt high threshold failed */
    
        return 1;                                                                              /* return error */
    }
    res = a_l3gd20h_iic_spi_write(handle, L3GD20H_REG_IG_THS_YL, (uint8_t *)&buf[1], 1);       /* write the config */
    if (res != 0)                                                                              /* check result */
    {
        handle->debug_print("l3gd20h: write y interrupt low threshold failed.\n");             /* write y interrupt low threshold failed */
    
        return 1;                                                                              /* return error */
    }
    
    return 0;                                                                                  /* success return 0 */
}

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
uint8_t l3gd20h_get_y_interrupt_threshold(l3gd20h_handle_t *handle, uint16_t *threshold)
{
    uint8_t buf[2];
  
    if (handle == NULL)                                                                     /* check handle */
    {
        return 2;                                                                           /* return error */
    }
    if (handle->inited != 1)                                                                /* check handle initialization */
    {
        return 3;                                                                           /* return error */
    }
    
    if (a_l3gd20h_iic_spi_read(handle, L3GD20H_REG_IG_THS_YH, (uint8_t *)&buf[0], 1) != 0)  /* read y interrupt high threshold */
    {
        handle->debug_print("l3gd20h: read y interrupt high threshold failed.\n");          /* read y interrupt high threshold failed */
    
        return 1;                                                                           /* return error */
    }
    if (a_l3gd20h_iic_spi_read(handle, L3GD20H_REG_IG_THS_YL, (uint8_t *)&buf[1], 1) != 0)  /* read y interrupt low threshold */
    {
        handle->debug_print("l3gd20h: read y interrupt low threshold failed.\n");           /* read y interrupt low threshold failed */
    
        return 1;                                                                           /* return error */
    }
    *threshold = (uint16_t)(buf[0] << 8) | buf[1];                                          /* get the threshold */
    *threshold &= ~(1 << 15);                                                               /* clear the 15th bit */
  
    return 0;                                                                               /* success return 0 */
}

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
uint8_t l3gd20h_set_z_interrupt_threshold(l3gd20h_handle_t *handle, uint16_t threshold)
{
    uint8_t res;
    uint8_t buf[2];
  
    if (handle == NULL)                                                                        /* check handle */
    {
        return 2;                                                                              /* return error */
    }
    if (handle->inited != 1)                                                                   /* check handle initialization */
    {
        return 3;                                                                              /* return error */
    }
    if (threshold > 0x8000U)                                                                   /* check the threshold */
    {
        handle->debug_print("l3gd20h: threshold is invalid.\n");                               /* threshold is invalid */
    
        return 4;                                                                              /* return error */
    }
    
    buf[0] = (threshold >> 8) & 0xFF;                                                          /* set the high threshold */
    buf[1] = (threshold) & 0xFF;                                                               /* set the low threshold */
  
    res = a_l3gd20h_iic_spi_write(handle, L3GD20H_REG_IG_THS_ZH, (uint8_t *)&buf[0], 1);       /* write config */
    if (res != 0)                                                                              /* check result */
    {
        handle->debug_print("l3gd20h: write z interrupt high threshold failed.\n");            /* write z interrupt high threshold failed */
    
        return 1;                                                                              /* return error */
    }
    res = a_l3gd20h_iic_spi_write(handle, L3GD20H_REG_IG_THS_ZL, (uint8_t *)&buf[1], 1);       /* write config */
    if (res != 0)                                                                              /* check result */
    {
        handle->debug_print("l3gd20h: write z interrupt low threshold failed.\n");             /* write z interrupt low threshold failed */
    
        return 1;                                                                              /* return error */
    }
    
    return 0;                                                                                  /* success return 0 */
}

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
uint8_t l3gd20h_get_z_interrupt_threshold(l3gd20h_handle_t *handle, uint16_t *threshold)
{
    uint8_t buf[2];
  
    if (handle == NULL)                                                                     /* check handle */
    {
        return 2;                                                                           /* return error */
    }
    if (handle->inited != 1)                                                                /* check handle initialization */
    {
        return 3;                                                                           /* return error */
    }
    
    if (a_l3gd20h_iic_spi_read(handle, L3GD20H_REG_IG_THS_ZH, (uint8_t *)&buf[0], 1) != 0)  /* read the config */
    {
        handle->debug_print("l3gd20h: read z interrupt high threshold failed.\n");          /* read z interrupt high threshold failed */
    
        return 1;                                                                           /* return error */
    }
    if (a_l3gd20h_iic_spi_read(handle, L3GD20H_REG_IG_THS_ZL, (uint8_t *)&buf[1], 1) != 0)  /* read the config */
    {
        handle->debug_print("l3gd20h: read z interrupt low threshold failed.\n");           /* read z interrupt low threshold failed */
    
        return 1;                                                                           /* return error */
    }
    *threshold = (uint16_t)((uint16_t)buf[0] << 8) | buf[1];                                /* get the threshold */
    *threshold &= ~(1 << 15);                                                               /* clear the 15th bit */
  
    return 0;                                                                               /* success return 0 */
}

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
uint8_t l3gd20h_set_counter_mode(l3gd20h_handle_t *handle, l3gd20h_counter_mode_t counter_mode)
{
    uint8_t res, prev;
  
    if (handle == NULL)                                                                       /* check handle */
    {
        return 2;                                                                             /* return error */
    }
    if (handle->inited != 1)                                                                  /* check handle initialization */
    {
        return 3;                                                                             /* return error */
    }
  
    res = a_l3gd20h_iic_spi_read(handle, L3GD20H_REG_IG_THS_XH, (uint8_t *)&prev, 1);         /* read config */
    if (res != 0)                                                                             /* check result */
    {
        handle->debug_print("l3gd20h: read x interrupt threshold failed.\n");                 /* read x interrupt threshold failed */
    
        return 1;                                                                             /* return error */
    }
    prev &= ~(1 << 7);                                                                        /* clear the counter mode bit */
    prev |= counter_mode << 7;                                                                /* set the counter mode */
  
    return a_l3gd20h_iic_spi_write(handle, L3GD20H_REG_IG_THS_XH, (uint8_t *)&prev, 1);       /* write config */
}

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
uint8_t l3gd20h_get_counter_mode(l3gd20h_handle_t *handle, l3gd20h_counter_mode_t *counter_mode)
{
    uint8_t res, prev;
  
    if (handle == NULL)                                                                     /* check handle */
    {
        return 2;                                                                           /* return error */
    }
    if (handle->inited != 1)                                                                /* check handle initialization */
    {
        return 3;                                                                           /* return error */
    }
  
    res = a_l3gd20h_iic_spi_read(handle, L3GD20H_REG_IG_THS_XH, (uint8_t *)&prev, 1);       /* read config */
    if (res != 0)                                                                           /* check result */
    {
        handle->debug_print("l3gd20h: read x interrupt threshold failed.\n");               /* read x interrupt threshold failed */
    
        return 1;                                                                           /* return error */
    }
    prev &= (1 << 7);                                                                       /* get the counter mode bit */
    *counter_mode = (l3gd20h_counter_mode_t)(prev >> 7);                                    /* get the counter mode */
  
    return 0;                                                                               /* success return 0 */
}

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
uint8_t l3gd20h_set_wait(l3gd20h_handle_t *handle, l3gd20h_bool_t enable)
{
    uint8_t res, prev;
  
    if (handle == NULL)                                                                         /* check handle */
    {
        return 2;                                                                               /* return error */
    }
    if (handle->inited != 1)                                                                    /* check handle initialization */
    {
        return 3;                                                                               /* return error */
    }
  
    res = a_l3gd20h_iic_spi_read(handle, L3GD20H_REG_IG_DURATION, (uint8_t *)&prev, 1);         /* read config */
    if (res != 0)                                                                               /* check result */
    {
        handle->debug_print("l3gd20h: read duration failed.\n");                                /* read duration failed */
    
        return 1;                                                                               /* return error */
    }
    prev &= ~(1 << 7);                                                                          /* clear enable bit */
    prev |= enable << 7;                                                                        /* set enable */
  
    return a_l3gd20h_iic_spi_write(handle, L3GD20H_REG_IG_DURATION, (uint8_t *)&prev, 1);       /* write config */
}

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
uint8_t l3gd20h_get_wait(l3gd20h_handle_t *handle, l3gd20h_bool_t *enable)
{
    uint8_t res, prev;
  
    if (handle == NULL)                                                                       /* check handle */
    {
        return 2;                                                                             /* return error */
    }
    if (handle->inited != 1)                                                                  /* check handle initialization */
    {
        return 3;                                                                             /* return error */
    }
  
    res = a_l3gd20h_iic_spi_read(handle, L3GD20H_REG_IG_DURATION, (uint8_t *)&prev, 1);       /* read config */
    if (res != 0)                                                                             /* check result */
    {
        handle->debug_print("l3gd20h: read duration failed.\n");                              /* read duration failed */
    
        return 1;                                                                             /* return error */
    }
    prev &= (1 << 7);                                                                         /* get the wait bit */
    *enable = (l3gd20h_bool_t)(prev >> 7);                                                    /* get bool */
  
    return 0;                                                                                 /* success return 0 */
}

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
uint8_t l3gd20h_set_duration(l3gd20h_handle_t *handle, uint8_t duration)
{
    uint8_t res, prev;
  
    if (handle == NULL)                                                                         /* check handle */
    {
        return 2;                                                                               /* return error */
    }
    if (handle->inited != 1)                                                                    /* check handle initialization */
    {
        return 3;                                                                               /* return error */
    }
    if (duration > 0x7F)                                                                        /* check the duration */
    {
        handle->debug_print("l3gd20h: duration is over 0x7F.\n");                               /* duration is over 0x7F */
    
        return 1;                                                                               /* return error */
    }
    
    res = a_l3gd20h_iic_spi_read(handle, L3GD20H_REG_IG_DURATION, (uint8_t *)&prev, 1);         /* read config */
    if (res != 0)                                                                               /* check result */
    {
        handle->debug_print("l3gd20h: read duration failed.\n");                                /* read duration failed */
    
        return 1;                                                                               /* return error */
    }
    prev |= duration & 0x7F;                                                                    /* set the duration */
  
    return a_l3gd20h_iic_spi_write(handle, L3GD20H_REG_IG_DURATION, (uint8_t *)&prev, 1);       /* write config */
}

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
uint8_t l3gd20h_get_duration(l3gd20h_handle_t *handle, uint8_t *duration)
{
    uint8_t res, prev;
  
    if (handle == NULL)                                                                       /* check handle */
    {
        return 2;                                                                             /* return error */
    }
    if (handle->inited != 1)                                                                  /* check handle initialization */
    {
        return 3;                                                                             /* return error */
    }
  
    res = a_l3gd20h_iic_spi_read(handle, L3GD20H_REG_IG_DURATION, (uint8_t *)&prev, 1);       /* read config */
    if (res != 0)                                                                             /* check result */
    {
        handle->debug_print("l3gd20h: read duration failed.\n");                              /* read duration failed */
    
        return 1;                                                                             /* return error */
    }
    prev &= 0x7F;                                                                             /* get duration bits */
    *duration = prev;                                                                         /* get duration */
  
    return 0;                                                                                 /* success return 0 */
}

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
uint8_t l3gd20h_set_data_ready_active_level(l3gd20h_handle_t *handle, l3gd20h_interrupt_active_level_t level)
{
    uint8_t res, prev;
  
    if (handle == NULL)                                                                     /* check handle */
    {
        return 2;                                                                           /* return error */
    }
    if (handle->inited != 1)                                                                /* check handle initialization */
    {
        return 3;                                                                           /* return error */
    }
  
    res = a_l3gd20h_iic_spi_read(handle, L3GD20H_REG_LOW_ODR, (uint8_t *)&prev, 1);         /* read config */
    if (res != 0)                                                                           /* check result */
    {
        handle->debug_print("l3gd20h: read low odr failed.\n");                             /* read low odr failed */
    
        return 1;                                                                           /* return error */
    }
    prev &= ~(1 << 5);                                                                      /* clear level bit */
    prev |= level << 5;                                                                     /* set level */
  
    return a_l3gd20h_iic_spi_write(handle, L3GD20H_REG_LOW_ODR, (uint8_t *)&prev, 1);       /* write config */
}

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
uint8_t l3gd20h_get_data_ready_active_level(l3gd20h_handle_t *handle, l3gd20h_interrupt_active_level_t *level)
{
    uint8_t res, prev;
  
    if (handle == NULL)                                                                   /* check handle */
    {
        return 2;                                                                         /* return error */
    }
    if (handle->inited != 1)                                                              /* check handle initialization */
    {
        return 3;                                                                         /* return error */
    }
  
    res = a_l3gd20h_iic_spi_read(handle, L3GD20H_REG_LOW_ODR, (uint8_t *)&prev, 1);       /* read config */
    if (res != 0)                                                                         /* check result */
    {
        handle->debug_print("l3gd20h: read low odr failed.\n");                           /* return error */
    
        return 1;                                                                         /* return error */
    }
    prev &= (1 << 5);                                                                     /* get pin level bit */
    *level = (l3gd20h_interrupt_active_level_t)(prev >> 5);                               /* get pin level */
  
    return 0;                                                                             /* success return 0 */
}

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
uint8_t l3gd20h_set_iic(l3gd20h_handle_t *handle, l3gd20h_bool_t enable)
{
    uint8_t res, prev;
  
    if (handle == NULL)                                                                     /* check handle */
    {
        return 2;                                                                           /* return error */
    }
    if (handle->inited != 1)                                                                /* check handle initialization */
    {
        return 3;                                                                           /* return error */
    }
  
    res = a_l3gd20h_iic_spi_read(handle, L3GD20H_REG_LOW_ODR, (uint8_t *)&prev, 1);         /* read config */
    if (res != 0)                                                                           /* check result */
    {
        handle->debug_print("l3gd20h: read low odr failed.\n");                             /* read low odr failed */
    
        return 1;                                                                           /* return error */
    }
    prev &= ~(1 << 3);                                                                      /* clear enable bit */
    prev |= enable << 3;                                                                    /* set enable */
  
    return a_l3gd20h_iic_spi_write(handle, L3GD20H_REG_LOW_ODR, (uint8_t *)&prev, 1);       /* write config */
}

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
uint8_t l3gd20h_get_iic(l3gd20h_handle_t *handle, l3gd20h_bool_t *enable)
{
    uint8_t res, prev;
  
    if (handle == NULL)                                                                   /* check handle */
    {
        return 2;                                                                         /* return error */
    }
    if (handle->inited != 1)                                                              /* check handle initialization */
    {
        return 3;                                                                         /* return error */
    }
  
    res = a_l3gd20h_iic_spi_read(handle, L3GD20H_REG_LOW_ODR, (uint8_t *)&prev, 1);       /* read config */
    if (res != 0)                                                                         /* check result */
    {
        handle->debug_print("l3gd20h: read low odr failed.\n");                           /* read low odr failed */
    
        return 1;                                                                         /* return error */
    }
    prev &= (1 << 3);                                                                     /* get enable bit */
   *enable = (l3gd20h_bool_t)(prev >> 3);                                                 /* get bool */
  
    return 0;                                                                             /* success return 0 */
}

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
uint8_t l3gd20h_soft_reset(l3gd20h_handle_t *handle)
{
    uint8_t res, prev;
  
    if (handle == NULL)                                                                     /* check handle */
    {
        return 2;                                                                           /* return error */
    }
    if (handle->inited != 1)                                                                /* check handle initialization */
    {
        return 3;                                                                           /* return error */
    }
  
    res = a_l3gd20h_iic_spi_read(handle, L3GD20H_REG_LOW_ODR, (uint8_t *)&prev, 1);         /* read config */
    if (res != 0)                                                                           /* check result */
    {
        handle->debug_print("l3gd20h: read low odr failed.\n");                             /* read low odr failed */
    
        return 1;                                                                           /* return error */
    }
    prev &= ~(1 << 2);                                                                      /* clear reset bit */
    prev |= 1 << 2;                                                                         /* set reset bit */
  
    return a_l3gd20h_iic_spi_write(handle, L3GD20H_REG_LOW_ODR, (uint8_t *)&prev, 1);       /* write config */
}

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
uint8_t l3gd20h_interrupt_threshold_convert_to_register(l3gd20h_handle_t *handle, float dps, uint16_t *reg)
{
    uint8_t range, prev;
  
    if (handle == NULL)                                                               /* check handle */
    {
        return 2;                                                                     /* return error */
    }
    if (handle->inited != 1)                                                          /* check handle initialization */
    {
        return 3;                                                                     /* return error */
    }
  
    if (a_l3gd20h_iic_spi_read(handle, L3GD20H_REG_CTRL4, (uint8_t *)&prev, 1) != 0)  /* read config */
    {
        handle->debug_print("l3gd20h: read ctrl4 failed.\n");                         /* read ctrl4 failed */
    
        return 1;                                                                     /* return error */
    }
    range = (prev & (3 << 4)) >> 4;                                                   /* get range */
    if (range == 0)
    {
        *reg = (uint16_t)(dps * 1000.0f / 7.5f);                                      /* convert */
    }
    else if (range == 1)
    {
        *reg = (uint16_t)(dps * 1000.0f / 15.3f);                                     /* convert */
    }
    else
    {
        *reg = (uint16_t)(dps * 1000.0f / 61.0f);                                     /* convert */
    }
    
    return 0;                                                                         /* success return 0 */
}

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
uint8_t l3gd20h_interrupt_threshold_convert_to_data(l3gd20h_handle_t *handle, uint16_t reg, float *dps)
{
    uint8_t range, prev;
  
    if (handle == NULL)                                                               /* check handle */
    {
        return 2;                                                                     /* return error */
    }
    if (handle->inited != 1)                                                          /* check handle initialization */
    {
        return 3;                                                                     /* return error */
    }
  
    if (a_l3gd20h_iic_spi_read(handle, L3GD20H_REG_CTRL4, (uint8_t *)&prev, 1) != 0)  /* read config */
    {
        handle->debug_print("l3gd20h: read ctrl4 failed.\n");                         /* read ctrl4 failed */
    
        return 1;                                                                     /* return error */
    }
    range = (prev & (3 << 4)) >> 4;                                                   /* get range */
    if (range == 0)
    {
        *dps = (float)(reg * 8.75f / 1000.0f);                                        /* convert */
    }
    else if (range == 1)
    {
        *dps = (float)(reg * 17.50f / 1000.0f);                                       /* convert */
    }
    else
    {
        *dps = (float)(reg * 70.0f / 1000.0f);                                        /* convert */
    }
    
    return 0;                                                                         /* success return 0 */
}

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
uint8_t l3gd20h_irq_handler(l3gd20h_handle_t *handle, uint8_t num)
{
    uint8_t res, prev;
  
    if (handle == NULL)                                                                      /* check handle */
    {
        return 2;                                                                            /* return error */
    }
    if (handle->inited != 1)                                                                 /* check handle initialization */
    {
        return 3;                                                                            /* return error */
    }
    
    if (num == 1)                                                                            /* interrupt 1 */
    {
        res = a_l3gd20h_iic_spi_read(handle, L3GD20H_REG_IG_SRC, (uint8_t *)&prev, 1);       /* read config */
        if (res != 0)                                                                        /* check result */
        {
            handle->debug_print("l3gd20h: read interrupt source failed.\n");                 /* read interrupt source failed */
      
            return 1;                                                                        /* return error */
        }
        if ((prev & (1 << 6)) != 0)                                                          /* check active */
        {
            if (handle->receive_callback != NULL)                                            /* receive callback is valid */
            {
                handle->receive_callback(L3GD20H_INTERRUPT1_INTERRUPT_ACTIVE);               /* run receive callback */
            }
        }
        if ((prev & (1 << 5)) != 0)                                                          /* check z high */
        {
            if (handle->receive_callback != NULL)                                            /* receive callback is valid */
            {
                handle->receive_callback(L3GD20H_INTERRUPT1_Z_HIGH);                         /* run receive callback */
            }
        }
        if ((prev & (1 << 4)) != 0)                                                          /* check z low */
        {
            if (handle->receive_callback != NULL)                                            /* receive callback is valid */
            {
                handle->receive_callback(L3GD20H_INTERRUPT1_Z_LOW);                          /* run receive callback */
            }
        }
        if ((prev & (1 << 3)) != 0)                                                          /* check y high */
        {
            if (handle->receive_callback != NULL)                                            /* receive callback is valid */
            {
                handle->receive_callback(L3GD20H_INTERRUPT1_Y_HIGH);                         /* run receive callback */
            }
        }
        if ((prev & (1 << 2)) != 0)                                                          /* check y low */
        {                                                                                    /* check y low */
            if (handle->receive_callback != NULL)                                            /* receive callback is valid */
            {
                handle->receive_callback(L3GD20H_INTERRUPT1_Y_LOW);                          /* run receive callback */
            }
        }
        if ((prev & (1 << 1)) != 0)                                                          /* check x high */
        {
            if (handle->receive_callback != NULL)                                            /* receive callback is valid */
            {
                handle->receive_callback(L3GD20H_INTERRUPT1_X_HIGH);                         /* run receive callback */
            }
        }
        if ((prev & (1 << 0)) != 0)                                                          /* check x low */
        {
            if (handle->receive_callback != NULL)                                            /* receive callback is valid */
           {
               handle->receive_callback(L3GD20H_INTERRUPT1_X_LOW);                           /* run receive callback */
           }
        }
        
        return 0;                                                                            /* success return 0 */
  }
  else if (num == 2)                                                                         /* interrupt 2 */
  {
      res = a_l3gd20h_iic_spi_read(handle, L3GD20H_REG_STATUS, (uint8_t *)&prev, 1);         /* read config */
      if (res != 0)                                                                          /* check result */
      {
          handle->debug_print("l3gd20h: read status failed.\n");                             /* read status failed */
      
          return 1;                                                                          /* return error */
      }
      if ((prev & (1 << L3GD20H_STATUS_XYZ_OVERRUN)) != 0)                                   /* check status xyz overrun */
      {
          if (handle->receive_callback != NULL)                                              /* receive callback is valid */
          {
              handle->receive_callback(L3GD20H_INTERRUPT2_XYZ_OVERRUN);                      /* run receive callback */
          }
      }
      if ((prev & (1 << L3GD20H_STATUS_Z_OVERRUN)) != 0)                                     /* check status z overrun */
      {
          if (handle->receive_callback != NULL)                                              /* receive callback is valid */
          {
              handle->receive_callback(L3GD20H_INTERRUPT2_Z_OVERRUN);                        /* run receive callback */
          }
      }
      if ((prev & (1 << L3GD20H_STATUS_Y_OVERRUN)) != 0)                                     /* check status y overrun */
      {
          if (handle->receive_callback != NULL)                                              /* receive callback is valid */
          {
              handle->receive_callback(L3GD20H_INTERRUPT2_Y_OVERRUN);                        /* run receive callback */
          }
      }
      if ((prev & (1 << L3GD20H_STATUS_X_OVERRUN)) != 0)                                     /* check status x overrun */
      {
          if (handle->receive_callback != NULL)                                              /* receive callback is valid */
          {
              handle->receive_callback(L3GD20H_INTERRUPT2_X_OVERRUN);                        /* run receive callback */
          }
      }
      if ((prev & (1 << L3GD20H_STATUS_XYZ_DATA_READY)) != 0)                                /* check status xyz data ready */
      {
          if (handle->receive_callback != NULL)                                              /* receive callback is valid */
          {
              handle->receive_callback(L3GD20H_INTERRUPT2_XYZ_DATA_READY);                   /* run receive callback */
          }
      }
      if ((prev & (1 << L3GD20H_STATUS_Z_DATA_READY)) != 0)                                  /* check status z data ready */
      {
          if (handle->receive_callback != NULL)                                              /* receive callback is valid */
          {
              handle->receive_callback(L3GD20H_INTERRUPT2_Z_DATA_READY);                     /* run receive callback */
          }
      }
      if ((prev & (1 << L3GD20H_STATUS_Y_DATA_READY)) != 0)                                  /* check status y data ready */
      {
          if (handle->receive_callback != NULL)                                              /* receive callback is valid */
          {
              handle->receive_callback(L3GD20H_INTERRUPT2_Y_DATA_READY);                     /* run receive callback */
          }
      }
      if ((prev & (1 << L3GD20H_STATUS_X_DATA_READY)) != 0)                                  /* check status x data ready */
      {
          if (handle->receive_callback != NULL)                                              /* receive callback is valid */
          {
              handle->receive_callback(L3GD20H_INTERRUPT2_X_DATA_READY);                     /* run receive callback */
          }
      }

      res = a_l3gd20h_iic_spi_read(handle, L3GD20H_REG_FIFO_SRC, (uint8_t *)&prev, 1);       /* read config */
      if (res != 0)                                                                          /* check result */
      {
          handle->debug_print("l3gd20h: read fifo source failed.\n");                        /* read fifo source failed*/
      
          return 1;                                                                          /* return error */
      }
      if ((prev & (1 << 7)) != 0)                                                            /* check fifo threshold */
      {
          if (handle->receive_callback != NULL)                                              /* receive callback is valid */
          {
              handle->receive_callback(L3GD20H_INTERRUPT2_FIFO_THRESHOLD);                   /* run receive callback */
          }
      }
      if ((prev & (1 << 6)) != 0)                                                            /* check fifo overrun */
      {
          if (handle->receive_callback != NULL)                                              /* receive callback is valid */
          {
              handle->receive_callback(L3GD20H_INTERRUPT2_FIFO_OVERRRUN);                    /* run receive callback */
          }
      }
      if ((prev & (1 << 5)) != 0)                                                            /* check fifo empty */
      {
          if (handle->receive_callback != NULL)                                              /* receive callback is valid */
          {
              handle->receive_callback(L3GD20H_INTERRUPT2_FIFO_EMPTY);                       /* run receive callback */
          }
      }
      
      return 0;                                                                              /* success return 0 */
  }
  else
  {
      handle->debug_print("l3gd20h: interrupt number is invalid.\n");                        /* interrupt number is invalid */
    
      return 1;                                                                              /* return error */
  }
}

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
uint8_t l3gd20h_init(l3gd20h_handle_t *handle)
{
    uint8_t res, prev;
    uint8_t id;
  
    if (handle == NULL)                                                                   /* check handle */
    {
        return 2;                                                                         /* return error */
    }
    if (handle->debug_print == NULL)                                                      /* check debug_print */
    {
        return 3;                                                                         /* return error */
    }
    if (handle->iic_init == NULL)                                                         /* check iic_init */
    {
        handle->debug_print("l3gd20h: iic_init is null.\n");                              /* iic_init is null */
    
        return 3;                                                                         /* return error */
    }
    if (handle->iic_deinit == NULL)                                                       /* check iic_deinit */
    {
        handle->debug_print("l3gd20h: iic_deinit is null.\n");                            /* iic_deinit is null */
        
        return 3;                                                                         /* return error */
    }
    if (handle->iic_read == NULL)                                                         /* check iic_read */
    {
        handle->debug_print("l3gd20h: iic_read is null.\n");                              /* iic_read is null */
    
        return 3;                                                                         /* return error */
    }
    if (handle->iic_write == NULL)                                                        /* check iic_write */
    {
        handle->debug_print("l3gd20h: iic_write is null.\n");                             /* iic_write is null */
    
        return 3;                                                                         /* return error */
    }
    if (handle->spi_init == NULL)                                                         /* check spi_init */
    {
        handle->debug_print("l3gd20h: spi_init is null.\n");                              /* spi_init is null */
    
        return 3;                                                                         /* return error */
    }
    if (handle->spi_deinit == NULL)                                                       /* check spi_deinit */
    {
        handle->debug_print("l3gd20h: spi_deinit is null.\n");                            /* spi_deinit is null */
    
        return 3;                                                                         /* return error */
    }
    if (handle->spi_read == NULL)                                                         /* check spi_read */
    {
        handle->debug_print("l3gd20h: spi_read is null.\n");                              /* spi_read is null */
    
        return 3;                                                                         /* return error */
    }
    if (handle->spi_write == NULL)                                                        /* check spi_write */
    {
        handle->debug_print("l3gd20h: spi_write is null.\n");                             /* spi_write is null */
    
        return 3;                                                                         /* return error */
    }
    if (handle->delay_ms == NULL)                                                         /* check delay_ms */
    {
        handle->debug_print("l3gd20h: delay_ms is null.\n");                              /* delay_ms is null */
    
        return 3;                                                                         /* return error */
    }
    
    if (handle->iic_spi == L3GD20H_INTERFACE_IIC)                                         /* iic interface */
    {
        if (handle->iic_init() != 0)                                                      /* initialize iic bus */
        {
            handle->debug_print("l3gd20h: iic init failed.\n");                           /* iic init failed */
      
            return 1;                                                                     /* return error */
        }
    }
    else
    {
        if (handle->spi_init() != 0)                                                      /* initialize spi bus */
        {
            handle->debug_print("l3gd20h: spi init failed.\n");                           /* spi init failed */
      
            return 1;                                                                     /* return error */
        }
    }
    if (a_l3gd20h_iic_spi_read(handle, L3GD20H_REG_WHO_AM_I, (uint8_t *)&id, 1) != 0)     /* read id */
    {
        handle->debug_print("l3gd20h: read id failed.\n");                                /* read id failed */
        if (handle->iic_spi == L3GD20H_INTERFACE_IIC)                                     /* if iic interface */
        {
            (void)handle->iic_deinit();                                                   /* iic deinit */
        }
        else                                                                              /* spi interface */
        {
            (void)handle->spi_deinit();                                                   /* spi deinit */
        }
        
        return 1;                                                                         /* return error */
    }
    if (id != 0xD7)                                                                       /* check id */
    {
        handle->debug_print("l3gd20h: id is invalid.\n");                                 /* id is invalid */
        if (handle->iic_spi == L3GD20H_INTERFACE_IIC)                                     /* if iic interface */
        {
            (void)handle->iic_deinit();                                                   /* iic deinit */
        }
        else
        {
            (void)handle->spi_deinit();                                                   /* spi deinit */
        }
        
        return 4;                                                                         /* return error */
    }
    
    res = a_l3gd20h_iic_spi_read(handle, L3GD20H_REG_LOW_ODR, (uint8_t *)&prev, 1);       /* read config */
    if (res != 0)                                                                         /* check result */
    {
        handle->debug_print("l3gd20h: read low odr failed.\n");                           /* read low odr failed */
        if (handle->iic_spi == L3GD20H_INTERFACE_IIC)                                     /* if iic interface */
        {                                                                               
            (void)handle->iic_deinit();                                                   /* iic deinit */
        }
        else
        {
            (void)handle->spi_deinit();                                                   /* spi deinit */
        }
    
        return 1;                                                                         /* return error */
    }
    prev &= ~(1 << 2);                                                                    /* clear reset bit */
    prev |= 1 << 2;                                                                       /* set reset bit */
    res = a_l3gd20h_iic_spi_write(handle, L3GD20H_REG_LOW_ODR, (uint8_t *)&prev, 1);      /* write config */
    if (res != 0)                                                                         /* check result */
    {
        handle->debug_print("l3gd20h: write low odr failed.\n");                          /* write low odr failed */
        if (handle->iic_spi == L3GD20H_INTERFACE_IIC)                                     /* if iic interface */
        {
            (void)handle->iic_deinit();                                                   /* iic deinit */
        }
        else
        {
            (void)handle->spi_deinit();                                                   /* spi deinit */
        }
        
        return 1;                                                                         /* return error */
    }
    handle->delay_ms(12);                                                                 /* delay 12 ms */
    res = a_l3gd20h_iic_spi_read(handle, L3GD20H_REG_LOW_ODR, (uint8_t *)&prev, 1);       /* read config */
    if (res != 0)                                                                         /* check result */
    {
        handle->debug_print("l3gd20h: read low odr failed.\n");                           /* read low odr failed */
        if (handle->iic_spi == L3GD20H_INTERFACE_IIC)                                     /* if iic interface */
        {
            (void)handle->iic_deinit();                                                   /* iic deinit */
        }
        else
        {
            (void)handle->spi_deinit();                                                   /* spi deinit */
        }
    
        return 1;                                                                         /* return error */
    }
    if (((prev >> 2) & 0x01) != 0x0)                                                      /* check result */
    {
        handle->debug_print("l3gd20h: reset chip failed.\n");                             /* reset chip failed */
        if (handle->iic_spi == L3GD20H_INTERFACE_IIC)                                     /* if iic interface */
        {
            (void)handle->iic_deinit();                                                   /* iic deinit */
        }
        else
        {
            (void)handle->spi_deinit();                                                   /* spi deinit */
        }
    
        return 1;                                                                         /* return error */
    }
    
    handle->inited = 1;                                                                   /* flag finish initialization */
  
    return 0;                                                                             /* success return 0 */
}

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
uint8_t l3gd20h_deinit(l3gd20h_handle_t *handle)
{
    uint8_t res, prev;
    
    if (handle == NULL)                                                                  /* check handle */
    {
        return 2;                                                                        /* return error */
    }
    if (handle->inited != 1)                                                             /* check handle initialization */
    {
        return 3;                                                                        /* return error */
    }
    
    res = a_l3gd20h_iic_spi_read(handle, L3GD20H_REG_CTRL1, (uint8_t *)&prev, 1);        /* read config */
    if (res != 0)                                                                        /* check result */
    {
        handle->debug_print("l3gd20h: read ctrl1 failed.\n");                            /* read ctrl1 failed */
        
        return 4;                                                                        /* return error */
    }
    prev &= ~(1 << 3);                                                                   /* set power down bit */
    res = a_l3gd20h_iic_spi_write(handle, L3GD20H_REG_CTRL1, (uint8_t *)&prev, 1);       /* write config */
    if (res != 0)                                                                        /* check result */
    {
        handle->debug_print("l3gd20h: write ctrl1 failed.\n");                           /* write ctrl1 failed */
        
        return 4;                                                                        /* return error */
    }
    handle->inited = 0;                                                                  /* flag close */
    
    return 0;                                                                            /* success return 0 */
}

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
uint8_t l3gd20h_read(l3gd20h_handle_t *handle, int16_t (*raw)[3], float (*dps)[3], uint16_t *len) 
{
    uint8_t res, prev;
    uint8_t mode, cnt, i;
    uint8_t ble, range, enable;
    uint8_t buf[32 * 6];
  
    if (handle == NULL)                                                                              /* check handle */
    {
        return 2;                                                                                    /* return error */
    }
    if (handle->inited != 1)                                                                         /* check handle initialization */
    {
        return 3;                                                                                    /* return error */
    }

    if ((*len) == 0)                                                                                 /* check length */
    {
        handle->debug_print("l3gd20h: length is zero.\n");                                           /* length is zero. */
    
        return 4;                                                                                    /* return error */
    }
    if (a_l3gd20h_iic_spi_read(handle, L3GD20H_REG_FIFO_CTRL, (uint8_t *)&prev, 1) != 0)             /* read fifo ctrl */
    {
        handle->debug_print("l3gd20h: read fifo ctrl failed.\n");                                    /* read fifo ctrl failed */
    
        return 1;                                                                                    /* return error */
    }
    mode = prev >> 5;                                                                                /* get the mode */
    if (a_l3gd20h_iic_spi_read(handle, L3GD20H_REG_CTRL5, (uint8_t *)&prev, 1) != 0)                 /* read ctrl5 */
    {
        handle->debug_print("l3gd20h: read ctrl5 failed.\n");                                        /* read ctrl5 failed */
    
        return 1;                                                                                    /* return error */
    }
    enable = (prev & (1 << 6)) >> 6;                                                                 /* get enable */
    if (a_l3gd20h_iic_spi_read(handle, L3GD20H_REG_CTRL4, (uint8_t *)&prev, 1) != 0)                 /* get ctrl4 */
    {
        handle->debug_print("l3gd20h: read ctrl4 failed.\n");                                        /* read ctrl4 failed */
    
        return 1;                                                                                    /* return error */
    }
    range = (prev & (3 << 4)) >> 4;                                                                  /* get range */
    ble = (prev & (1 << 6)) >> 6;                                                                    /* get big little endian */
    if ((mode && enable) != 0)                                                                       /* fifo modes */
    {
        res = a_l3gd20h_iic_spi_read(handle, L3GD20H_REG_FIFO_SRC, (uint8_t *)&prev, 1);             /* read fifo source */
        if (res != 0)                                                                                /* check result */
        {
            handle->debug_print("l3gd20h: read fifo source failed.\n");                              /* read fifo source failed */
      
            return 1;                                                                                /* return error */
        }
        cnt = prev & 0x1F;                                                                           /* get counter */
        *len = ((*len) < cnt) ? (*len) : cnt;                                                        /* get the length */
        res = a_l3gd20h_iic_spi_read(handle, L3GD20H_REG_OUT_X_L, (uint8_t *)buf, 6 * (*len));       /* read all data */
        if (res != 0)                                                                                /* check result */
        {
            handle->debug_print("l3gd20h: read data failed.\n");                                     /* read data failed */
      
            return 1;                                                                                /* return error */
        }
        for (i = 0; i < (*len); i++)                                                                 /* get data */
        {
            if (ble == 0)                                                                            /* little endian */
            {
                raw[i][0] = (int16_t)(((uint16_t)buf[1 + i * 6] << 8) | buf[0 + i * 6]);             /* set x */
                raw[i][1] = (int16_t)(((uint16_t)buf[3 + i * 6] << 8) | buf[2 + i * 6]);             /* set y */
                raw[i][2] = (int16_t)(((uint16_t)buf[5 + i * 6] << 8) | buf[4 + i * 6]);             /* set z */
            }
            else                                                                                     /* big endian */
            {
                raw[i][0] = (int16_t)(((uint16_t)buf[0 + i * 6] << 8) | buf[1 + i * 6]);             /* set x */
                raw[i][1] = (int16_t)(((uint16_t)buf[2 + i * 6] << 8) | buf[3 + i * 6]);             /* set y */
                raw[i][2] = (int16_t)(((uint16_t)buf[4 + i * 6] << 8) | buf[5 + i * 6]);             /* set z */
            }
           if (range == 0)                                                                           /* ±245 dps */
           {
               dps[i][0] = (float)(raw[i][0]) * 8.75f / 1000.0f;                                     /* set x */
               dps[i][1] = (float)(raw[i][1]) * 8.75f / 1000.0f;                                     /* set y */
               dps[i][2] = (float)(raw[i][2]) * 8.75f / 1000.0f;                                     /* set z */
           }
           else if (range == 1)                                                                      /* ±500 dps */
           {
               dps[i][0] = (float)(raw[i][0]) * 17.5f / 1000.0f;                                     /* set x */
               dps[i][1] = (float)(raw[i][1]) * 17.5f / 1000.0f;                                     /* set y */
               dps[i][2] = (float)(raw[i][2]) * 17.5f / 1000.0f;                                     /* set z */
           }
           else                                                                                      /* ±2000 dps */
           {
               dps[i][0] = (float)(raw[i][0]) * 70.0f / 1000.0f;                                     /* set x */
               dps[i][1] = (float)(raw[i][1]) * 70.0f / 1000.0f;                                     /* set y */
               dps[i][2] = (float)(raw[i][2]) * 70.0f / 1000.0f;                                     /* set z */
           }
       }
    }                                                                                                /* bypass mode */
    else
    {
        *len = 1;                                                                                    /* set length */
        res = a_l3gd20h_iic_spi_read(handle, L3GD20H_REG_OUT_X_L, (uint8_t *)buf, 6);                /* read data */
        if (res != 0)                                                                                /* check result */
        {
            handle->debug_print("l3gd20h: read data failed.\n");                                     /* read data failed */
      
            return 1;                                                                                /* return error */
        }
        if (ble == 0)                                                                                /* little endian */
        {
            raw[0][0] = (int16_t)(((uint16_t)buf[1] << 8) | buf[0]);                                 /* set x */
            raw[0][1] = (int16_t)(((uint16_t)buf[3] << 8) | buf[2]);                                 /* set y */
            raw[0][2] = (int16_t)(((uint16_t)buf[5] << 8) | buf[4]);                                 /* set z */
        }
        else                                                                                         /* big endian */
        {
            raw[0][0] = (int16_t)(((uint16_t)buf[0] << 8) | buf[1]);                                 /* set x */
            raw[0][1] = (int16_t)(((uint16_t)buf[2] << 8) | buf[3]);                                 /* set y */
            raw[0][2] = (int16_t)(((uint16_t)buf[4] << 8) | buf[5]);                                 /* set z */
        }
        if (range == 0)                                                                              /* ±245 dps */
        {
            dps[0][0] = (float)(raw[0][0]) * 8.75f / 1000.0f;                                        /* set x */
            dps[0][1] = (float)(raw[0][1]) * 8.75f / 1000.0f;                                        /* set y */
            dps[0][2] = (float)(raw[0][2]) * 8.75f / 1000.0f;                                        /* set z */
        }
        else if (range == 1)                                                                         /* ±500 dps */
        {
            dps[0][0] = (float)(raw[0][0]) * 17.5f / 1000.0f;                                        /* set x */
            dps[0][1] = (float)(raw[0][1]) * 17.5f / 1000.0f;                                        /* set y */
            dps[0][2] = (float)(raw[0][2]) * 17.5f / 1000.0f;                                        /* set z */
        }
        else                                                                                         /* ±2000 dps */
        {
            dps[0][0] = (float)(raw[0][0]) * 70.0f / 1000.0f;                                        /* set x */
            dps[0][1] = (float)(raw[0][1]) * 70.0f / 1000.0f;                                        /* set y */
            dps[0][2] = (float)(raw[0][2]) * 70.0f / 1000.0f;                                        /* set z */
        }
     }
  
     return 0;                                                                                       /* success return 0 */
}

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
uint8_t l3gd20h_set_reg(l3gd20h_handle_t *handle, uint8_t reg, uint8_t *buf, uint16_t len)
{
    if (handle == NULL)                                          /* check handle */
    {
        return 2;                                                /* return error */
    }
    if (handle->inited != 1)                                     /* check handle initialization */
    {
        return 3;                                                /* return error */
    }
  
    return a_l3gd20h_iic_spi_write(handle, reg, buf, len);       /* write data */
}

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
uint8_t l3gd20h_get_reg(l3gd20h_handle_t *handle, uint8_t reg, uint8_t *buf, uint16_t len)
{
    if (handle == NULL)                                         /* check handle */
    {
        return 2;                                               /* return error */
    }
    if (handle->inited != 1)                                    /* check handle initialization */
    {
        return 3;                                               /* return error */
    }
  
    return a_l3gd20h_iic_spi_read(handle, reg, buf, len);       /* read data */
}

/**
 * @brief      get chip's information
 * @param[out] *info points to an l3gd20h info structure
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 * @note       none
 */
uint8_t l3gd20h_info(l3gd20h_info_t *info)
{
    if (info == NULL)                                               /* check handle */
    {
        return 2;                                                   /* return error */
    }
    
    memset(info, 0, sizeof(l3gd20h_info_t));                        /* initialize l3gd20h info structure */
    strncpy(info->chip_name, CHIP_NAME, 32);                        /* copy chip name */
    strncpy(info->manufacturer_name, MANUFACTURER_NAME, 32);        /* copy manufacturer name */
    strncpy(info->interface, "IIC SPI", 8);                         /* copy interface name */
    info->supply_voltage_min_v = SUPPLY_VOLTAGE_MIN;                /* set minimal supply voltage */
    info->supply_voltage_max_v = SUPPLY_VOLTAGE_MAX;                /* set maximum supply voltage */
    info->max_current_ma = MAX_CURRENT;                             /* set maximum current */
    info->temperature_max = TEMPERATURE_MAX;                        /* set minimal temperature */
    info->temperature_min = TEMPERATURE_MIN;                        /* set maximum temperature */
    info->driver_version = DRIVER_VERSION;                          /* set driver version */
    
    return 0;                                                       /* success return 0 */
}
