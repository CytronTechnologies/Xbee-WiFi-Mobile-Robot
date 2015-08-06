/*******************************************************
 *  Title: WiFi Mobile Robot
 *  Author: Idris Zainal Abidin
 *  PIC: PIC16F887
 *  Oscillator: 8 MHz (Internal)
 *  Software: MPLAB X IDE V1.51 + XC8 V1.12
 *  Last Modified: 4 Jan 2012
 *  Revision: 1.0
*******************************************************/

/***** Include files *****/
#include "system.h"
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

/***** Function prototype *****/
void picInit(void);
void beep(uChar times, uInt delayMs);
void motor(sChar speedLM, sChar speedRM);

/***** Global variable *****/

/***** Main function *****/
void main(void)
{
    uChar dataRx, sw2press = 0;

    picInit();
    uartInit(9600);
    UARTRST = 1;    // Reset Xbee WiFi
    delay_ms(1000);
    UARTRST = 0;
    
    beep(2,50);

    while(1)
    {
        if(!SW1)
        {
            beep(1,50);     // Beep once
            uartPutstr("WiFi Mobile Robot\r\n");    // Send string to HyperTerminal
            uartPutstr("w/W - Forward\r\n");        // Send string to HyperTerminal
            uartPutstr("a/A - Rotate Left\r\n");    // Send string to HyperTerminal
            uartPutstr("d/D - Rotate Right\r\n");   // Send string to HyperTerminal
            uartPutstr("s/S - Stop\r\n\n");         // Send string to HyperTerminal
            /* Test motor functionality */
            motor(50,-50); delay_ms(200); motor(0,0); delay_ms(200);
            motor(-50,50); delay_ms(400); motor(0,0); delay_ms(200);
            motor(50,-50); delay_ms(200); motor(0,0); delay_ms(200);
            uartPutstr("Ready!\r\n\n");     // Robot ready to action
            while(1)    // Forever loop
            {
                dataRx = uartReceive();         // Wait key to be pressed
                if(dataRx=='w' || dataRx=='W')  // If pressed 'w' / 'W'
                {
                    uartPutstr("Forward\r\n");  // Send string "Forward" to HyperTerminal
                    motor(70,70);               // Robot forward
                }
                else if(dataRx=='a' || dataRx=='A') // If pressed 'a' / 'A'
                {
                    uartPutstr("Rotate Left\r\n");  // Send string "Rotate Left" to HyperTerminal
                    motor(-60,60);                  // Robot rotate left
                }
                else if(dataRx=='d' || dataRx=='D') // If pressed 'd' / 'D'
                {
                    uartPutstr("Rotate Right\r\n"); // Send string "Rotate Right" to HyperTerminal
                    motor(60,-60);                  // Robot rotate right
                }
                else if(dataRx=='s' || dataRx=='S') // If pressed 's' / 'S'
                {
                    uartPutstr("Stop\r\n");     // Send string "Stop" to HyperTerminal
                    motor(0,0);                 // Robot stop
                }
            }
        }
        else if(!SW2)
        {
            uartNumber(sw2press++,DEC,2);
            uartPutstr(" Xbee WiFi\r\n");
            uartPutstr("Press any character\r\n\n");
            dataRx = uartReceive();
            beep(1,50);
        }
    }
}

/***** Sub function *****/
void picInit(void)
{
    IRCF2 = 1;      // IRCF<2:0> = 111 => 8MHz
    IRCF1 = 1;      // IRCF<2:0> = 110 => 4MHz, default
    IRCF0 = 1;      // IRCF<2:0> = 101 => 2MHz, PORTA = 0;

    ANSEL = 0;      // Set all AN pins as digital
    ANSELH = 0;

    PORTA = 0;      // Clear all pins at PORTA
    PORTB = 0;      // Clear all pins at PORTB
    PORTC = 0;      // Clear all pins at PORTC
    PORTD = 0;      // Clear all pins at PORTD
    PORTE = 0;      // Clear all pins at PORTE

    TRISA = 0b00000000;     // Set TRISA, 0:output, 1:input
    TRISB = 0b00000011;     // Set TRISB, 0:output, 1:input
    TRISC = 0b10000000;     // Set TRISC, 0:output, 1:input
    TRISD = 0b00000000;     // Set TRISD, 0:output, 1:input
    TRISE = 0b000;          // Set TRISE, 0:output, 1:input
}

void beep(uChar times, uInt delayMs)
{
    uInt loop;
    for(;times>0;times--)
    {
        BUZZER = 1;
        for(loop=0;loop<delayMs;loop++) delay_ms(1);
        BUZZER = 0;
        for(loop=0;loop<delayMs;loop++) delay_ms(1);
    }
}

void motor(sChar speedLM, sChar speedRM)
{
    uChar speed, maxSpeed = 80;

    if(speedLM<0)       // if speedLM is (-) value
    {
        motorLeftA = 1; // Left motor rotate backward
        motorLeftB = 0;
        speed = (-1)*speedLM;   // Change the (-) to (+) value
    }
    else if(speedLM>0)  // if speedLM is (+) value
    {
        motorLeftA = 0; // Left motor rotate forward
        motorLeftB = 1;
        speed = speedLM;
    }
    else if(!speedLM)   // if speedLM is null
    {
        motorLeftA = 0; // Left motor stop
        motorLeftB = 0;
        speed = speedLM;
    }
    if(speed>maxSpeed) speed = maxSpeed;    // Limit the speed
    setPwmRC1(1000,speed);  // Set freq = 1000, duty cycle = speed

    if(speedRM<0)       // if speedRM is (-) value
    {
        motorRightA = 1;    // Right motor rotate backward
        motorRightB = 0;
        speed = (-1)*speedRM;   // Change the (-) to (+) value
    }
    else if(speedRM>0)  // if speedRM is (+) value
    {
        motorRightA = 0;    // Right motor rotate forward
        motorRightB = 1;
        speed = speedRM;
    }
    else if(!speedRM)   // if speedRM is numm
    {
        motorRightA = 0;    // Right motor stop
        motorRightB = 0;
        speed = speedRM;
    }
    if(speed>maxSpeed) speed = maxSpeed;    // Limit the speed
    setPwmRC2(1000,speed);  // Set freq = 1000, duty cycle = speed
}

