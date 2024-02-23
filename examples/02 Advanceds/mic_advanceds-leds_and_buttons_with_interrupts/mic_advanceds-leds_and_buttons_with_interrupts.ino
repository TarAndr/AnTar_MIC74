/*
   This sketch shows how to deal with input/output setup.
   Here you will see how to read values from the port pins (input operations).
   Also you will see how to turn the port pins HIGH or LOW (output operations).

   Arduino and MIC74 setup

   | Arduino  |  MIC74   | Description |
   | -------- | -------- | ----------- |
   |    D2    | ALR (13) | Alert Line  |
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

   See schematic on https://github.com/TarAndr/AnTar_MIC74/blob/main/extras/images/MIC74_LEDs_and_buttons_with_interrupts.GIF

   Instructions:
   When the system starts, press any button.
   The state of each button is stored in a separate variable,
   and displayed in the serial monitor if the test mode is defined.
   Also each button turn on/off a corresponding LED.

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

// constants won't change. They're used here to set pin numbers:
// Arduino interrupt pin:
const int micAlertPin = 2;
//const int micAlertPin = A3;

// pushbutton pin numbers
const int buttonPin1 = 0;
const int buttonPin2 = 1;
const int buttonPin3 = 2;
const int buttonPin4 = 3;

// LED pin numbers
const int ledPin1 = 4;
const int ledPin2 = 5;
const int ledPin3 = 6;
const int ledPin4 = 7;

// variables will change:
// Variable for interrupt status flag from MIC74:
volatile bool micAlert = false;

// variables for reading the state of the pushbuttons
bool buttonState1 = HIGH;
bool buttonState2 = HIGH;
bool buttonState3 = HIGH;
bool buttonState4 = HIGH;

// variables for the last state of the pushbuttons
bool lastButtonState1 = HIGH;
bool lastButtonState2 = HIGH;
bool lastButtonState3 = HIGH;
bool lastButtonState4 = HIGH;

// variables for storing the state of the leds
bool ledState1 = HIGH;
bool ledState2 = HIGH;
bool ledState3 = HIGH;
bool ledState4 = HIGH;

/* 
void attachPCinterrupt(uint8_t pin)
{
    *digitalPinToPCMSK(pin) |= bit(digitalPinToPCMSKbit(pin));  // enable pin
    PCIFR |= bit(digitalPinToPCICRbit(pin)); // clear any outstanding interrupt
    PCICR |= bit(digitalPinToPCICRbit(pin)); // enable interrupt for the group
}

void detachPCinterrupt(uint8_t pin)
{
    *digitalPinToPCMSK(pin) &= ~(bit(digitalPinToPCMSKbit(pin)));  // disable pin
    PCICR &= ~(bit(digitalPinToPCICRbit(pin))); // disable interrupt for the group
}

// Function for interrupt handling from MIC74:
ISR(PCINT1_vect) {
  if(!digitalRead(alertPin)) {
	  detachPCinterrupt(alertPin);
	  micAlert = true;
	  }
}
 */

// Function for interrupt handling from MIC74:
void micAlertHandling() {
	detachInterrupt(digitalPinToInterrupt(micAlertPin));
	micAlert = true;
}

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

  mic.portRead(); // Reading the entire port to store its current value in the shadow register

  // turn off all LEDs
  mic.digitalWriteDelayed(ledPin1, ledState1);
  mic.digitalWriteDelayed(ledPin2, ledState2);
  mic.digitalWriteDelayed(ledPin3, ledState3);
  mic.digitalWriteDelayed(ledPin4, ledState4);

  mic.portWrite();  // write the state of the led value

  int portModeValue = mic.readPortMode();   // Reading the current value of port mode register

  // Preparing a Port Value to Set a Specific Pin Mode
  portModeValue = mic.bitToClr(portModeValue, buttonPin1);    // Clearing the port bit to set it to input
  portModeValue = mic.bitToClr(portModeValue, buttonPin2);    // Clearing the port bit to set it to input
  portModeValue = mic.bitToClr(portModeValue, buttonPin3);    // Clearing the port bit to set it to input
  portModeValue = mic.bitToClr(portModeValue, buttonPin4);    // Clearing the port bit to set it to input

  portModeValue = mic.bitToSet(portModeValue, ledPin1);    // Setting a port bit to set it to output
  portModeValue = mic.bitToSet(portModeValue, ledPin2);    // Setting a port bit to set it to output
  portModeValue = mic.bitToSet(portModeValue, ledPin3);    // Setting a port bit to set it to output
  portModeValue = mic.bitToSet(portModeValue, ledPin4);    // Setting a port bit to set it to output

  // Write a new port value to the mode register
  mic.writePortMode(portModeValue);   // pushbuttons pins as input, led pins as an output

  portModeValue = mic.readPortInterrupts();   // Reading the current value of interrupts mask register

  // Preparing a Mask Value to Set a Specific Pin interrupts
  portModeValue = mic.bitToSet(portModeValue, buttonPin1);    // Seting the port bit to set it for interrupt
  portModeValue = mic.bitToSet(portModeValue, buttonPin2);    // Seting the port bit to set it for interrupt
  portModeValue = mic.bitToSet(portModeValue, buttonPin3);    // Seting the port bit to set it for interrupt
  portModeValue = mic.bitToSet(portModeValue, buttonPin4);    // Seting the port bit to set it for interrupt

  mic.writePortInterrupts(portModeValue);   // Writing the value of interrupts mask register

  mic.readStatus();   // Read port state change status for clear it

  mic.portRead(); // Read entire port

  mic.setInterrupts(ON);  // Enable global interrupt feature

  // get the state of the pushbutton values:
  lastButtonState1 = buttonState1 = mic.isBitSet(mic.getData(), buttonPin1);
  lastButtonState2 = buttonState2 = mic.isBitSet(mic.getData(), buttonPin2);
  lastButtonState3 = buttonState3 = mic.isBitSet(mic.getData(), buttonPin3);
  lastButtonState4 = buttonState4 = mic.isBitSet(mic.getData(), buttonPin4);

  pinMode(micAlertPin, INPUT_PULLUP);  // Setting Arduino pin to detect interrupt
  attachInterrupt(digitalPinToInterrupt(micAlertPin), micAlertHandling, LOW);
  //attachPCinterrupt(micAlertPin);
}

void loop() {
  // If any pin level was changed
  if(micAlert) {
    mic.readStatus();  // Read port state change status for clear it
    micAlert = false;	// Reset alert status

    delay(50);    // short delay for debounce
    mic.portRead(); // Read entire port

    if(mic.isBitSet(mic.getStatus(), buttonPin1)) {
      // get the state of the pushbutton value:
      buttonState1 = mic.isBitSet(mic.getData(), buttonPin1);
      // If the button state changed from pressed to released:
      if(buttonState1 && !lastButtonState1) {
        ledState1 = !ledState1;   // Change the state of the corresponding LED to the opposite
        mic.digitalWriteDelayed(ledPin1, ledState1);    // Write LED state delayed
      }
      lastButtonState1 = buttonState1;    // Remember last button state
    }

    if(mic.isBitSet(mic.getStatus(), buttonPin2)) {
      buttonState2 = mic.isBitSet(mic.getData(), buttonPin2);
      if(buttonState2 && !lastButtonState2) {
        ledState2 = !ledState2;
        mic.digitalWriteDelayed(ledPin2, ledState2);
      }
      lastButtonState2 = buttonState2;
    }

    if(mic.isBitSet(mic.getStatus(), buttonPin3)) {
      buttonState3 = mic.isBitSet(mic.getData(), buttonPin3);
      if(buttonState3 && !lastButtonState3) {
        ledState3 = !ledState3;
        mic.digitalWriteDelayed(ledPin3, ledState3);
      }
      lastButtonState3 = buttonState3;
    }

    if(mic.isBitSet(mic.getStatus(), buttonPin4)) {
      buttonState4 = mic.isBitSet(mic.getData(), buttonPin4);
      if(buttonState4 && !lastButtonState4) {
        ledState4 = !ledState4;
        mic.digitalWriteDelayed(ledPin4, ledState4);
      }
      lastButtonState4 = buttonState4;
    }

    mic.portWrite();    // write the state of the led value

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

      delay(30);    // short delay
    #endif

	attachInterrupt(digitalPinToInterrupt(micAlertPin), micAlertHandling, LOW);
	//attachPCinterrupt(micAlertPin);
  }
}
