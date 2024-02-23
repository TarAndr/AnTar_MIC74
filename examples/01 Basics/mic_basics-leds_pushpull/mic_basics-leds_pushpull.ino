/*
   This sketch shows how to deal with output setup.
   Here you will see how to turn the port pins HIGH or LOW (output operations).

   Arduino and MIC74 setup

   | Arduino  |  MIC74   | Description |
   | -------- | -------- | ----------- |
   |    A5    | CLK (14) | I2C Clock   |
   |    A4    | DAT (15) | I2C Data    |
   | -------- | -------- | ----------- |   
   |   LEDs:  |          |             |
   |   LED1   |  P0 (4)  |  output     |
   |   LED2   |  P1 (5)  |  output     |
   |   LED3   |  P2 (6)  |  output     |
   |   LED4   |  P3 (7)  |  output     |

   See schematic on https://github.com/TarAndr/AnTar_MIC74/blob/main/extras/images/MIC74_LEDs_pushpull.GIF

   Instructions:
   After starting, the connected LEDs will light up in turn, and also turn off in turn.

   If more efficiency is needed, then the example in the
    "Advanced" section should be used.

   Author: Andrey Tarasenko.
*/

#include <AnTar_mic74.h>

#define TESTING // Determining the test mode
//#undef TESTING  // Uncomment if test mode is not used

#ifdef TESTING  // If the test mode is defined
  const char h[] = "HIGH";
  const char l[] = "LOW";

  // variables for storing the state of the leds
  bool ledState1 = LOW;
  bool ledState2 = LOW;
  bool ledState3 = LOW;
  bool ledState4 = LOW;
#endif

// constants won't change. They're used here to set pin numbers:
// led pin numbers
const int ledPin1 = 0;
const int ledPin2 = 1;
const int ledPin3 = 2;
const int ledPin4 = 3;

// variables will change:
bool activeState = HIGH;

MIC74 mic;  // Creating a MIC object

void setup() {
  
  #ifdef TESTING  // If the test mode is defined
    Serial.begin(9600); // The baudrate of Serial monitor is set in 9600
    while (!Serial); // Waiting for Serial Monitor
    Serial.println("\nInitializing . . .\n\n");
    Serial.println("\tOutput levels:");
    Serial.print("\n\tOutput");
    Serial.print(ledPin1);
    Serial.print("\tOutput");
    Serial.print(ledPin2);
    Serial.print("\tOutput");
    Serial.print(ledPin3);
    Serial.print("\tOutput");
    Serial.println(ledPin4);
  #endif

  mic.begin();  // Starting the device with default settings
  //mic.begin(0x25);  // Starting a device with a custom I2C address
  //mic.begin(0x25, 400000);  // Starting a device with a custom I2C address and frequency

  // turn off all LEDs
  mic.digitalWrite(ledPin1, !activeState);
  mic.digitalWrite(ledPin2, !activeState);
  mic.digitalWrite(ledPin3, !activeState);
  mic.digitalWrite(ledPin4, !activeState);

  // initialize the led pins as an output:
  mic.pinMode(ledPin1, OUTPUT_PUSHPULL);
  mic.pinMode(ledPin2, OUTPUT_PUSHPULL);
  mic.pinMode(ledPin3, OUTPUT_PUSHPULL);
  mic.pinMode(ledPin4, OUTPUT_PUSHPULL);
}

void loop() {

  for(int i = ledPin1; i <= ledPin4; ++i) {
    // write the state of the led value:
    mic.digitalWrite(i, activeState);

    #ifdef TESTING  // If the test mode is defined
      // read the state of the led values:
      ledState1 = mic.digitalRead(ledPin1);
      ledState2 = mic.digitalRead(ledPin2);
      ledState3 = mic.digitalRead(ledPin3);
      ledState4 = mic.digitalRead(ledPin4);

      Serial.print("\t ");
      Serial.print((ledState1 ? h : l));
      Serial.print("\t ");
      Serial.print((ledState2 ? h : l));
      Serial.print("\t ");
      Serial.print((ledState3 ? h : l));
      Serial.print("\t ");
      Serial.println((ledState4 ? h : l));
    #endif

    delay(400);
  }

  activeState = !activeState;
}
