/*
   This sketch shows how to deal with input setup.
   Here you will see how to read values from the port pins (input operations).

   Arduino and MIC74 setup

   | Arduino  |  MIC74   | Description |
   | -------- | -------- | ----------- |
   |    A5    | CLK (14) | I2C Clock   |
   |    A4    | DAT (15) | I2C Data    |
   | -------- | -------- | ----------- |   
   | Buttons: |          |             |
   |    S1    |  P0 (4)  |  input      |
   |    S2    |  P1 (5)  |  input      |
   |    S3    |  P2 (6)  |  input      |
   |    S4    |  P3 (7)  |  input      |

   See schematic on https://github.com/TarAndr/AnTar_MIC74/blob/main/extras/images/MIC74_buttons.GIF

   Instructions:
   When the system starts, press any button.
   The state of each button is stored in a separate variable,
   and displayed in the serial monitor if the test mode is defined.

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
#endif

// constants won't change. They're used here to set pin numbers:
// pushbutton pin numbers
const int buttonPin1 = 0;
const int buttonPin2 = 1;
const int buttonPin3 = 2;
const int buttonPin4 = 3;

// variables will change:
// variables for reading the state of the pushbuttons
bool buttonState1 = HIGH;
bool buttonState2 = HIGH;
bool buttonState3 = HIGH;
bool buttonState4 = HIGH;

MIC74 mic;  // Creating a MIC object

void setup() {
  
  #ifdef TESTING  // If the test mode is defined
    Serial.begin(9600); // The baudrate of Serial monitor is set in 9600
    while (!Serial); // Waiting for Serial Monitor
    Serial.println("\nInitializing . . .\n\n");
    Serial.println("\tInput levels:");
    Serial.print("\n\tInput");
    Serial.print(buttonPin1);
    Serial.print("\tInput");
    Serial.print(buttonPin2);
    Serial.print("\tInput");
    Serial.print(buttonPin3);
    Serial.print("\tInput");
    Serial.println(buttonPin4);
  #endif

  mic.begin();  // Starting the device with default settings
  //mic.begin(0x25);  // Starting a device with a custom I2C address
  //mic.begin(0x25, 400000);  // Starting a device with a custom I2C address and frequency

  // initialize the pushbuttons pins as an input:
  mic.pinMode(buttonPin1, INPUT);
  mic.pinMode(buttonPin2, INPUT);
  mic.pinMode(buttonPin3, INPUT);
  mic.pinMode(buttonPin4, INPUT);
}

void loop() {
  // read the state of the pushbutton value:
  buttonState1 = mic.digitalRead(buttonPin1);
  buttonState2 = mic.digitalRead(buttonPin2);
  buttonState3 = mic.digitalRead(buttonPin3);
  buttonState4 = mic.digitalRead(buttonPin4);

  #ifdef TESTING  // If the test mode is defined
    Serial.print("\t ");
    Serial.print((buttonState1 ? h : l));
    Serial.print("\t ");
    Serial.print((buttonState2 ? h : l));
    Serial.print("\t ");
    Serial.print((buttonState3 ? h : l));
    Serial.print("\t ");
    Serial.println((buttonState4 ? h : l));
    delay(400);
  #endif
}
