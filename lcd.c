/*****************************************************************************/
/*  Filename: lcd.c                                                          */
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

#include "lcd.h"
#include "i2c.h"
#include <msp430g2553.h>


/*****************************************************************************/
/*                                SYNOPSIS                                   */
/*                                --------                                   */
/*   Function Return Value :-                                                */
/*   ------------------------                                                */
/*   None                                                                    */
/*                                                                           */
/*   Function Specification:-                                                */
/*   -----------------------                                                 */
/*   Initialize the LCD and Backlight                                        */
/*****************************************************************************/
void LCD_init(void)
{
    _showfunction = LCD_4BITMODE | LCD_1LINE | LCD_5x8DOTS;

    const int lines = 2;
    const int dotsize = 0;

    if (lines > 1) {
         _showfunction |= LCD_2LINE;
     }
     _numlines = lines;
     _currline = 0;

     ///< for some 1 line displays you can select a 10 pixel high font
     if ((dotsize != 0) && (lines == 1)) {
         _showfunction |= LCD_5x10DOTS;
     }

     ///< SEE PAGE 45/46 FOR INITIALIZATION SPECIFICATION!
     ///< according to datasheet, we need at least 40ms after power rises above 2.7V
     ///< before sending commands. Arduino can turn on way befer 4.5V so we'll wait 50
     delay_ms(50);


     ///< this is according to the hitachi HD44780 datasheet
     ///< page 45 figure 23

     ///< Send function set command sequence
     command(LCD_FUNCTIONSET | _showfunction);
     delay_ms(5);  // wait more than 4.1ms

     ///< second try
     command(LCD_FUNCTIONSET | _showfunction);
     delay_ms(5);

     ///< third go
     command(LCD_FUNCTIONSET | _showfunction);

     ///< turn the display on with no cursor or blinking default
     _showcontrol = LCD_DISPLAYON | LCD_CURSOROFF | LCD_BLINKOFF;
     display_on();

     ///< clear it off
     clear();

     ///< Initialize to default text direction (for romance languages)
     _showmode = LCD_ENTRYLEFT | LCD_ENTRYSHIFTDECREMENT;
     ///< set the entry mode
     command(LCD_ENTRYMODESET | _showmode);


     ///< backlight init
     setReg(REG_MODE1, 0);
     ///< set LEDs controllable by both PWM and GRPPWM registers
     setReg(REG_OUTPUT, 0xFF);
     ///< set MODE2 values
     ///< 0010 0000 -> 0x20  (DMBLNK to 1, ie blinky mode)
     setReg(REG_MODE2, 0x20);

     setColorWhite();

}

/*****************************************************************************/
/*                                SYNOPSIS                                   */
/*                                --------                                   */
/*   Function Return Value :-                                                */
/*   ------------------------                                                */
/*   None                                                                    */
/*                                                                           */
/*   Function Specification:-                                                */
/*   -----------------------                                                 */
/*   Generic Test Routine                                                    */
/*****************************************************************************/
void lcd_test(void)
{
    /* Cursor for tracking the LCD
         * prototype: setCursor(col2,row2)
         */
        setCursor(3,1);
        delay_ms(50);

        /* Added for test purposes, you close it */
        cursor_on();
        delay_ms(50);

        /* Text alignment */
        leftToRight();

        /* Sample Text on the screen */
        write_char('T');
        write_char('E');
        write_char('M');
        write_char('P');
        write_char(':');
        write_char('2');
        write_char('5');
        write_char(223);
        write_char('C');

        /* Cursor on bottom line
         * prototype: setCursor(col2,row2)
         */
        setCursor(3,0); // col2, row2
        //rightToLeft();
        write_char('T');
        write_char('E');
        write_char('M');
        write_char('P');
        write_char(':');
        write_char('2');
        write_char('5');
        write_char(223);
        write_char('C');
}

/*****************************************************************************/
/*                                SYNOPSIS                                   */
/*                                --------                                   */
/*   Function Return Value :-                                                */
/*   ------------------------                                                */
/*   None                                                                    */
/*                                                                           */
/*   Function Specification:-                                                */
/*   -----------------------                                                 */
/*   Clears the LCD screen                                                   */
/*****************************************************************************/
void clear(void)
{
    //UCB0I2CSA = LCD_ADDRESS;

    command(LCD_CLEARDISPLAY);        // clear display, set cursor position to zero
    delay_ms(5);                      // this command takes a long time!
}

/*****************************************************************************/
/*                                SYNOPSIS                                   */
/*                                --------                                   */
/*   Function Return Value :-                                                */
/*   ------------------------                                                */
/*   None                                                                    */
/*                                                                           */
/*   Function Specification:-                                                */
/*   -----------------------                                                 */
/*   Can be used for scrolling to LEFT purposes                              */
/*****************************************************************************/
void scrollDisplayLeft(void)
{
    command(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVELEFT);
}

/*****************************************************************************/
/*                                SYNOPSIS                                   */
/*                                --------                                   */
/*   Function Return Value :-                                                */
/*   ------------------------                                                */
/*   None                                                                    */
/*                                                                           */
/*   Function Specification:-                                                */
/*   -----------------------                                                 */
/*   Can be used for scrolling to RIGHT purposes                             */
/*****************************************************************************/
void scrollDisplayRight(void)
{
    command(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVERIGHT);
}

/*****************************************************************************/
/*                                SYNOPSIS                                   */
/*                                --------                                   */
/*   Function Return Value :-                                                */
/*   ------------------------                                                */
/*   None                                                                    */
/*                                                                           */
/*   Function Specification:-                                                */
/*   -----------------------                                                 */
/*   Display on function needs to show your texts                            */
/*****************************************************************************/
void display_on(void)
{
    //UCB0I2CSA = LCD_ADDRESS;

    _showcontrol |= LCD_DISPLAYON;
    command(LCD_DISPLAYCONTROL | _showcontrol);
}

/*****************************************************************************/
/*                                SYNOPSIS                                   */
/*                                --------                                   */
/*   Function Return Value :-                                                */
/*   ------------------------                                                */
/*   None                                                                    */
/*                                                                           */
/*   Function Specification:-                                                */
/*   -----------------------                                                 */
/*   Display off function needs to make invisible your texts                 */
/*****************************************************************************/
void display_off()
{
    _showcontrol &= ~LCD_DISPLAYON;
    command(LCD_DISPLAYCONTROL | _showcontrol);
}

/*****************************************************************************/
/*                                SYNOPSIS                                   */
/*                                --------                                   */
/*   Function Return Value :-                                                */
/*   ------------------------                                                */
/*   None                                                                    */
/*                                                                           */
/*   Function Specification:-                                                */
/*   -----------------------                                                 */
/*   LCD Driver package sender, but not for backlight driver take care!      */
/*****************************************************************************/
void command(uint8_t value)
{
    // Slave address shall be updated
    //UCB0I2CSA = LCD_ADDRESS;

    //uint8_t data[3] = {0x80, value};

    //I2C_write(2, &data);
    uint8_t MasterType[1];

    MasterType[0] = value;

    I2C_Master_WriteReg(LCD_ADDRESS, 0x80, MasterType, 1);
}

/*****************************************************************************/
/*                                SYNOPSIS                                   */
/*                                --------                                   */
/*   Function Return Value :-                                                */
/*   ------------------------                                                */
/*   None                                                                    */
/*                                                                           */
/*   Function Specification:-                                                */
/*   -----------------------                                                 */
/*   Prints character on the screen where cursor is waiting                  */
/*****************************************************************************/
void write_char(uint8_t value)
{
    //uint8_t data[3] = {0x40, value};
    //I2C_write(2, &data);

    uint8_t MasterType[1];

    MasterType[0] = value;

    I2C_Master_WriteReg(LCD_ADDRESS, 0x40, MasterType, 1);
}

/*****************************************************************************/
/*                                SYNOPSIS                                   */
/*                                --------                                   */
/*   Function Return Value :-                                                */
/*   ------------------------                                                */
/*   None                                                                    */
/*                                                                           */
/*   Function Specification:-                                                */
/*   -----------------------                                                 */
/*   Text alignment for latine languages                                     */
/*****************************************************************************/
void leftToRight(void)
{
    _showmode |= LCD_ENTRYLEFT;
    command(LCD_ENTRYMODESET | _showmode);
}

/*****************************************************************************/
/*                                SYNOPSIS                                   */
/*                                --------                                   */
/*   Function Return Value :-                                                */
/*   ------------------------                                                */
/*   None                                                                    */
/*                                                                           */
/*   Function Specification:-                                                */
/*   -----------------------                                                 */
/*   Text alignment for arabian languages                                    */
/*****************************************************************************/
void rightToLeft(void)
{
    _showmode &= ~LCD_ENTRYLEFT;
    command(LCD_ENTRYMODESET | _showmode);
}

/*****************************************************************************/
/*                                SYNOPSIS                                   */
/*                                --------                                   */
/*   Function Return Value :-                                                */
/*   ------------------------                                                */
/*   None                                                                    */
/*                                                                           */
/*   Function Specification:-                                                */
/*   -----------------------                                                 */
/*   Set cursor location                                                     */
/*****************************************************************************/
void setCursor(uint8_t col, uint8_t row)
{
    //UCB0I2CSA = LCD_ADDRESS;

    col = (row == 0 ? col|0x80 : col|0xc0);
    /*uint8_t data[3] = {0x80, col};

    I2C_write(2, &data);*/

    uint8_t MasterType[1];

    MasterType[0] = col;

    I2C_Master_WriteReg(LCD_ADDRESS, 0x80, MasterType, 1);
}

/*****************************************************************************/
/*                                SYNOPSIS                                   */
/*                                --------                                   */
/*   Function Return Value :-                                                */
/*   ------------------------                                                */
/*   None                                                                    */
/*                                                                           */
/*   Function Specification:-                                                */
/*   -----------------------                                                 */
/*   Cursor starts to blink                                                  */
/*****************************************************************************/
void cursor_on(void)
{
    _showcontrol |= LCD_CURSORON;
    command(LCD_DISPLAYCONTROL | _showcontrol);
}

/*****************************************************************************/
/*                                SYNOPSIS                                   */
/*                                --------                                   */
/*   Function Return Value :-                                                */
/*   ------------------------                                                */
/*   None                                                                    */
/*                                                                           */
/*   Function Specification:-                                                */
/*   -----------------------                                                 */
/*   Cursor stops to blink, will be invisible                                */
/*****************************************************************************/
void cursor_off(void)
{
    _showcontrol &= ~LCD_CURSORON;
    command(LCD_DISPLAYCONTROL | _showcontrol);
}


/*****************************************************************************/
/*                                SYNOPSIS                                   */
/*                                --------                                   */
/*   Function Return Value :-                                                */
/*   ------------------------                                                */
/*   None                                                                    */
/*                                                                           */
/*   Function Specification:-                                                */
/*   -----------------------                                                 */
/*   White color background as defined with arduino                          */
/*****************************************************************************/
void setColorWhite(void)
{
    setRGB(255, 255, 255);
}

/*****************************************************************************/
/*                                SYNOPSIS                                   */
/*                                --------                                   */
/*   Function Return Value :-                                                */
/*   ------------------------                                                */
/*   None                                                                    */
/*                                                                           */
/*   Function Specification:-                                                */
/*   -----------------------                                                 */
/*   Set the backlight color                                                 */
/*****************************************************************************/
void setRGB(uint8_t r, uint8_t g, uint8_t b)
{
    setReg(REG_RED, r);
    setReg(REG_GREEN, g);
    setReg(REG_BLUE, b);
}

/*****************************************************************************/
/*                                SYNOPSIS                                   */
/*                                --------                                   */
/*   Function Return Value :-                                                */
/*   ------------------------                                                */
/*   None                                                                    */
/*                                                                           */
/*   Function Specification:-                                                */
/*   -----------------------                                                 */
/*   Register setting for backlight driver IC                                */
/*****************************************************************************/
void setReg(uint8_t addr, uint8_t data)
{
    //UCB0I2CSA = RGB_ADDRESS;    // Slave Address passed as parameter

    /*uint8_t TxData[2];
    TxData[0]= addr;  // start address
    TxData[1]= data;  // data

    I2C_write(2, &TxData);*/

    uint8_t MasterType[1];

    MasterType[0] = data;

    I2C_Master_WriteReg(RGB_ADDRESS, addr, MasterType, 1);


}

/*****************************************************************************/
/*                                SYNOPSIS                                   */
/*                                --------                                   */
/*   Function Return Value :-                                                */
/*   ------------------------                                                */
/*   None                                                                    */
/*                                                                           */
/*   Function Specification:-                                                */
/*   -----------------------                                                 */
/*   Backlight will start blink with this routine                            */
/*****************************************************************************/
void blinkLED_on(void)
{
    ///< blink period in seconds = (<reg 7> + 1) / 24
    ///< on/off ratio = <reg 6> / 256
    setReg(0x07, 0x17);  // blink every second
    setReg(0x06, 0x7f);  // half on, half off
}

/*****************************************************************************/
/*                                SYNOPSIS                                   */
/*                                --------                                   */
/*   Function Return Value :-                                                */
/*   ------------------------                                                */
/*   None                                                                    */
/*                                                                           */
/*   Function Specification:-                                                */
/*   -----------------------                                                 */
/*   Backlight stops blinking                                                */
/*****************************************************************************/
void blinkLED_off(void)
{
    setReg(0x07, 0x00);
    setReg(0x06, 0xff);
}

/*****************************************************************************/
/*                                SYNOPSIS                                   */
/*                                --------                                   */
/*   Function Return Value :-                                                */
/*   ------------------------                                                */
/*   0xFF -> Fail                                                            */
/*   0x00 -> Success                                                         */
/*                                                                           */
/*   Function Specification:-                                                */
/*   -----------------------                                                 */
/*   Backlight stops blinking                                                */
/*****************************************************************************/
uint8_t update_lcd(uint8_t rows, uint8_t cols,
                    uint8_t byteCtr, uint8_t *data)
{
    uint8_t i;
    uint8_t do_not_update =0;

    char *StringData;

    if(byteCtr>16)
        return 0xFF; // fail

    // Clears the screen
    //clear();
    delay_ms(50);

    setCursor(cols, rows);
    //delay_ms(10);
    //cursor_on();
    //delay_ms(10);

    StringData = (char *) data;



    for (i=0; i<byteCtr; i++)
    {
        //if(*StringData == NULL)
        if (!(((data[i] <='9') && (data[i] >='0')) || (data[i] =='.')))
        {
            do_not_update = 1;
            delay_ms(10);
        }
    }

    I2C_Master_WriteReg(LCD_ADDRESS, 0x40, data, byteCtr);

/*
    if (do_not_update != 1)
    {
        for (i=0; i<byteCtr; i++)
        {
                write_char(*StringData);    // write the character to screen
                StringData++;               // increment the array idx
                delay_ms(2);
        }
    }*/


    return 0; // success
}
