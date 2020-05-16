#include "main.h"
#include "i2c.h"
#include "lcd.h"
#include "mcp9600.h"
#include <msp430.h>
#include <math.h>
#include <stdint.h>
#include <debug.h>
#include <usci_settings.h>

typedef unsigned short uint16;
typedef unsigned long uint32;

#define TRUE 1
#define FALSE 0

char uart_buf[200];
int bnum;
const unsigned RXD = BIT1;
const unsigned TXD = BIT2;

/* External glabal variables */
extern I2C_Mode MasterMode;
extern uint8_t RXByteCtr;
extern uint8_t ReceiveBuffer[20];
extern uint8_t ReceiveIndex;
extern uint8_t TransmitBuffer[20];
extern uint8_t TXByteCtr;
extern uint8_t TransmitIndex;

uint8_t MasterType0 [TYPE_0_LENGTH] = { 11};
uint8_t STATIC_LABEL[] = {'T','e','m','p','e','r', 'a', 't', 'u', 'r', 'e'};
uint8_t SUCCESS_LABEL[] = {'S', 'U', 'C', 'C', 'E', 'S', 'S',' ', ' ', ' '};

//******************************************************************************
// Device Initialization *******************************************************
//******************************************************************************
/*****************************************************************************/
/*                                SYNOPSIS                                   */
/*                                --------                                   */
/*   Function Return Value :-                                                */
/*   ------------------------                                                */
/*   None                                                                    */
/*                                                                           */
/*   Function Specification:-                                                */
/*   -----------------------                                                 */
/*   Initial CLK settings                                                    */
/*****************************************************************************/
void initClockTo16MHz(void)
{
    if (CALBC1_16MHZ==0xFF)                  // If calibration constant erased
    {
        while(1);                               // do not load, trap CPU!!
    }
    DCOCTL = 0;                               // Select lowest DCOx and MODx settings
    BCSCTL1 = CALBC1_16MHZ;                    // Set DCO
    DCOCTL = CALDCO_16MHZ;
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
/*   Main IO and I2C comm initialization                                     */
/*****************************************************************************/
void initGPIO(void)
{
    P1DIR |= BIT0 + BIT1 + BIT2 + BIT3 + BIT4;
    P1OUT &= ~(BIT0 + BIT1 + BIT2 + BIT3 + BIT4);

    P1SEL |= BIT6 + BIT7;                     // Assign I2C pins to USCI_B0
    P1SEL2|= BIT6 + BIT7;                     // Assign I2C pins to USCI_B0

    P1SEL |= BIT2;
    P1SEL2 |=BIT2;
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
/*   Main IO and I2C comm initialization                                     */
/*   I2C baudrate is 100khz                                                  */
/*   Assign initial slave address                                            */
/*****************************************************************************/
void initI2C(void)
{
    UCB0CTL1 |= UCSWRST;                      // Enable SW reset
    UCB0CTL0 = UCMST + UCMODE_3 + UCSYNC;     // I2C Master, synchronous mode
    UCB0CTL1 = UCSSEL_2 + UCSWRST;            // Use SMCLK, keep SW reset
    //UCB0BR0 = 160;                            // fSCL = SMCLK/160 = ~100kHz
    //UCB0BR1 = 0;

    /* 50khz SCK */
    UCB0BR0 = 0x40; 
    UCB0BR1 = 0x1;
    /* Bit clock prescaler setting. The 16-bit value of (UCBxBR0 + UCBxBR1 × 256) */

    UCB0I2CSA = SLAVE_ADDR;                   // Slave Address
    UCB0CTL1 &= ~UCSWRST;                     // Clear SW reset, resume operation
    UCB0I2CIE |= UCNACKIE;
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
/*   Main IO and uart comm initialization                                     */
/*   uart baudrate is 115200                                                  */
/*   Assign initial slave address                                            */
/*****************************************************************************/
void initUART(void)
{
    /* UART LCD Conn */
    P1SEL |=BIT1;
    P1SEL |=BIT2;
    P1SEL2 |=BIT1;
    P1SEL2 |= BIT2;
    UCA0CTL1 |= UCSSEL_2 + UCSWRST;
    UCA0BR0 = USCI_BR0_VAL;
    UCA0BR1 = USCI_BR1_VAL;
    UCA0MCTL = USCI_BRS_VAL;
    UCA0CTL1 &= ~UCSWRST;
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
/*   Converts inter 0-9 to its ASCII equivalent                              */
/*****************************************************************************/
int intToAscii(int number)
{
   return '0' + number;
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
/*   Delay in miliseconds, tested with Saleae                                */
/*****************************************************************************/
void delay_ms(unsigned int delay)
{
    while (delay--)
    {
        __delay_cycles(16000);  //1ms = 1000 cycles per 1Mhz clock freq.
    }
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
/*   Converts float to its ASCII equivalent                              */
/*****************************************************************************/
void flot(char* p, float x)
{
    int n, i=0, k=0;
    n=(int)x;

    while(n>0)
    {
        x/=10;
        n=(int)x;
        i++;
    }

    //*(p+i) = '.';
    x *= 10;
    n = (int)x;
    x = x-n;

    while((n>0)||(i>k))
    {
        if(k == i)
            k++;
        *(p+k)='0'+n;
        x *= 10;
        n = (int)x;
        x = x-n;
        k++;
    }

    /* Null-terminated string */
    //*(p+k) = '\0';
}

// Maximum buffer size needed
#define ITOA_BASE_N (sizeof(unsigned)*CHAR_BIT + 2)

void itoa_base(char *s, int x, int base) {
  s += ITOA_BASE_N - 1;
  *s = '\0';
  if (base >= 2 && base <= 36) {
    int x0 = x;
    do {
      *(--s) = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ"[abs(x % base)];
      x /= base;
    } while (x);
    if (x0 < 0) {
      *(--s) = '-';
    }
  }
  return s;
}

#define TO_BASE(x,b) itoa_base((char [ITOA_BASE_N]){0} , (x), (b))

//*****************************************************************************/
// Main ***********************************************************************/
//*****************************************************************************/
int main(void)
{
    float cur_temp=0;
    uint16 lowres=0;
    int lowres_2=0;
    float prev_temp=0;
    float normal=0;
    float rounded_down=0;
    int truncated=0;
    uint8_t i=0, success=0, update=0, weird=0;
    uint8_t version[3]={0};
    uint8_t status[1]={0};
    uint8_t adc_val[4]={0};
    uint8_t hot_junc[2]={0};
    uint8_t hot_junc_t[2]={0};
    uint8_t cold_junc[4]={0};
    uint8_t delta_junc[4]={0};
    char s[10];
    char temperature[10];
    char a[10]={};
    char* p=&a;

    /* Tags for the code */
    //_cols = 16;
    //_rows = 2;

    WDTCTL = WDTPW | WDTHOLD;   // Stop watchdog timer
                                // Setup I/O for UART and switch
     
     //   P1SEL = P1SEL2 = TXD;
    initClockTo16MHz();
    initGPIO();
    initI2C();
    initUART();

    /* LCD Initialization routine according to datasheet */
    LCD_init();
    delay_ms(100);

    I2C_Master_ReadReg(SLAVE_ADDR, VERSION_ID_REG_ADDR, 1); //verison id
    CopyArray(ReceiveBuffer, version, 1);

    I2C_Master_ReadReg(SLAVE_ADDR, STAT_REG_ADDR, 1);       // status
    CopyArray(ReceiveBuffer, status, 1);

    MasterType0[0] = 0x0; // reset sensor configuration
    I2C_Master_WriteReg(SLAVE_ADDR, THERM_SENS_CFG_REG_ADDR, MasterType0, 1);
    delay_ms(100);

    /* MCP Settings */
    set_filt_coefficients(FILT_MID);
    delay_ms(100);
    set_cold_junc_resolution(COLD_JUNC_RESOLUTION_0_625);
    delay_ms(100);
    set_ADC_meas_resolution(ADC_16BIT_RESOLUTION);
    delay_ms(100);
    set_burst_mode_samp(BURST_32_SAMPLE);
    delay_ms(100);
    set_sensor_mode(NORMAL_OPERATION);
    delay_ms(100);

    /* lcd_clear */
    clear();

    // Tag the first static label
    update_lcd(FIRST_LINE, COL_0, 11, &STATIC_LABEL);

    while(1)
    {
        update = 0;

        I2C_Master_ReadReg(SLAVE_ADDR, STAT_REG_ADDR, 1); // status
        CopyArray(ReceiveBuffer, status, 1);

        status[0] = status[0] & 0x3F; // clear burst mode and adc conversion completed flags.

        // Update the status register with cleaned flags
        I2C_Master_WriteReg(SLAVE_ADDR, STAT_REG_ADDR, status, 1);

        success = 0;
        while(!success)
        {
            I2C_Master_ReadReg(SLAVE_ADDR, STAT_REG_ADDR, 1); // status
            CopyArray(ReceiveBuffer, status, 1);

            if (status[0] & 0x40) // convertion completed?
            {
                // Trig the read adc register
                I2C_Master_ReadReg(SLAVE_ADDR, HOT_JUNCTION_REG_ADDR, 2);
                CopyArray(ReceiveBuffer, hot_junc, 2);

                while ((hot_junc[0] ==0x01) && (hot_junc[1] ==0x01))
                {
                    // continue reading adc register, until returns valid value
                    I2C_Master_ReadReg(SLAVE_ADDR, HOT_JUNCTION_REG_ADDR, 2);
                    CopyArray(ReceiveBuffer, hot_junc, 2);
                    delay_ms(30);
                }
                // got the value
                success = 1;
            }
            else
            {
                success = 0;
            }
        }

        // handle real temp data
        hot_junc_t[0] = hot_junc[0];
        hot_junc_t[1] = hot_junc[1];

        // calculates the hot_junc temp
        cur_temp = readADC(&hot_junc);

        lowres_2 = (int)cur_temp;

        memset(s,' ',10);
        memset(temperature,' ',10);

        sprintf(s,"%d", lowres_2);

        for (i=0; i<10; i++)
        {
            if ((s[i] < '0') || (s[i] > '9'))
                s[i] = ' ';
        }

        /* Negative Temp Handle */
        if (cur_temp <= (-1))
        {
            setRGB(255, 0, 0);
            temperature[0] = '-';
            temperature[1] = s[0];
            temperature[2] = s[1];
            temperature[3] = s[2];
            temperature[4] = s[3];
            temperature[5] = ' ';
            temperature[3] = ' ';
            temperature[4] = ' ';
            temperature[5] = ' ';
        }
        else
        {
            setRGB(0, 255, 0);
            temperature[0] = s[0];
            temperature[1] = s[1];
            temperature[2] = s[2];
            temperature[3] = s[3];
            temperature[4] = ' ';
            temperature[5] = ' ';
            temperature[6] = ' ';
            temperature[7] = ' ';
            temperature[8] = ' ';
            temperature[9] = ' ';
        }

        if (cur_temp <= 40 && cur_temp >= 30)
        {
            setRGB(0, 0, 255);
            update_lcd(SECOND_LINE, COL_0, 10, &SUCCESS_LABEL);
        }
        else
        {
            update_lcd(SECOND_LINE, COL_0, 10, &temperature);
        }


        // UART LCD Temp Update
        //lowres_2 = (int)prev_temp;
        DEBUG("|-");
        DEBUG(" Temperature: \n %d", lowres_2);

        // Searchs for update on current temp
        prev_temp = cur_temp;
        delay_ms(200);

        // DEBUG("Program started\r\n");
    }

    /* You can use below routines if you ve AT24 EEPROM */
    //void I2C_Write_EEProm(unsigned char slave_address, unsigned int memory_address, char * data, unsigned char DataLength )
    //I2C_Write_EEProm(SLAVE_ADDR, 0x00, "ABCDEFGHIJKLMNOP", 16);

    //void I2C_Read_EEProm(unsigned char slave_address, unsigned int memory_address, char * data, unsigned char DataLength )
    //I2C_Read_EEProm(SLAVE_ADDR, 0x05, t_ReceiveBuffer2, 16);  //reads five bytes starting at memory location 0x05 and returns the data into 'buf'

    //__bis_SR_register(LPM0_bits + GIE);
    //return 0;
}

