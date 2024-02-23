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
   |   LED1   |  P4 (9)  |  output     |
   |   LED2   |  P5 (10) |  output     |
   |   LED3   |  P6 (11) |  output     |
   |   LED4   |  P7 (12) |  output     |

   See schematic on https://github.com/TarAndr/AnTar_MIC74/blob/main/extras/images/MIC74_LEDs.GIF

   Instructions:
   After starting, the connected LEDs will light up in turn, and also turn off in turn.

   Compared to the base one, higher operating speed and less load on the chip.

   Author: Andrey Tarasenko.
*/

#include <AnTar_mic74.h>

#define TESTING // Determining the test mode
//#undef TESTING  // Uncomment if test mode is not used

#ifdef TESTING  // If the test mode is defined
  const char h[] = "HIGH";
  const char l[] = "LOW";

  // variables for storing the state of the leds
  bool ledState1 = HIGH;
  bool ledState2 = HIGH;
  bool ledState3 = HIGH;
  bool ledState4 = HIGH;
#endif

// constants won't change. They're used here to set pin numbers:
// led pin numbers
const int ledPin1 = 4;
const int ledPin2 = 5;
const int ledPin3 = 6;
const int ledPin4 = 7;

// variables will change:
bool activeState = LOW;

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

  mic.portRead(); // Reading the entire port to store its current value in the shadow register

  // turn off all LEDs
  mic.digitalWriteDelayed(ledPin1, !activeState);
  mic.digitalWriteDelayed(ledPin2, !activeState);
  mic.digitalWriteDelayed(ledPin3, !activeState);
  mic.digitalWriteDelayed(ledPin4, !activeState);

  mic.portWrite();  // write the state of the led value

  int portModeValue = mic.readPortMode();   // Reading the current value of port mode register

  // Preparing a Port Value to Set a Specific Pin Mode
  portModeValue = mic.bitToSet(portModeValue, ledPin1);    // Setting a port bit to set it to output
  portModeValue = mic.bitToSet(portModeValue, ledPin2);    // Setting a port bit to set it to output
  portModeValue = mic.bitToSet(portModeValue, ledPin3);    // Setting a port bit to set it to output
  portModeValue = mic.bitToSet(portModeValue, ledPin4);    // Setting a port bit to set it to output

  // Write a new port value to the mode register
  mic.writePortMode(portModeValue);   // led pins as an output
}

void loop() {
	// preparing values for later writing to the port:
	mic.digitalWriteDelayed(ledPin1, activeState);
	mic.digitalWriteDelayed(ledPin2, activeState);
	mic.digitalWriteDelayed(ledPin3, !activeState);
	mic.digitalWriteDelayed(ledPin4, !activeState);

	// write the state of the led value:
	mic.portWrite();

	#ifdef TESTING  // If the test mode is defined
	  // get the state of the led values:
	  ledState1 = mic.isBitSet(mic.getData(), ledPin1);
	  ledState2 = mic.isBitSet(mic.getData(), ledPin2);
	  ledState3 = mic.isBitSet(mic.getData(), ledPin3);
	  ledState4 = mic.isBitSet(mic.getData(), ledPin4);

	  Serial.print("\t ");
	  Serial.print((ledState1 ? h : l));
	  Serial.print("\t ");
	  Serial.print((ledState2 ? h : l));
	  Serial.print("\t ");
	  Serial.print((ledState3 ? h : l));
	  Serial.print("\t ");
	  Serial.println((ledState4 ? h : l));
	#endif

	delay(700);

	activeState = !activeState;
}
