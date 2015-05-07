/*******************************************************
 *  Title: WiFi Mobile Robot
 *  Author: Idris Zainal Abidin
 *  PIC: PIC16F887
 *  Oscillator: 8 MHz (Internal)
 *  Software: MPLAB X IDE V2.00 + XC8 V1.30
 *  Last Modified: 14 Apr 2014
 *  Revision: 1.0
 *******************************************************/

/***** Include files *****/
#include "system.h"
#include "lcd.h"
#include "uart.h"
#include "pwm.h"

/***** PIC special fuction configuration *****/
#pragma config FOSC = INTRC_NOCLKOUT    // I/O function on RA6 & RA7
#pragma config WDTE = OFF
#pragma config PWRTE = ON
#pragma config MCLRE = ON   // RE3/MCLR pin function is MCLR
#pragma config CP = OFF
#pragma config CPD = OFF
#pragma config BOREN = OFF
#pragma config IESO = OFF
#pragma config FCMEN = OFF
#pragma config LVP = OFF

/***** Define *****/
#define SW1 RB0
#define SW2 RB1

/***** Function prototype *****/
void picInit(void);
void beep(uChar times, uInt delayMs);
void wifiString(const char *s);
void motor(sChar speedLM, sChar speedRM);

/***** Global variable *****/

/***** Main function *****/
void main(void)
{
  uChar inByte;

  picInit();
  lcdInit();
  uartInit(9600);
  beep(2, 50);

  lcdClear();
  lcdPutstr("MC40A + ");
  lcdGoto(2,1);
  lcdPutstr("XbeeWiFi");

  while(1)
  {
    inByte = uartReceive();
    if(inByte == 'F') // Forward
    {
      // Read dummy byte
      uartReceive();
      uartReceive();
      uartReceive();

      motor(80, 80);

      lcdGoto(1,1);
      lcdPutstr("Go      ");
      lcdGoto(2,1);
      lcdPutstr("Forward ");
    }
    else if(inByte == 'B') // Backward
    {
      // Read dummy byte
      uartReceive();
      uartReceive();
      uartReceive();

      motor(-80, -80);

      lcdGoto(1,1);
      lcdPutstr("Go      ");
      lcdGoto(2,1);
      lcdPutstr("Backward");
    }
    else if(inByte == 'R') // Right
    {
      // Read dummy byte
      uartReceive();
      uartReceive();
      uartReceive();

      motor(80, -80);

      lcdGoto(1,1);
      lcdPutstr("Turn    ");
      lcdGoto(2,1);
      lcdPutstr("Right   ");
    }
    else if(inByte == 'L') // Left
    {
      // Read dummy byte
      uartReceive();
      uartReceive();
      uartReceive();

      motor(-80, 80);

      lcdGoto(1,1);
      lcdPutstr("Turn    ");
      lcdGoto(2,1);
      lcdPutstr("Left    ");
    }
    else if(inByte == 'S') // Stop
    {
      // Read dummy byte
      uartReceive();
      uartReceive();

      motor(0, 0);

      lcdGoto(1,1);
      lcdPutstr("        ");
      lcdGoto(2,1);
      lcdPutstr("Stop    ");
    }
  }
}

/***** Sub function *****/
void picInit(void)
{
  IRCF2 = 1; // IRCF<2:0> = 111 => 8MHz
  IRCF1 = 1; // IRCF<2:0> = 110 => 4MHz, default
  IRCF0 = 1; // IRCF<2:0> = 101 => 2MHz, PORTA = 0;

  ANSEL = 0; // Set all AN pins as digital
  ANSELH = 0;

  PORTA = 0; // Clear all pins at PORTA
  PORTB = 0; // Clear all pins at PORTB
  PORTC = 0; // Clear all pins at PORTC
  PORTD = 0; // Clear all pins at PORTD
  PORTE = 0; // Clear all pins at PORTE

  TRISA = 0b00111000; // Set TRISA, 0:output, 1:input
  TRISB = 0b00000011; // Set TRISB, 0:output, 1:input
  TRISC = 0b10000000; // Set TRISC, 0:output, 1:input
  TRISD = 0b00000000; // Set TRISD, 0:output, 1:input
  TRISE = 0b011; // Set TRISE, 0:output, 1:input
}

void beep(uChar times, uInt delayMs)
{
  uInt loop;
  for(; times > 0; times--)
  {
    BUZZER = 1;
    for(loop = 0; loop < delayMs; loop++) delayMs(1);
    BUZZER = 0;
    for(loop = 0; loop < delayMs; loop++) delayMs(1);
  }
}

void motor(sChar speedLM, sChar speedRM)
{
  uChar speed, maxSpeed = 80;

  if(speedLM < 0) // if speedLM is (-) value
  {
    motorLeftA = 1; // Left motor rotate backward
    motorLeftB = 0;
    speed = (-1) * speedLM; // Change the (-) to (+) value
  }
  else if(speedLM > 0) // if speedLM is (+) value
  {
    motorLeftA = 0; // Left motor rotate forward
    motorLeftB = 1;
    speed = speedLM;
  }
  else if(!speedLM) // if speedLM is null
  {
    motorLeftA = 0; // Left motor stop
    motorLeftB = 0;
    speed = speedLM;
  }
  if(speed > maxSpeed) speed = maxSpeed; // Limit the speed
  setPwmRC1(1000, speed); // Set freq = 1000, duty cycle = speed

  if(speedRM < 0) // if speedRM is (-) value
  {
    motorRightA = 1; // Right motor rotate backward
    motorRightB = 0;
    speed = (-1) * speedRM; // Change the (-) to (+) value
  }
  else if(speedRM > 0) // if speedRM is (+) value
  {
    motorRightA = 0; // Right motor rotate forward
    motorRightB = 1;
    speed = speedRM;
  }
  else if(!speedRM) // if speedRM is numm
  {
    motorRightA = 0; // Right motor stop
    motorRightB = 0;
    speed = speedRM;
  }
  if(speed > maxSpeed) speed = maxSpeed; // Limit the speed
  setPwmRC2(1000, speed); // Set freq = 1000, duty cycle = speed
}
