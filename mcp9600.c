/*****************************************************************************/
/*  Filename: mcp9600.c                                                      */
/*  Feature:  Temperature Sensor                                             */
/*  Module:   Temperature Sensor                                             */
/*                                                                           */
/*  ********************* Ltd. owns the copyright in this                    */
/*  and associated documents and all rights are reserved.                    */
/*  This document and associated documents must not be                       */
/*  used for any purpose other than that for which they                      */
/*  are supplied and must not be copied in whole or in                       */
/*  part, or disclosed to others without the prior                           */
/*  written consent of ********************* Ltd.  Any copy                  */
/*  of this document made by any method must also contain a                  */
/*  copy of this legend.                                                     */
/*                                                                           */
/*  (c) ********************* LTD. 2020                                      */
/*****************************************************************************/

#include "mcp9600.h"
#include "i2c.h"
#include <msp430g2553.h>

/* External glabal variables */
extern uint8_t ReceiveBuffer[20];

/** @brief read hot-junction,the temperature result.
    @param value: result.
    @return 0 if successed.
 * */
float readADC(uint8_t *data)
{
    float temp;

    //Serial.printf("Data[0] = %i, Data[1] = %i \n", data[0], data[1]);
//  (UpperByte x 16 + LowerByte/16) – 4096
    if((data[0] & 0x80) == 0x80)
    {
        //data[0] = data[0] & 0x7F;
        //temp = 1024 - (data[0] *16.00 + data[1]/16.00);
        temp = (data[0] *16.00 + data[1]/16.00)-4096;
        /*if(fahrenheit){
            temp = (temp *1.8)+32;
        }*/
        return temp;
    }

    temp = data[0]*16.00 + data[1]/16.00;

    /*if(fahrenheit)
    {
        temp = (temp *1.8)+32;
    }*/

    return temp;
}

/** @brief set_filt_coefficients
    The  content of filt-coefficients register are:code-junction-resolution/ADC measurement resolution/burst mode temp samples/sensor mode
    @param set_byte: the byte to be set.
    @return 0 if successed.
 * */
void set_filt_coefficients(uint8_t set_byte)
{
    uint8_t therm_cfg_data = 0;
    uint8_t byte_to_set = 0;
    uint8_t MasterType[1];

    I2C_Master_ReadReg(SLAVE_ADDR, THERM_SENS_CFG_REG_ADDR, 1);
    CopyArray(ReceiveBuffer, therm_cfg_data, 1);

    delay_ms(25);

    byte_to_set = (therm_cfg_data & 0xf8) | set_byte;

    MasterType[0] = byte_to_set;
    I2C_Master_WriteReg(SLAVE_ADDR, THERM_SENS_CFG_REG_ADDR, MasterType, 1);
}

/** @brief set_cold_junc_resolution
    @param set_byte: the byte to be set.
    @return 0 if successed.
 * */
void set_cold_junc_resolution(uint8_t set_byte)
{
    uint8_t therm_cfg_data = 0;
    uint8_t byte_to_set = 0;
    uint8_t MasterType[1];

    I2C_Master_ReadReg(SLAVE_ADDR, DEVICE_CFG_REG_ADDR, 1);
    CopyArray(ReceiveBuffer, therm_cfg_data, 1);

    delay_ms(25);

    byte_to_set = (therm_cfg_data & 0x7f) | set_byte;
    MasterType[0] = byte_to_set;
    I2C_Master_WriteReg(SLAVE_ADDR, DEVICE_CFG_REG_ADDR, MasterType, 1);
}

/** @brief set_ADC_meas_resolution
    @param set_byte: the byte to be set.
    @return 0 if successed.
 * */
void set_ADC_meas_resolution(uint8_t set_byte)
{
    uint8_t therm_cfg_data = 0;
    uint8_t byte_to_set = 0;
    uint8_t MasterType[1];

    I2C_Master_ReadReg(SLAVE_ADDR, DEVICE_CFG_REG_ADDR, 1);
    CopyArray(ReceiveBuffer, therm_cfg_data, 1);

    delay_ms(25);

    byte_to_set = (therm_cfg_data & 0x9f) | set_byte;
    MasterType[0] = byte_to_set;
    I2C_Master_WriteReg(SLAVE_ADDR, DEVICE_CFG_REG_ADDR, MasterType, 1);
}

/** @brief set_burst_mode_samp
    @param set_byte: the byte to be set.
    @return 0 if successed.
 * */
void set_burst_mode_samp(uint8_t set_byte)
{
    uint8_t therm_cfg_data = 0;
    uint8_t byte_to_set = 0;
    uint8_t MasterType[1];

    I2C_Master_ReadReg(SLAVE_ADDR, DEVICE_CFG_REG_ADDR, 1);
    CopyArray(ReceiveBuffer, therm_cfg_data, 1);

    delay_ms(25);

    byte_to_set = (therm_cfg_data & 0xe3) | set_byte;
    MasterType[0] = byte_to_set;
    I2C_Master_WriteReg(SLAVE_ADDR, DEVICE_CFG_REG_ADDR, MasterType, 1);
}

/** @brief set_sensor_mode
    @param set_byte: the byte to be set.
    @return 0 if successed.
 * */
void set_sensor_mode(uint8_t set_byte)
{
    uint8_t therm_cfg_data = 0;
    uint8_t byte_to_set = 0;
    uint8_t MasterType[1];

    I2C_Master_ReadReg(SLAVE_ADDR, DEVICE_CFG_REG_ADDR, 1);
    CopyArray(ReceiveBuffer, therm_cfg_data, 1);

    delay_ms(25);

    byte_to_set = (therm_cfg_data & 0xfc) | set_byte;
    MasterType[0] = byte_to_set;
    I2C_Master_WriteReg(SLAVE_ADDR, DEVICE_CFG_REG_ADDR, MasterType, 1);
}
