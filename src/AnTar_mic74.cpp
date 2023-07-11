


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
 * @details 4) Internal Interrupt feature setup
 * @author Andrey Tarasenko (AndreiTarasenko1984@gmail.com)
 * @brief It is a Library to control the MIC74 device.
 * @date 2023-05-01
 * @copyright Copyright (c) 2023 Andrey Tarasenko
 */

#include "AnTar_mic74.h"
#include <Wire.h>

/** @defgroup group01 MIC74 basic functions */

/**
 * @ingroup group01
 * @brief Look for MIC74 device I2C Address
 * @details This method will look for a valid MIC74 device adress between 0x20 and 0x27 
 * @return uint8_t the I2C address of the first MIC74 device connect in the I2C bus
 */
uint8_t MIC::lookFor() {
    int err = 0;
    Wire.begin();
    for (int addr = 0x20; addr <= 0x27; ++addr)
    {
        Wire.beginTransmission(addr);
        err = Wire.endTransmission();
        if (err == 0)
            return addr;
    }
    // Any MIC74 device was found
    return 0;
}

/**
 * @ingroup group01
 * @brief Starts the MIC74 
 * @details Starts the MIC74 with default values.
 */
void MIC::begin(uint8_t i2cAddress, long i2cFrequency)
{
    Wire.begin(); 						//creates a Wire object
	Wire.setClock(i2cFrequency);
    this->_i2cAddress = i2cAddress;
}

/**
 * @ingroup group01
 * @brief Setup the MIC74
 * @details Setup the MIC74 and it's registers. 
 * @param i2cAddress I2C address (0x20 ~ 0x27) - default 0x27; ports input/output setup; and I2C clock frequency
 * @param direction  If GPIO_OUTPUT (255), all  GPIO PINS will configured to output
 *					If GPIO_INPUT  (0), all GPIO PINS will configured to input  
 *            		You also can use a bitmask to configure some pins for input and other pins for output.
 * @param i2cFreq set the I2C bus frequency/speed (default 100000 = 100KHz)
 */
/* void MIC::setup(uint8_t dir, uint8_t intMask, uint8_t outCfg, uint8_t data)
{
	if(data != PORT_SET)
	{
		this->setReg(REG_DATA, data);		// Sets levels of pins
		this->_data = data;
	}

	if(outCfg != PORT_CLR) this->setReg(REG_OUT_CFG, outCfg);	// Selects output driver configuration

	if(dir != PORT_CLR) this->setReg(REG_DIR, dir);		// Selects data direction, input or output

    this->setReg(REG_DIR, dir);    	// All GPIO pins are configured to input (0)  or output (1)
    
} */

/** @defgroup group02 MIC74 IO functions */

/**
 * @ingroup group02
 * @brief Gets the current register information. 
 * @details Gets the current register content. 
 * @param reg  (0x00 ~ 0x06) see MIC74 registers documentation 
 * @return uint8_t current register value
 */
uint8_t MIC::getReg(uint8_t reg) {
    Wire.beginTransmission(this->_i2cAddress);
    Wire.write(reg);
    Wire.endTransmission();
    Wire.requestFrom((int) this->_i2cAddress, (int) 1);
    return Wire.read();
}

/**
 * @ingroup group02
 * @brief Sets a given value (HIGH(1) or LOW(0)) to a given GPIO pin
 * @details It is like the pinHigh() or pinLow()
 * @param gpio pin number
 * @param value 1 = High;  0 = Low; or HIGH and LOW
 */
void MIC::pinMode(uint8_t pin, uint8_t mode)
{
    if(pin > 7) return;

	uint8_t dir_mask = 0, another_mask = 0;

	if(mode == INPUT_WITH_INTERRUPT || mode == OUTPUT_PUSHPULL) another_mask = 1;

	if(mode == INPUT || mode == INPUT_WITH_INTERRUPT)
	{
		another_mask = this->getReg(REG_INT_MASK) & ~(1 << pin) | (another_mask << pin);
		this->setReg(REG_INT_MASK, another_mask);
	}
	else if(mode == OUTPUT || mode == OUTPUT_PUSHPULL)
	{
		dir_mask = 1;
		another_mask = this->getReg(REG_OUT_CFG) & ~(1 << pin) | (another_mask << pin);
		this->setReg(REG_OUT_CFG, another_mask);
	}

	dir_mask = this->getReg(REG_DIR) & ~(1 << pin) | (dir_mask << pin);
    this->setReg(REG_DIR, dir_mask);
}

   /**
     * @ingroup group02
     * @brief Returns the value of STATUS register 
     * @details The STATUS register reflects the input-change event on the port pins of any pin that is configured as input.
     * @return uint8_t value of STATUS register
     */
   uint8_t MIC::getStatus()
   {
      this->_status = getReg(REG_STATUS);
      return this->_status;
   }

   /**
   * @ingroup group02
   * @brief Returns the current MIC GPIO pin levels 
   * @param none
   * @return uint8_t 
   */
   uint8_t MIC::portRead()
   {
      this->_data = getReg(REG_DATA);
      return this->_data;
   }

   /**
   * @ingroup group02
   * @brief Returns the current fan speed level
   * @param none
   * @return speed level from 0 to 7
   */
   uint8_t MIC::getFanSpeed()
   {
      return this->getReg(REG_FAN_SPEED);
   }

/**
 * @ingroup group02
 * @brief Sets a value to a given register
 * @details Sets a given 8 bit value to a given register.  
 * @param reg   (0x00 ~ 0x06 exclude 0x03) see MIC74 registers documentation 
 * @param value (8 bits)
 */
void MIC::setReg(uint8_t reg, uint8_t value) {
    Wire.beginTransmission(this->_i2cAddress);
    Wire.write(reg);
    Wire.write(value);
    Wire.endTransmission(); //ends communication with the device
}

   /**
   * @ingroup group02
   * @brief Sets a value to the GPIO Register
   * @details A direct way to set a given value to deal with the GPIOs pins.
   * @param value (8 bits)
   */
   void MIC::portWrite(uint8_t value)
   {
       this->setReg(REG_DATA, value);
	   this->_data = value;
   }

   /**
   * @ingroup group02
   * @brief Sets a value to the DIR Register
   * @details A direct way to set a given value to deal with the GPIOs pins.
   * @param value (8 bits)
   */
   void MIC::setPortMode(uint8_t value)
   {
       this->setReg(REG_DIR, value);
   }

   /**
   * @ingroup group02
   * @brief Gets a value from the DIR Register
   * @details A direct way to get a value to deal with the GPIOs pins.
   * @param none
   * @return value (8 bits)
   */
   uint8_t MIC::getPortMode()
   {
       return this->getReg(REG_DIR);
   }

   /**
   * @ingroup group02
   * @brief Sets a value to the OUT_CFG Register
   * @details A direct way to set a given value to deal with the GPIOs pins.
   * @param value (8 bits)
   */
   void MIC::setPortOutMode(uint8_t value)
   {
       this->setReg(REG_OUT_CFG, value);
   }

   /**
   * @ingroup group02
   * @brief Gets a value from the OUT_CFG Register
   * @details A direct way to get a value to deal with the GPIOs pins.
   * @param none
   * @return value (8 bits)
   */
   uint8_t MIC::getPortOutMode()
   {
       return this->getReg(REG_OUT_CFG);
   }

   /**
   * @ingroup group02
   * @brief Sets a interrupts mask to the INT_MASK Register
   * @details A direct way to set a given value to deal with the GPIOs pins.
   * @param mask (8 bits)
   */
   void MIC::setPortInterrupts(uint8_t mask)
   {
       this->setReg(REG_INT_MASK, mask);
   }

   /**
   * @ingroup group02
   * @brief Gets a interrupts mask from the INT_MASK Register
   * @details A direct way to get a value to deal with the GPIOs pins.
   * @param none
   * @return mask (8 bits)
   */
   uint8_t MIC::getPortInterrupts()
   {
       return this->getReg(REG_INT_MASK);
   }

   /**
   * @ingroup group02
   * @brief Sets a fan speed
   * @details Way to set a given speed to fan speed register.
   * @param speed from 0 to 7 (3 bits)
   */
   void MIC::setFanSpeed(uint8_t speed)
   {
	   if(speed > 7) speed = 7;
       this->setReg(REG_FAN_SPEED, speed);
   }

   /**
   * @ingroup group02
   * @brief Sets the shadow buffer to the GPIO Register
   * @details Physical writing of the shadow buffer to the port register for delayed functions
   * @param none
   */
   void MIC::portWrite()
   {
       this->portWrite(_data);
   }

/**
 * @ingroup group02
 * @brief Turns a given GPIO pin on (high level)
 * @details Sets a given GPIO pin high
 * @param pin the GPIO PIN number (0-7)
 */
void MIC::pinHigh(uint8_t pin)
{
    if(pin > 7) return;
    pin = (1 << pin);
    this->_data = this->getReg(REG_DATA) | pin;
    this->setReg(REG_DATA, this->_data);
}

/**
 * @ingroup group02
 * @brief Sets a given GPIO pin bit (high level)
 * @details Just sets a given GPIO pin bit on the shadow register and do not send it to the chip
 * @param pin the GPIO PIN number (0-7)
 */
void MIC::pinHighDelayed(uint8_t pin)
{
    if(pin > 7) return;
    pin = (1 << pin);
    this->_data |= pin;
}

/**
 * @ingroup group02
 * @brief Turns a given GPIO pin off (low level)
 * @details Sets a given GPIO pin to low
 * @param pin the GPIO PIN number (0-7)
 */
void MIC::pinLow(uint8_t pin)
{
    if(pin > 7) return;
    pin = (1 << pin);    
    this->_data = this->getReg(REG_DATA) & ~pin;
    this->setReg(REG_DATA, this->_data);
}

/**
 * @ingroup group02
 * @brief Clear a given GPIO pin bit (low level)
 * @details Just clear a given GPIO pin bit on the shadow register and do not send it to the chip
 * @param pin the GPIO PIN number (0-7)
 */
void MIC::pinLowDelayed(uint8_t pin)
{
    if(pin > 7) return;
    pin = (1 << pin);    
    this->_data &= ~pin;
}

/**
 * @ingroup group02
 * @brief Reads the status (high or low) of a given PIN
 * @details Returns true if the pin is hight or false if it is low.
 * @param gpio pin number
 * @returns true if it is High
 */
uint8_t MIC::digitalRead(uint8_t pin) {
    if(pin > 7) return LOW;
    if(getReg(REG_DATA) & (1 << pin)) return HIGH;
	return LOW;
}

/**
 * @ingroup group02
 * @brief Sets a given value (HIGH(1) or LOW(0)) to a given GPIO pin
 * @details It is like the pinHigh() or pinLow()
 * @param GPIO pin number
 * @param value 1 = High; 0 = Low; or HIGH and LOW
 */
void MIC::digitalWrite(uint8_t pin, uint8_t value) {
    if(pin > 7) return;
	if(value != LOW) value = 1;
    this->_data = this->getReg(REG_DATA) & ~(1 << pin) | (value << pin);
    this->setReg(REG_DATA, this->_data);
}

/**
 * @ingroup group02
 * @brief Sets a given value (high(1) or low(0)) to a given GPIO pin delayed
 * @details It is just write a value to the shadow register and do not send it to the chip
 * @param GPIO pin number
 * @param value 1 = High;  0 = Low; or HIGH and LOW
 */
void MIC::digitalWriteDelayed(uint8_t pin, uint8_t value) {
    if(pin > 7) return;
	if(value != LOW) value = 1;
    this->_data = this->_data & ~(1 << pin) | (value << pin);
}

/**
 * @ingroup group02
 * @brief Reads the status (high or low) of a given bit (position) of a given MIC74 register
 * @details Returns true if the bit of the register is hight or false if it is low.
 * @param bit_position bit position 
 * @returns true if it is High
 */
bool MIC::regBitRead(uint8_t mic_register, uint8_t bit_position)
{
    if(bit_position > 7) return false;
    return getReg(mic_register) & (1 << bit_position);
}

/**
 * @ingroup group02
 * @brief Sets High or Low to a given position in a given MIC74 register 
 * @details Sets a given bit value to a given position in a given MIC74 register  
 * @param mic_register MIC74 register
 * @param bit_position bit position
 * @param value 0 = Low; 1 = High
 */
void MIC::regBitWrite(uint8_t mic_register, uint8_t bit_position, uint8_t value)
{
    if(bit_position > 7) return;
    uint8_t currentRegisterValue = this->getReg(mic_register); // Gets the current register value
    this->setReg(mic_register, (currentRegisterValue & ~(1 << bit_position)) | (value << bit_position));
}

/**
 * @ingroup group02
 * @brief Selects output driver configuration to a given GPIO PIN as push-pull
 * @details Activates the push-pull to a given GPIO pin
 * @param pin the GPIO PIN number (0-7)
 */
void MIC::pushPullPinOn(uint8_t pin)
{
    if(pin > 7) return;

    uint8_t gppp;
    gppp = this->getReg(REG_OUT_CFG); // Gets the current values of push-pull setup
    gppp |= 1 << pin;
    this->setReg(REG_OUT_CFG, gppp); // Updates the values of push-pull setup
}

/**
 * @ingroup group02
 * @brief Selects output driver configuration to a given GPIO PIN as open-drain
 * @details Deactivates the push-pull to a given GPIO pin
 * @param pin the GPIO PIN number (0-7)
 */
void MIC::pushPullPinOff(uint8_t pin)
{
    if(pin > 7) return;

    uint8_t gppp;
    gppp = this->getReg(REG_OUT_CFG); // Gets the current values of push-pull setup
    gppp &= ~(1 << pin);
    this->setReg(REG_OUT_CFG, gppp); // Updates the values of push-pull setup
}

/**
 * @ingroup group02
 * @brief Sets the DEV_CFG register
 * @details The DEV_CFG register contains several bits for configuring the device:
 * @details The Global Interrupt Enable (IE) control bit sets the Interrupts.
 * 
 * @param IE	This bit sets the Global Interrupt Enable. Operation: 1 (ON) = enabled, 0 (OFF) = disabled.
 * @param FAN	This bit Selects fan mode. Operation: 1 (ON) = fan mode; 0 (OFF) = I/O mode.
 */
void MIC::setup(uint8_t ie, uint8_t fan)
{
	if(ie != OFF) ie = 1;
	if(fan != OFF) fan = 1;
    uint8_t devcfg = this->getReg(REG_DEV_CFG); // Gets the current value of the REG_DEV_CFG register
	devcfg = devcfg & ~(1 << BIT_IE) | (ie << BIT_IE);
	devcfg = devcfg & ~(1 << BIT_FAN) | (fan << BIT_FAN);
    this->setReg(REG_DEV_CFG, devcfg); // Write the new REG_DEV_CFG register value
}

/**
 * @ingroup group02
 * @brief Configures the MIC74 global interrupt feature.
 * @details The ALERT output pin will be activated when an internal interrupt occurs.
 * @details Active-low, open-drain output signals input-change-interrupts to the host on this pin. 
 * @param value - Global Interrupt Enable. Operation: 1 (ON) = enabled, 0 (OFF) = disabled.
 */
void MIC::setInterrupts(uint8_t value)
{
	if(value != OFF) value = 1;
    uint8_t devcfg = this->getReg(REG_DEV_CFG); // Gets the current value of the REG_DEV_CFG register
	devcfg = devcfg & ~(1 << BIT_IE) | (value << BIT_IE);
    this->setReg(REG_DEV_CFG, devcfg); // Write the new REG_DEV_CFG register value
}

/**
 * @ingroup group02
 * @brief Configures the MIC74 fan mode feature.
 * @details If the chip is configured for fan control operation, the P[7:4] pins are automatically configured as open drain outputs.
 * @param value - Global Interrupt Enable. Operation: 1 (ON) = enabled, 0 (OFF) = disabled.
 */
void MIC::fanMode(uint8_t value)
{
	if(value != OFF) value = 1;
    uint8_t devcfg = this->getReg(REG_DEV_CFG); // Gets the current value of the REG_DEV_CFG register
	devcfg = devcfg & ~(1 << BIT_FAN) | (value << BIT_FAN);
    this->setReg(REG_DEV_CFG, devcfg); // Write the new REG_DEV_CFG register value
}

/**
 * @ingroup group02
 * @brief Sets the interrupt-on-change feature to a given GPIO pin 
 * @details The REG_INT_MASK register controls the interrupt-on-change feature for each pin.
 * @details If a bit is set, the corresponding pin is enabled for interrupt-on-change.
 * @details if you want to configure more than one GPIO PIN at once, use the setPortInterrupts(mask);
 * @details If enabled (via REG_INT_MASK and REG_DEV_CFG) changing the logic level on the associated pin will cause an interrupt to occur.
 * @param pin GPIO PIN you want to configure
 */
void MIC::interruptPinOn(uint8_t pin) 
{
    if(pin > 7) return;

    uint8_t mask;
    // Enables the GPIO pin to deal with interrupt  
    mask = this->getReg(REG_INT_MASK); // Gets the current values of REG_INT_MASK
	mask |= 1 << pin;
    this->setReg(REG_INT_MASK, mask); // Updates the values of the REG_INT_MASK register
}

/**
 * @ingroup group02
 * @brief Disables the interrupt-on-change feature to a given GPIO pin
 * @details The REG_INT_MASK register controls the interrupt-on-change feature for each pin.
 * @details If a bit is clear, the corresponding pin is disabled for interrupt-on-change.
 * @details if you want to configure more than one GPIO PIN at once, use the setPortInterrupts(mask);
 * @details If disabled, changing the logic level on the associated pin will not cause an interrupt to occur.
 * @param pin GPIO PIN you want to configure
 */
void MIC::interruptPinOff(uint8_t pin)
{
    if(pin > 7) return;

    uint8_t mask;
    // Disables the GPIO pin to deal with interrupt  
    mask = this->getReg(REG_INT_MASK); // Gets the current values of REG_INT_MASK
	mask &= ~(1 << pin);
    this->setReg(REG_INT_MASK, mask); // Updates the values of the REG_INT_MASK register
}
