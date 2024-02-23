/*
   This sketch shows how to deal with input/output setup.
   Here you will see how to read values from the port pins (input operations).
   Also you will see how to turn the port pins HIGH or LOW (output operations).

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
   | -------- | -------- | ----------- |   
   |   LEDs:  |          |             |
   |   LED1   |  P4 (9)  |  output     |
   |   LED2   |  P5 (10) |  output     |
   |   LED3   |  P6 (11) |  output     |
   |   LED4   |  P7 (12) |  output     |

   See schematic on https://github.com/TarAndr/AnTar_MIC74/blob/main/extras/images/MIC74_LEDs_and_buttons.GIF

   Instructions:
   When the system starts, press any button.
   The state of each button is stored in a separate variable,
   and displayed in the serial monitor if the test mode is defined.
   Also each button turn on/off a corresponding LED.

   If more efficiency is needed, then the example in the
    "Advanced" section should be used.

   Author: Andrey Tarasenko.
*/

// Connecting the library:
#include <AnTar_mic74.h>

#define TESTING // Determining the test mode
//#undef TESTING  // Uncomment if test mode is not used

#ifdef TESTING  // If the test mode is defined
  const char h[] = "HIGH";
  const char l[] = "LOW";
#endif

// constants won't change:
// They're used here to set pushbutton pin numbers:
const int buttonPin1 = 0;
const int buttonPin2 = 1;
const int buttonPin3 = 2;
const int buttonPin4 = 3;

// LED pin numbers:
const int ledPin1 = 4;
const int ledPin2 = 5;
const int ledPin3 = 6;
const int ledPin4 = 7;

// variables will change:
// variables for reading the state of the pushbuttons:
bool buttonState1 = HIGH;
bool buttonState2 = HIGH;
bool buttonState3 = HIGH;
bool buttonState4 = HIGH;

// variables for the last state of the pushbuttons:
bool lastButtonState1 = HIGH;
bool lastButtonState2 = HIGH;
bool lastButtonState3 = HIGH;
bool lastButtonState4 = HIGH;

// variables for storing the state of the leds:
bool ledState1 = HIGH;
bool ledState2 = HIGH;
bool ledState3 = HIGH;
bool ledState4 = HIGH;

MIC74 mic;  // Creating a MIC object

void setup() {
  
  #ifdef TESTING  // If the test mode is defined
    Serial.begin(9600); // The baudrate of Serial monitor is set in 9600
    while (!Serial); // Waiting for Serial Monitor
    Serial.println("\nInitializing . . .\n\n");
    Serial.println("\tInput levels:\t\t\t |\tOutput levels:");
    Serial.print("\n\tInput");
    Serial.print(buttonPin1);
    Serial.print("\tInput");
    Serial.print(buttonPin2);
    Serial.print("\tInput");
    Serial.print(buttonPin3);
    Serial.print("\tInput");
    Serial.print(buttonPin4);

    Serial.print("\t |\tOut ");
    Serial.print(ledPin1);
    Serial.print("\tOut ");
    Serial.print(ledPin2);
    Serial.print("\tOut ");
    Serial.print(ledPin3);
    Serial.print("\tOut ");
    Serial.println(ledPin4);
  #endif

  mic.begin();  // Starting the device with default settings
  //mic.begin(0x25);  // Starting a device with a custom I2C address
  //mic.begin(0x25, 400000);  // Starting a device with a custom I2C address and frequency

  // turn off all LEDs
  mic.digitalWrite(ledPin1, ledState1);
  mic.digitalWrite(ledPin2, ledState2);
  mic.digitalWrite(ledPin3, ledState3);
  mic.digitalWrite(ledPin4, ledState4);

  // initialize the pushbuttons pins as an input:
  mic.pinMode(buttonPin1, INPUT);
  mic.pinMode(buttonPin2, INPUT);
  mic.pinMode(buttonPin3, INPUT);
  mic.pinMode(buttonPin4, INPUT);

  // initialize the led pins as an output:
  mic.pinMode(ledPin1, OUTPUT);
  mic.pinMode(ledPin2, OUTPUT);
  mic.pinMode(ledPin3, OUTPUT);
  mic.pinMode(ledPin4, OUTPUT);
}

void loop() {
  // read the state of the pushbutton value:
  buttonState1 = mic.digitalRead(buttonPin1);
  // If the button state changed from pressed to released:
  if(buttonState1 && !lastButtonState1) {
    ledState1 = !ledState1;   // Change the state of the corresponding LED to the opposite
    mic.digitalWrite(ledPin1, ledState1);   // Write LED state
  }
  lastButtonState1 = buttonState1;    // Remember last button state

  buttonState2 = mic.digitalRead(buttonPin2);
  if(buttonState2 && !lastButtonState2) {
    ledState2 = !ledState2;
    mic.digitalWrite(ledPin2, ledState2);
  }
  lastButtonState2 = buttonState2;

  buttonState3 = mic.digitalRead(buttonPin3);
  if(buttonState3 && !lastButtonState3) {
    ledState3 = !ledState3;
    mic.digitalWrite(ledPin3, ledState3);
  }
  lastButtonState3 = buttonState3;

  buttonState4 = mic.digitalRead(buttonPin4);
  if(buttonState4 && !lastButtonState4) {
    ledState4 = !ledState4;
    mic.digitalWrite(ledPin4, ledState4);
  }
  lastButtonState4 = buttonState4;

  #ifdef TESTING  // If the test mode is defined
    Serial.print("\t ");
    Serial.print((buttonState1 ? h : l));
    Serial.print("\t ");
    Serial.print((buttonState2 ? h : l));
    Serial.print("\t ");
    Serial.print((buttonState3 ? h : l));
    Serial.print("\t ");
    Serial.print((buttonState4 ? h : l));

    Serial.print("\t |\t ");
    Serial.print((ledState1 ? h : l));
    Serial.print("\t ");
    Serial.print((ledState2 ? h : l));
    Serial.print("\t ");
    Serial.print((ledState3 ? h : l));
    Serial.print("\t ");
    Serial.println((ledState4 ? h : l));

    delay(30);  // short delay
  #endif

  delay(20);  // short delay for debounce
}
