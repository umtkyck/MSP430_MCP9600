#ifndef DEBUG_H_
#define DEBUG_H_

#include <msp430g2553.h>
#include <string.h>
#include <stdio.h>

extern char uart_buf[200];
extern int bnum;

#define DEBUG_ENABLE 1

#if DEBUG_ENABLE
  #define DEBUG(...) sprintf(uart_buf,__VA_ARGS__);\
                     for (bnum = 0; bnum < strlen(uart_buf); bnum++) {\
                        while (!(IFG2&UCA0TXIFG));\
                        UCA0TXBUF = uart_buf[bnum];\
                     }
#else
  #define DEBUG(...)
#endif

#endif /* DEBUG_H_ */
