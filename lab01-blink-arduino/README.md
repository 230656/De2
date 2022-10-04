# Lab 1: Filip Sec 230656

### Morse code

1. The code displays the word P A R I S at intervals using       Morse code. A delay is added at the end of the program to make it easier to recognize the end.

```c
int LED = 13;
int sTime = 300;
int lTime = 900;
int down = 2000;

void setup()
{
    // Set pin where on-board LED is connected as output
    pinMode(LED, OUTPUT);}

    // Infinite loop
    void loop() {

        // Generate P

       switchLED(sTime);
       switchLED(lTime);
       switchLED(lTime);
       switchLED(sTime);
       delay(sTime);
       switchLED(sTime); // Generate A
       switchLED(lTime);
       delay(sTime);
       switchLED(sTime); // Generate R
       switchLED(lTime);
       switchLED(sTime);
       delay(sTime);
       switchLED(sTime); //Generate I
       switchLED(sTime);
       delay(sTime);
       switchLED(sTime); //Generate S
       switchLED(sTime);
       switchLED(sTime);
       delay(down); //end
    }



void switchLED(int timing){
        digitalWrite(LED,HIGH); // posle napetie na pin 13
        delay(timing);
        digitalWrite(LED,LOW);
        delay(timing);
    }
```

2. Scheme of Morse code application, i.e. connection of AVR device, LED, resistor, and supply voltage. The image can be drawn on a computer or by hand. Always name all components and their values!
   resistor = 220 ohm
   LED generic
video of functionality
https://youtu.be/U-zBKXliTQQ
