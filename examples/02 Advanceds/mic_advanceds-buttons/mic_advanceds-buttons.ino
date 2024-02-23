/*
   This sketch shows how to deal with input advanced setup.
   Here you will see how to read values from the port pins only after change there state (input operations).

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
   
   Compared to the base one, higher operating speed and less load on the chip.

   Author: Andrey Tarasenko.
*/

#include <AnTar_mic74.h>

#define TESTING // Determining the test mode
//#undef TESTING  // Uncomment if test mode is not used

#ifdef TESTING  // If the test mode is defined
  const char h[] = "HIGH";
  const char l[] = "LOW";
#endif

// constants won't change.
// They're used here to set pushbutton pin numbers:
const int buttonPin1 = 0;
const int buttonPin2 = 1;
const int buttonPin3 = 2;
const int buttonPin4 = 3;

// variables will change:
// variables for reading the state of the pushbuttons:
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

  int portModeValue = mic.readPortMode();   // Reading the current value of port mode register

  // Preparing a Port Value to Set a Specific Pin Mode
  portModeValue = mic.bitToClr(portModeValue, buttonPin1);    // Clearing the port bit to set it to input
  portModeValue = mic.bitToClr(portModeValue, buttonPin2);    // Clearing the port bit to set it to input
  portModeValue = mic.bitToClr(portModeValue, buttonPin3);    // Clearing the port bit to set it to input
  portModeValue = mic.bitToClr(portModeValue, buttonPin4);    // Clearing the port bit to set it to input

  // Write a new port value to the mode register
  mic.writePortMode(portModeValue);   // pushbuttons pins as input

  // Read port state change status for clear it
  mic.readStatus();

  mic.portRead(); // Read entire port

  // get the state of the pushbutton values:
  buttonState1 = mic.isBitSet(mic.getData(), buttonPin1);
  buttonState2 = mic.isBitSet(mic.getData(), buttonPin2);
  buttonState3 = mic.isBitSet(mic.getData(), buttonPin3);
  buttonState4 = mic.isBitSet(mic.getData(), buttonPin4);
}

void loop() {
  // If any pin level was changed
  if(mic.readStatus()) {
    mic.portRead(); // Read entire port

    // get the state of the pushbutton value:
    buttonState1 = mic.isBitSet(mic.getData(), buttonPin1);
    buttonState2 = mic.isBitSet(mic.getData(), buttonPin2);
    buttonState3 = mic.isBitSet(mic.getData(), buttonPin3);
    buttonState4 = mic.isBitSet(mic.getData(), buttonPin4);
  }

  #ifdef TESTING  // If the test mode is defined
    Serial.print("\t ");
    Serial.print((buttonState1 ? h : l));
    Serial.print("\t ");
    Serial.print((buttonState2 ? h : l));
    Serial.print("\t ");
    Serial.print((buttonState3 ? h : l));
    Serial.print("\t ");
    Serial.println((buttonState4 ? h : l));
    delay(200);
  #endif
}
