/***********************************************************************
 * 
 * Stopwatch by Timer/Counter2 on the Liquid Crystal Display (LCD)
 *
 * ATmega328P (Arduino Uno), 16 MHz, PlatformIO
 *
 * Copyright (c) 2017 Tomas Fryza
 * Dept. of Radio Electronics, Brno University of Technology, Czechia
 * This work is licensed under the terms of the MIT license.
 * 
 * Components:
 *   16x2 character LCD with parallel interface
 *     VSS  - GND (Power supply ground)
 *     VDD  - +5V (Positive power supply)
 *     Vo   - (Contrast)
 *     RS   - PB0 (Register Select: High for Data transfer, Low for Instruction transfer)
 *     RW   - GND (Read/Write signal: High for Read mode, Low for Write mode)
 *     E    - PB1 (Read/Write Enable: High for Read, falling edge writes data to LCD)
 *     D3:0 - NC (Data bits 3..0, Not Connected)
 *     D4   - PD4 (Data bit 4)
 *     D5   - PD5 (Data bit 5)
 *     D6   - PD6 (Data bit 3)
 *     D7   - PD7 (Data bit 2)
 *     A+K  - Back-light enabled/disabled by PB2
 * 
 **********************************************************************/

/* Includes ----------------------------------------------------------*/
#include <avr/io.h>         // AVR device-specific IO definitions
#include <avr/interrupt.h>  // Interrupts standard C library for AVR-GCC
#include <gpio.h>           // GPIO library for AVR-GCC
#include "timer.h"          // Timer library for AVR-GCC
#include <lcd.h>            // Peter Fleury's LCD library
#include <stdlib.h>         // C library. Needed for number conversions


/* Function definitions ----------------------------------------------*/
/**********************************************************************
 * Function: Main function where the program execution begins
 * Purpose:  Update stopwatch value on LCD screen when 8-bit 
 *           Timer/Counter2 overflows.
 * Returns:  none
 **********************************************************************/
int main(void)
{
    // Initialize display
    lcd_init(LCD_DISP_ON_CURSOR_BLINK);
    // Custom character definition using https://omerk.github.io/lcdchargen/
    uint8_t customChar[8] = {
       	0b10001, 
	      0b11011,
      	0b11111,
      	0b11111,
        0b01110,
        0b01110,
        0b00000,
        0b00100
    };


    lcd_command(1<<LCD_CGRAM);       // Set addressing to CGRAM (Character Generator RAM)
                                     // ie to individual lines of character patterns
    for (uint8_t i = 0; i < 8; i++)  // Copy new character patterns line by line to CGRAM
        lcd_data(customChar[i]);
   
    lcd_command(1<<LCD_DDRAM);       // Set addressing back to DDRAM (Display Data RAM)
                                     // ie to character codes

    // Display symbol with Character code 0
    lcd_gotoxy(10,2);
    lcd_putc(0x00);

    // Configuration of 8-bit Timer/Counter2 for Stopwatch update
    // Set the overflow prescaler to 16 ms and enable interrupt
    TIM2_overflow_16ms();
    TIM2_overflow_interrupt_enable();
    TIM0_overflow_4096us();
    TIM0_overflow_interrupt_enable();

    // Enables interrupts by setting the global interrupt mask
    sei();

    // Infinite loop
    while (1)
    {
        /* Empty loop. All subsequent operations are performed exclusively 
         * inside interrupt service routines, ISRs */
    }

    // Will never reach this
    return 0;
}


/* Interrupt service routines ----------------------------------------*/
/**********************************************************************
 * Function: Timer/Counter2 overflow interrupt
 * Purpose:  Update the stopwatch on LCD screen every sixth overflow,
 *           ie approximately every 100 ms (6 x 16 ms = 100 ms).
 **********************************************************************/
ISR(TIMER2_OVF_vect)
{
    static uint8_t no_of_overflows = 0;
    static uint8_t tenths = 0;  // Tenths of a second
    char string[2];             // String for converted numbers by itoa()
    static uint8_t seconds=0;
    static uint8_t minutes=0;
    no_of_overflows++;
    if (no_of_overflows >= 6)
    {
        // Do this every 6 x 16 ms = 100 ms
        no_of_overflows = 0;
        
        tenths++;

        if (tenths > 9)

        {
          tenths =0;
          seconds++;
          if (seconds >59 )
          {
            seconds=0;
            minutes++;
          }
        
        }
        
        // Display "00:00.tenths"
        // Count tenth of seconds 0, 1, ..., 9, 0, 1, ...
        
        lcd_gotoxy(1, 0);

        itoa(minutes, string, 10);  // Convert decimal value to string
        lcd_puts(string);

        lcd_puts(":");

        lcd_puts(string);
        itoa(seconds, string, 10);  // Convert decimal value to string

        lcd_puts(":");

        itoa(tenths, string, 10);  // Convert decimal value to string
        lcd_puts(string);
          
       
          
         }
    // Else do nothing and exit the ISR
}

ISR(TIMER0_OVF_vect)
{
    static uint8_t symbol = 0;
    static uint8_t position = 0;
    static uint8_t no_of_overflows = 0;

    no_of_overflows++;
    if (no_of_overflows >= 5)
    {
        // Do this every 6 x 16 ms = 100 ms
        no_of_overflows = 0;
        lcd_gotoxy(1+position,1);
        lcd_putc(symbol);
        symbol++;
        if (symbol > 5)

        {
         symbol=0;
         position++;
        
        }
        if(position > 10){
          position=0;
        }
    
    }
}