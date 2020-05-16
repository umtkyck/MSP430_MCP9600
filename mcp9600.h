/*****************************************************************************/
/*  Filename: mcp9600.h                                                      */
/*  Feature:  Temperature Sensor                                             */
/*  Module:   Temperature Sensor                                             */
/*                                                                           */
/*  ********************* Ltd. owns the copyright in this                    */
/*  and associated documents and all rights are reserved.                    */
/*  This document and associated documents must not be                       */
/*  used for any purpose other than that for which they                      */
/*  are supplied and must not be copied in whole or in                       */
/*  part, or disclosed to others without the prior                           */
/*  written consent of Delphi Diesel Systems Ltd.  Any copy                  */
/*  of this document made by any method must also contain a                  */
/*  copy of this legend.                                                     */
/*                                                                           */
/*  (c) ********************* LTD. 2020                                      */
/*****************************************************************************/

#ifndef MCP9600_H_
#define MCP9600_H_

#include "main_config.h"

/**** Global Definitions *****************************/
#define HOT_JUNCTION_REG_ADDR               0X0
#define JUNCTION_TEMP_DELTA_REG_ADDR        0X1
#define COLD_JUNCTION_TEMP_REG_ADDR         0X2
#define RAW_ADC_DATA_REG_ADDR               0X3
#define STAT_REG_ADDR                       0X4
#define THERM_SENS_CFG_REG_ADDR             0X5
#define DEVICE_CFG_REG_ADDR                 0X6

#define ALERT1_CFG_REG_ADDR                 0X8
#define ALERT2_CFG_REG_ADDR                 0X9
#define ALERT3_CFG_REG_ADDR                 0XA
#define ALERT4_CFG_REG_ADDR                 0XB
#define ALERT1_HYS_REG_ADDR                 0XC
#define ALERT2_HYS_REG_ADDR                 0XD
#define ALERT3_HYS_REG_ADDR                 0XE
#define ALERT4_HYS_REG_ADDR                 0XF

#define TEMP_ALERT1_LIMIT_REG_ADDR          0X10
#define TEMP_ALERT2_LIMIT_REG_ADDR          0X11
#define TEMP_ALERT3_LIMIT_REG_ADDR          0X12
#define TEMP_ALERT4_LIMIT_REG_ADDR          0X13

#define VERSION_ID_REG_ADDR                 0x20

#define RESOLUTION_0_5_DEGREE               0
#define RESOLUTION_0_25_DEGREE              0X01
#define RESOLUTION_0_125_DEGREE             0X02
#define RESOLUTION_0_0625_DEGREE            0X03

#define THER_TYPE_K                         0X0<<4
#define THER_TYPE_J                         0X1<<4
#define THER_TYPE_T                         0X2<<4
#define THER_TYPE_N                         0X3<<4
#define THER_TYPE_S                         0X4<<4
#define THER_TYPE_E                         0X5<<4
#define THER_TYPE_B                         0X6<<4
#define THER_TYPE_R                         0X7<<4

#define ALERT_NUN_1                         1
#define ALERT_NUN_2                         2
#define ALERT_NUN_3                         3
#define ALERT_NUN_4                         4

#define FILT_OFF                            0
#define FILT_MIN                            1
#define FILT_MID                            4
#define FILT_MAX                            7

#define ENABLE   true
#define DISABLE  false

#define COLD_JUNC_RESOLUTION_0_625          0<<7
#define COLD_JUNC_RESOLUTION_0_25           1<<7

#define ADC_18BIT_RESOLUTION                0<<5
#define ADC_16BIT_RESOLUTION                1<<5
#define ADC_14BIT_RESOLUTION                2<<5
#define ADC_12BIT_RESOLUTION                3<<5

#define BURST_1_SAMPLE                      0<<2
#define BURST_2_SAMPLE                      1<<2
#define BURST_4_SAMPLE                      2<<2
#define BURST_8_SAMPLE                      3<<2
#define BURST_16_SAMPLE                     4<<2
#define BURST_32_SAMPLE                     5<<2
#define BURST_64_SAMPLE                     6<<2
#define BURST_128_SAMPLE                    7<<2

#define NORMAL_OPERATION                    0
#define SHUTDOWN_MODE                       1
#define BURST_MODE                          2

#define ACTIVE_LOW                          0<<2
#define ACTIVE_HIGH                         1<<2

#define INT_MODE                            1<<1
#define COMPARE_MODE                        0<<1

#define UPDATE_FLAG                         1<<6


/**** Global Function Prototypes *********************/
float readADC(uint8_t *data);
void set_filt_coefficients(uint8_t set_byte);
void set_ADC_meas_resolution(uint8_t set_byte);
void set_cold_junc_resolution(uint8_t set_byte);
void set_burst_mode_samp(uint8_t set_byte);
void set_sensor_mode(uint8_t set_byte);


#endif /* MCP9600_H_ */
