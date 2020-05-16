/*****************************************************************************/
/*  Filename: main_config.h                                                  */
/*  Feature:  Main                                                           */
/*  Module:   Main                                                           */
/*                                                                           */
/*  ********************* Ltd. owns the copyright in this                    */
/*  and associated documents and all rights are reserved.                    */
/*  This document and associated documents must not be                       */
/*  used for any purpose other than that for which they                      */
/*  are supplied and must not be copied in whole or in                       */
/*  part, or disclosed to others without the prior                           */
/*  written consent of **************************  Any copy                  */
/*  of this document made by any method must also contain a                  */
/*  copy of this legend.                                                     */
/*                                                                           */
/*  (c) ********************* LTD. 2020                                      */
/*****************************************************************************/

#ifndef MAIN_CONFIG_H_
#define MAIN_CONFIG_H_

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

/*!
 *  @brief Device I2C Adrress
 */
#define LCD_ADDRESS             (0x7c>>1)
#define RGB_ADDRESS             (0xc0>>1)
#define SLAVE_ADDR               0x66

typedef enum
{
    FALSE = 0,
    TRUE
} boolean;

void delay_ms(unsigned int delay);

#endif /* MAIN_CONFIG_H_ */
