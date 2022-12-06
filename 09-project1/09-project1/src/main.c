#include <avr/io.h>         // AVR device-specific IO definitions
#include <avr/interrupt.h>  // Interrupts standard C library for AVR-GCC
#include <gpio.h>           // GPIO library for AVR-GCC
#include "timer.h"          // Timer library for AVR-GCC
#include <lcd.h>            // Peter Fleury's LCD library
#include <stdlib.h>         // C library. Needed for number conversions
#include <uart.h>

//joyStick button
#define BUTTON PB2
#define BUTTONenc PB3
//encoder pins
#define A  4 
#define B  5

//static global variable
static uint8_t joyX = 0;
static uint8_t joyY = 0;

int main(void)
{
  // Initialize display
  lcd_init(LCD_DISP_ON);
  uart_init(UART_BAUD_SELECT(9600, F_CPU));

  //Setting GPIO pins
  GPIO_mode_input_pullup(&DDRB, BUTTON);
  GPIO_mode_input_nopullup(&DDRB, A);
  GPIO_mode_input_nopullup(&DDRB, B);


// Configure Analog-to-Digital Convertion unit
    // Select ADC voltage reference to "AVcc with external capacitor at AREF pin"
    ADMUX |= (1<<REFS0);
    ADMUX &= ~(1<<REFS1);
    // Select input channel ADC0 (voltage divider pin)
  
    // channel ADC1
   

    ADCSRA |= (1<<ADEN);
    // Enable conversion complete interrupt
    ADCSRA |= (1<<ADIE);
    // Set clock prescaler to 128
    ADCSRA |= (1<<ADPS0);
    ADCSRA |= (1<<ADPS1);
    ADCSRA |= (1<<ADPS2);

    // Configure 16-bit Timer/Counter1 to start ADC conversion
    // Set prescaler to 33 ms and enable overflow interrupt
    TIM1_overflow_33ms();
    TIM1_overflow_interrupt_enable();



    // Enables interrupts by setting the global interrupt mask
    sei();

    // Infinite loop
    while (1)
    {
        /* Empty loop. All subsequent operations are performed exclusively 
         * inside interrupt service routines ISRs */
    }

    // Will never reach this
    return 0;
}

ISR(TIMER1_OVF_vect)
{ 
  static int8_t tec = 0;
  if(tec == 0)
  {  	ADMUX &= ~(1<<MUX0);
      ADMUX &= ~(1<<MUX1);
      ADMUX &= ~(1<<MUX2);
      ADMUX &= ~(1<<MUX3);
      // Start ADC conversion
      ADCSRA |= (1<<ADSC);
      tec = 1;
      }
  else{ADMUX |=  (1<<MUX0);
    ADMUX &= ~(1<<MUX1);
    ADMUX &= ~(1<<MUX2);
    ADMUX &= ~(1<<MUX3);
        // Start ADC conversion
    ADCSRA |= (1<<ADSC);
    tec = 0;
  }
  Encoder();
  //lcd_clrscr();
  
     
}




ISR(ADC_vect)
{

    
    char string[4];
    static float valueX;
    static float valueY;
    static int16_t tec = 0;
    
    if (tec == 0)
    {
      valueX = ADC;
      tec = 1;
    }else{
      valueY = ADC;
      tec = 0;
    }
    
   /* itoa(valueX,string, 10);
    lcd_gotoxy(6, 0);
    lcd_puts(string);

    itoa(valueY,string, 10);
    lcd_gotoxy(6, 1);
    lcd_puts(string);*/

    if(!GPIO_read(&PINB, BUTTON)){
      lcd_gotoxy(joyX,joyY);
      lcd_puts(" ");
    }
    //down
    if (valueY > 800)
    {
      joyY--;
    }
    //up
    else if (valueY < 200)
    {
      joyY++;
    }
    //right
    if (valueX > 800)
    {
      joyX++;
    }
    //left
    else if (valueX < 200)
    {
      joyX--;
    }
    if(joyY > 1){
      joyY = 0;
    }if(joyY < 0){
      joyY = 1;
    }
    if(joyX > 15){
      joyX = 0;
    }if(joyX < 0){
      joyX = 15;
    }
    
    
    


}

//method for encoder
void Encoder(){
  static uint8_t A_curr, B_curr, A_prev;
  static uint8_t prom = 0;
  static int8_t counter = 0;
  static char alphabet[] = {'A'};
  A_curr = GPIO_read(&PINB, A); 
  B_curr = GPIO_read(&PINB, B);
 
  if(A_curr != A_prev && A_curr  == 1)
  {
    //turning left
    if (B_curr ==A_curr)
    {
    prom = 1;
    counter--;
    }
    //turning right
    else{
       
        prom = 2;
        counter++;
    }
   
  }
  A_prev = A_curr;
  if (prom == 1)
  {
    //lcd_gotoxy(10, 0); 
    //lcd_puts("CCW");
    uart_puts("CCW");
    uart_puts("\r\n\n");
    

  }
  if (prom == 2)
  {
    uart_puts("CW ");
    uart_puts("\r\n\n");
    
  }
  if (counter <= -1)
  {
    counter = 25;
  }
  if (counter >= 26)
  {
    counter == 0;
  }
  lcd_gotoxy(joyX,joyY);
  lcd_puts("_");

  if(!GPIO_read(&PINB, BUTTONenc)){
    lcd_gotoxy(joyX,joyY);
    lcd_puts(alphabet[0]);
  }

}