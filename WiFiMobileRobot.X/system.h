/***** Include files *****/
#include <htc.h>

/***** Define *****/
#define	_XTAL_FREQ  8000000

#define SW1 RB0
#define SW2 RB1

#define BUZZER  RB7
#define UARTRST RC0

#define motorRightA RB2
#define motorRightB RB3
#define motorLeftA  RB4
#define motorLeftB  RB5

#define HEX     16
#define DEC     10
#define OCT     8
#define BIN     2

#define delay_ms(x) __delay_ms(x)
#define delay_us(x) __delay_us(x)

typedef unsigned char uChar;
typedef unsigned int uInt;
typedef unsigned long uLong;
typedef signed char sChar;
typedef signed int sInt;
typedef signed long sLong;
