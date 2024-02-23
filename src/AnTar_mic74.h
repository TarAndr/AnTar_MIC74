#ifndef AnTar_mic74_h
#define AnTar_mic74_h

/**
 * @mainpage MIC74 Arduino Library implementation 
 * @details This library was built based on the Datasheet "MIC74 2-Wire Serial I/O Expander and Fan Controller" from Microchip
 * @details The MIC74 is a fully programmable serial-to-parallel I/O expander compatible with the SMBus™ (system management bus) protocol. It acts as a client on the bus, providing eight independent I/O lines.
 * @details Some Arduino projects may require more pins than the Arduino actually has. In these cases, you can use up to 8 MIC74 devices using only the I²C bus (two Arduino pins) and add up to 64 input/output ports to your project. 
 * @details This library can be freely distributed using the MIT Free Software model.
 * @details This library uses the I²C communication protocol and implements most important functions offered by MIC74 device from MicroChip. It also has primitive functions that make it easier to implement commands that may not have been implemented yet. The main features implemented can be seen below:
 * @details 1) GPIO individual control (8 I/O pins)
 * @details 2) Reading and writing Access to all registers (0x00 ~ 0x06)
 * @details 3) I²C address customization (0x20 ~ 0x27)
 * @details 4) Output mode configuration (push-pull or open-drain)
 * @details 5) Internal Interrupt feature setup
 * @details 6) Fan speed control
 * @author Andrey Tarasenko (antar.georgia@gmail.com)
 * @brief It is a Library to control the MIC74 device.
 * @date 2023-05-01
 * @copyright Copyright (c) 2023 Andrey Tarasenko
 */

#include <Arduino.h>
#include <Wire.h>

// registers
#define REG_DEV_CFG 0x00	// 0b00000000 (Power-on default value) - Device configuration read/write register.
/*										^- IE - Global Interrupt Bit 0 (Operation: 1 = enabled, 0 = disabled).
 *									   ^-- FAN - Selects fan mode Bit 1 (Operation: 1 = fan mode; 0 = I/O mode).
 *								 ^^^^^^--- Bit Name: D[2] through D[7] Reserved (Operation: always write as zero).
 */
#define REG_DIR 0x01		// 0b00000000 (Power-on default value) - Data direction read/write register.
/*								 ^^^^^^^^- DIRn - Selects data direction, input or output (Operation: 0 = input; 1 = output).
 */
#define REG_OUT_CFG 0x02	// 0b00000000 (Power-on default value) - Output configuration read/write register.
/*								 ^^^^^^^^- OUTn - Selects output driver configuration (Operation: 1 = push-pull; 0 = open-drain).
 */
#define REG_STATUS 0x03		// 0b00000000 (Power-on default value) - Status read register.
/*								 ^^^^^^^^- Sn - Flag for input-change event (Operation: 1 = change occurred; 0 = no change occurred).
 */
#define REG_INT_MASK 0x04	// 0b00000000 (Power-on default value) - Interrupt mask read/write register.
/*								 ^^^^^^^^- IMn - Interrupt enable bit (Operation: 1 = enabled; 0 = disabled).
 */
#define REG_DATA 0x05		// 0b11111111 (Power-on default value) - Data read/write register.
/*							 	 ^^^^^^^^- Pn - Returns the current state of any Pin configured as an input (Read operation: 1 = Pin is high; 0 = Pin is low).
								 ^^^^^^^^- Pn - Returns the last value written to Pin’s configured as outputs,
											Writing the DATA register sets the output state of any Pin’s configured as outputs;
											writes to I/O bits configured as inputs are ignored (Write operation: 1 = Pin is set to ‘1’; 0 = Pin is cleared).
 */
#define REG_FAN_SPEED 0x06	// 0b00000000 (Power-on default value) - Fan speed read/write register, Determines bit-pattern on FS[2:0].
/*									  ^^^- Bit name: D[0] through D[2] (Operation: Fan speed settings).
 *								 ^^^^^---- Bit Name: D[3] through D[7] Reserved (Operation: Always write as zero).
 */

// bits
#define BIT_IE 0x00
#define BIT_FAN 0x01

// values
#define PORT_CLR 0x00
#define PORT_SET 0xFF

#define OFF 0x0
#define ON 0x1

#define INPUT_WITH_INTERRUPT 0x3
#define OUTPUT_PUSHPULL 0x4

#define DEF_I2C_ADDR 0x27
#define DEF_I2C_FREQ 100000

#define IS_BIT_SET(x,y) ( (x) & (1 << (y)) )  // Check if a bit is set. Returns 0 or != 0

class MIC74
{

protected:
   uint8_t _i2cAddress = DEF_I2C_ADDR;	// Default i2c address
   uint8_t _data = PORT_SET;			// REG_DATA shadow register
   uint8_t _status = PORT_CLR;			// REG_STATUS shadow register

   uint8_t regRead(uint8_t reg);								// Gets the given register information
   void regWrite(uint8_t reg, uint8_t value);					// Sets a value to a given register

   bool regBitRead(uint8_t mic_register, uint8_t bit_position);
   void regBitWrite(uint8_t mic_register, uint8_t bit_position, uint8_t value);

public:
   void begin(uint8_t i2cAddress = DEF_I2C_ADDR, long i2cFrequency = DEF_I2C_FREQ);

   void setup(uint8_t ie = OFF, uint8_t fan = OFF);			// Sets the DEV_CFG register

   uint8_t lookFor();										// Look for MIC74 device I2C Address

   uint8_t readPortMode();									// Gets a value from the DIR Register
   void writePortMode(uint8_t value);						// Sets a value to the DIR Register

   void pinMode(uint8_t pin, uint8_t mode);					// Configures the specified pin to behave either as an input or an output

   uint8_t readPortOutMode();								// Gets a value from the OUT_CFG Register
   void writePortOutMode(uint8_t value);					// Sets a value to the OUT_CFG Register

   void pushPullPinOn(uint8_t pin);							// Activates the push-pull to a given GPIO pin
   void pushPullPinOff(uint8_t pin);						// Deactivates the push-pull to a given GPIO pin

   void setInterrupts(uint8_t value);						// Configures the MIC74 global interrupt feature
   void fanMode(uint8_t value);								// Configures the MIC74 fan mode feature

   uint8_t readPortInterrupts();							// Gets a value from the INT_MASK Register
   void writePortInterrupts(uint8_t mask);					// Sets a value to the INT_MASK Register

   void interruptPinOn(uint8_t pin);						// Sets the interrupt-on-change feature to a given GPIO pin 
   void interruptPinOff(uint8_t pin);						// Disables the interrupt-on-change feature to a given GPIO pin

   uint8_t portRead();										// Reads the value from the MIC74 entire GPIO, from 0 to 255
   void portWrite();										// Sets a value to the GPIO Register
   void portWrite(uint8_t value);							// Sets a value to the GPIO Register

   uint8_t digitalRead(uint8_t pin);						// Reads the value from a specified MIC74 pin, either HIGH or LOW
   void digitalWrite(uint8_t pin, uint8_t value);			// Sets a given value to a given GPIO pin
   void digitalWriteDelayed(uint8_t pin, uint8_t value);	// Just write a value to the shadow register

   void pinToHigh(uint8_t pin);								// Sets a given GPIO pin high
   void pinToHighDelayed(uint8_t pin);						// Just sets a given GPIO pin bit on the shadow register
   void pinToLow(uint8_t pin);								// Sets a given GPIO pin to low
   void pinToLowDelayed(uint8_t pin);						// Just clear a given GPIO pin bit on the shadow register

   uint8_t readFanSpeed();									// Gets the fan speed
   void writeFanSpeed(uint8_t speed = 7);					// Sets a fan speed

   uint8_t readStatus();									// Gets the current STATUS register value

/*
    * @ingroup group01
    * @brief Just view STATUS shadow register
    * @details The function can be used to view the previously read STATUS value
    * @param none
    * @return The value of the locally stored STATUS */
   
   inline uint8_t getStatus()
   {
      return this->_status;
   };

/*
    * @ingroup group01
    * @brief Just view DATA shadow register
    * @details The function can be used to view the previously read DATA value
    * @param none
    * @return The value of the locally stored DATA */
   
   inline uint8_t getData()
   {
      return this->_data;
   };

/*
    * @ingroup group01
    * @brief Checks if the Bit Value of a given bit position is set
    * @details This funcion is useful to extract a bit value (0 or 1) from a given MIC74 register
    * @param byteValue byte or register value
    * @param bitNumber bit offset (number/possition from 0 to 7)
    * @return true if the bit position is 1 (set) */
   
   inline bool isBitSet(uint8_t byteValue, uint8_t bitNumber)
   {
      return (byteValue & (1 << bitNumber)) != 0;
   };

/*
    * @ingroup group01
    * @brief Set the Bit Value (1) of a given bit position in a given Byte Value
    * @details This funcion is useful to set a bit value (1) to a given MIC74 register
    * @param byteValue byte or register value
    * @param bitNumber bit offset (number/possition from 0 to 7)
    * @return new Byte Value with set (1) bit in given position */
   
   inline uint8_t bitToSet(uint8_t byteValue, uint8_t bitNumber)
   {
		bitNumber = (1 << bitNumber);
		byteValue |= bitNumber;
		return byteValue;
   };

/*
    * @ingroup group01
    * @brief Clear the Bit Value (0) of a given bit position in a given Byte Value
    * @details This funcion is useful to clear a bit value (0) in a given MIC74 register
    * @param byteValue byte or register value
    * @param bitNumber bit offset (number/possition from 0 to 7)
    * @return new Byte Value with clear (0) bit in given position */
   
   inline uint8_t bitToClr(uint8_t byteValue, uint8_t bitNumber)
   {
		bitNumber = (1 << bitNumber);
		byteValue &= ~bitNumber;
		return byteValue;
   };

};

#endif
