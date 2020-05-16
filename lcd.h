/*****************************************************************************/
/*  Filename: lcd.h                                                          */
/*  Feature:  LCD                                                            */
/*  Module:   LCD                                                            */
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
#ifndef LCD_H_
#define LCD_H_

#include "main_config.h"

/*!
 *  @brief color define
 */
#define WHITE           0
#define RED             1
#define GREEN           2
#define BLUE            3

#define REG_RED         0x04        // pwm2
#define REG_GREEN       0x03        // pwm1
#define REG_BLUE        0x02        // pwm0

#define REG_MODE1       0x00
#define REG_MODE2       0x01
#define REG_OUTPUT      0x08

/*!
 *  @brief commands
 */
#define LCD_CLEARDISPLAY 0x01
#define LCD_RETURNHOME 0x02
#define LCD_ENTRYMODESET 0x04
#define LCD_DISPLAYCONTROL 0x08
#define LCD_CURSORSHIFT 0x10
#define LCD_FUNCTIONSET 0x20
#define LCD_SETCGRAMADDR 0x40
#define LCD_SETDDRAMADDR 0x80

/*!
 *  @brief flags for display entry mode
 */
#define LCD_ENTRYRIGHT 0x00
#define LCD_ENTRYLEFT 0x02
#define LCD_ENTRYSHIFTINCREMENT 0x01
#define LCD_ENTRYSHIFTDECREMENT 0x00

/*!
 *  @brief flags for display on/off control
 */
#define LCD_DISPLAYON 0x04
#define LCD_DISPLAYOFF 0x00
#define LCD_CURSORON 0x02
#define LCD_CURSOROFF 0x00
#define LCD_BLINKON 0x01
#define LCD_BLINKOFF 0x00

/*!
 *  @brief flags for display/cursor shift
 */
#define LCD_DISPLAYMOVE 0x08
#define LCD_CURSORMOVE 0x00
#define LCD_MOVERIGHT 0x04
#define LCD_MOVELEFT 0x00

/*!
 *  @brief flags for function set
 */
#define LCD_8BITMODE 0x10
#define LCD_4BITMODE 0x00
#define LCD_2LINE 0x08
#define LCD_1LINE 0x00
#define LCD_5x10DOTS 0x04
#define LCD_5x8DOTS 0x00

enum rows
{
    FIRST_LINE = 0,
    SECOND_LINE
};

enum columns
{
    COL_0 = 0,
    COL_1,
    COL_2,
    COL_3,
    COL_4,
    COL_5,
    COL_6,
    COL_7,
    COL_8,
    COL_9,
    COL_10,
    COL_11,
    COL_12,
    COL_13,
    COL_14,
    COL_15
};

/**** Global Definitions ****/
uint8_t _showfunction;
uint8_t _showcontrol;
uint8_t _showmode;
uint8_t _initialized;
uint8_t _lcdAddr;
uint8_t _RGBAddr;
uint8_t _backlightval;
uint8_t _cols;
uint8_t _rows;
uint8_t _numlines;
uint8_t _currline;


/**** Global Function Prototypes ****/

void setReg(uint8_t addr, uint8_t data);
void LCD_init(void);
void setRGB(uint8_t r, uint8_t g, uint8_t b);
void clear(void);
void setColorWhite(void);
void command(uint8_t value);
void write_char(uint8_t value);
void cursor_off(void);
void cursor_on(void);
void setCursor(uint8_t col, uint8_t row);
void leftToRight(void);
void rightToLeft(void);
void display_off(void);
void display_on(void);
void blinkLED_on(void);
void blinkLED_off(void);
uint8_t update_lcd(uint8_t rows, uint8_t cols,
                    uint8_t byteCtr, uint8_t *data);
void lcd_test(void);



#endif /* LCD_H_ */
