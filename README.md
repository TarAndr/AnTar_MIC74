Documentation made in [Open source online Markdown editor](https://pandao.github.io/editor.md/en.html "Open source online Markdown editor") and converted to HTML in [Free Markdown to HTML Converter](https://markdowntohtml.com/ "Convert Markdown to HTML").

## [AnTar MIC74 Arduino Library](https://github.com/TarAndr/AnTar_MIC74)

It is an Arduino Library to control the MIC74 8-Bit I/O Expander. 

The MIC74 device provides 8-bit, general purpose, parallel I/O expansion. It can be controlled via I²C bus applications. It is a great and inexpensive device that allow you to add more peripherals to be controlled by your Arduino board via I²C protocol.

Some Arduino projects may require more pins than the Arduino actually has. In these cases, you can use up to 8 MIC74 devices using only the I²C bus (two Arduino pins) and add up to 64 input / output ports to your project. 


This Arduino library implements the most important functions available on MIC74 device  and can be freely distributed using the MIT Free Software model. 

[Copyright (c) 2023 Tarasenko Andrey](LICENSE).

**Contents**

[TOCM]

[TOC]

- [AnTar MIC74 Arduino Library](https://github.com/TarAndr/AnTar_MIC74 "AnTar MIC74 Arduino Library")

1. [MIT License](#mit-license)
1. [Thanks](#thanks)
1. [Library Features](#library-features)
1. [Demo Video](#demo-video)
1. [Library Installation](#library-installation)
1. [MIC74 brief characteristics](#Brief-characteristics-of-the-MIC74-chip)
1. [MIC74 registers](#Description-and-purpose-of-registers-of-the-MIC74)
1. [API documentation](https://tarandr.github.io/MIC74/extras/apidoc/html/)
1. [Basic Schematic](https://tarandr.github.io/MIC74/#basic-schematic)
1. [Internal Interrupt setup](https://tarandr.github.io/MIC74/#internal-interrupt-setup) 
1. [References](https://tarandr.github.io/MIC74/#references)


### MIT License 

Copyright (c) 2023 Andrey Tarasenko

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

### Thanks

* Mrs. [Ricardo Lima Caratti](https://github.com/pu2clr) the [MCP23008](https://github.com/pu2clr/MCP23008/) library he created was taken as a template.

### Library Features

This library uses the I²C communication protocol and implements most important functions offered by MIC74 device from Microchip. It also has primitive functions that make it easier to implement commands that may not have been implemented yet. The main features implemented can be seen below:

* GPIO individual control (8 I/O pins)
* Reading and writing Access to all registers (0x00 ~ 0x06) 
* I²C address customization (0x20 ~ 0x27)
* Output mode configuration (push-pull or open-drain)
* Internal Interrupt feature setup
* Fan speed control

### Demo Video 

{% include video01.html %}

[See on Youtube](https://www.youtube.com/watch?v=muUAhf5DGE8)
<BR>

### Library Installation

You can install this library on your Arduino environment using different methods. The best ways to do that are described below.  

#### Installing via Arduino IDE

This is the easiest method to install this library.

On Arduino IDE, select the __"Tools" >> Manage Libraries..."__ item and look for MIC74. Finally select "AnTar MIC74". The images below show the installation process. 


##### Going to Manage libraries...

![Manage libraries](extras/images/00_MIC74.jpg)


##### Looking for MIC74

![Looking for MIC74](extras/images/00_MIC74.jpg)


##### Checking the AnTar MIC74 examples

![Checking examples](extras/images/00_MIC74.jpg)


##### More information

See also [Installing Libraries](https://docs.arduino.cc/software/ide-v1/tutorials/installing-libraries)



#### Installing via this repository 

First, you have to [download](https://github.com/TarAndr/AnTar_MIC74/archive/main.zip) this library in zip format. 
After, unzip the AnTar_MIC74-main.zip file in your Arduino Library folder:

* On __Windows__: "My Documents\Arduino\libraries"
* On __MAC OS__: ˜/Documents/Arduino/libraries
* On __Linux__: ˜/Documents/Arduino/libraries

With that approach, you will have the __most current version__ of the library. However, __it may not be the most stable version__. This is because the current version is always in development.
Do you need some old version (release) of this library?  If yes, [check here](https://github.com/TarAndr/AnTar_MIC74/releases). 


### Brief characteristics of the MIC74 chip

The MIC74 chip is a fully programmable serial-to-parallel I/O expander converter that is compatible with the SMBus™ protocol. It operates as a bus client providing eight independent I/O lines.

Each I/O bit can be individually programmed as an input or output. If any pin is programmed as an output, then each such pin can be programmed as an open-drain output or a push-pull output. If desired, four outputs can be programmed to implement fan speed control. An internal clock and status system eliminates the overhead required to control fan speed.

The output pins are capable of directly driving loads such as LEDs. It is possible to use interrupts during state changes on the input pins. This eliminates the need to poll the device each time to obtain pin status information. Three address selection inputs are provided, allowing up to eight devices to be connected and shared on the same bus, providing a total of 64 additional I/Os to your microcontroller.

The MIC74 chip is available in an ultra-compact 16-pin QSOP package. Low quiescent current, small footprint and low chassis height make the MIC74 ideal for portable and desktop applications:

![MIC74 housing type and pinout](extras/images/00_MIC74-2-Wire-Serial-IO-Expander-and-Fan-Controller.png "MIC74 housing type and pinout")

- A0, A1, A2 - Client address setting inputs, the three least significant bits of the MIC74 chip address are set;
- P0, P1, P2, P3, P4, P5, P6, P7 - General purpose I/O contacts, direction and type of output user programmable;
- /ALERT - An output with an active low level and an open drain, an interrupt output signal for the host when the logic level changes at the inputs of the microcircuit. The signal is cleared when the bus host polls the ARA (alert response address = 0001100) or reads the status;
- CLK - The host provides the clock clock on this input;
- DATA - Serial data input and serial data output with open drain.

### Description and purpose of registers of the MIC74

For configuration and control, the MIC74 chip has 7 registers, each of which consists of 8 bits:

| Register | Address | Description |
| -------- | ----- | ------------|
| DEV_CFG  | 0x00  | Device configuration read/write register|
| DIR  | 0x01  | Data direction read/write register|
| OUT_CFG  | 0x02  | Output configuration read/write register|
| STATUS   | 0x03  | Status read register|
| INT_MASK   | 0x04  | Interrupt mask read/write register|
| DATA    | 0x05  | Data read/write register|
| FAN_SPEED   | 0x06  | Fan speed read/write register, Determines bit-pattern on FS[2:0]|

- DEV_CFG - Chip configuration register, the two least significant bits of which set operating modes, such as global interrupt enable and fan control. The remaining bits are reserved and must always be set to zero. After the chip starts up, by default all bits of this register are set to zero, meaning that interrupts and fan control mode are disabled. To enable interrupts, you need to set the zero bit, and to control the fan, set bit number 1 to one, for example, by writing the value 0b00000011 to the register. The register is intended for both writing and reading.;
- DIR - Register for setting all or individual pins to work as inputs or outputs. Once enabled, the register has a default value of 0b00000000 and all pins are configured as inputs. Accordingly, to configure a separate pin to operate as an output, the corresponding bit of this register must be set to one. You can also read set values from the register. If the chip is configured for fan control, the P[7:4] pins are automatically configured as open-drain outputs, and the DIR register does not affect the operation of these pins;
- OUT_CFG - Register for setting all or individual pins configured as outputs to operate in push-pull or open-drain mode. The default register value is 0b00000000 and all output stages are configured to operate in open drain mode. Accordingly, to configure a separate output to operate in push-pull mode, the corresponding bit of this register must be set to one. You can also read set values from the register. If the chip is configured for fan control, the P[7:4] pins are automatically configured as open-drain outputs, and the OUT_CFG register does not affect the operation of these pins;
- STATUS - The bits of this register record events of changes in the logical level at the corresponding pins of the microcircuit, configured as inputs. The bit is set to one when the logic level changes at the corresponding input, and if no change occurs, the bit is reset to 0. The register is read-only, and all its bits are reset to 0 after this operation is performed. If the chip is configured for fan control, the P[7:4] pins are automatically configured as open-drain outputs, and no interrupts are generated by changing the logic levels on these pins;
- INT_MASK - Interrupt enable register on the required pins configured as inputs, and the corresponding bits of which are set to one in this register. The register is used for reading/writing. If the chip is configured for fan control, the P[7:4] pins are automatically configured as open-drain outputs, and no interrupts are generated by changing the logic levels on these pins;
- DATA - The register displays the current state of any pin configured as an input and the last value applied to a pin configured as an output. Writing a value to the DATA register sets the state of any pin configured as an output; writes to I/O bits configured as inputs are ignored. By default, all register bits are set to one. If the chip is configured for fan control, the P[7:4] pins are automatically configured as open drain outputs and their logic levels are controlled by the FAN_SPEED register, and the DATA register does not affect the operation of these pins;
- FAN_SPEED - Fan operation control register, in which the three least significant bits are used. Any time the register contains a value of zero, it means the fan is off, and if a non-zero value is written to it, the /FS[2:0] and /SHDN outputs will go to the maximum fan speed state for approximately one second (tSTART). After this interval, the state of the fan speed control outputs will take the value specified in the contents of the FAN_SPEED register. This ensures that the fan starts reliably even when low speed operation is required. The dependence of the fan rotation speed on the value of the low-order bits of the register is shown in the following table, with “Speed 1” corresponding to the lowest, and “Speed 7” the highest actual fan rotation speed.

### Functions

This library has two categories of functions that you can use to control everything on the MIC74. The first category includes configuration functions and is used less frequently, while the second category consists of control functions and is used more often. This library also has other auxiliary functions that simplify the creation of applications based on Arduino and MIC74 devices. See [API documentation](https://tarandr.github.io/MIC74/extras/apidoc/html/).


#### Configuration functions

Function for setting the pin direction and operating mode of the output:

**pinMode(*pin, mode*);**

- **pin**: port pin 0 to 7;
- **mode**: the desired operating mode of the specified INPUT, OUTPUT, INPUT_WITH_INTERRUPT or OUTPUT_PUSHPULL pin.

This function interacts with three registers at once and, in addition to specifying the output direction, can set an interrupt mask for it, in the case of setting the output to an input, or a push-pull mode for the output stage, in the case of setting the output to an output. In the case of simply specifying a pin to operate as an input, interrupts for it will be disabled, and for operation as an output, the operating mode of the output stage will be set to open drain.

Function to configure the entire port at once

**writePortMode(*value*);**

- **value**: a specified value, which for clarity is conveniently represented as a bit field 0bxxxxxxxx.

Where in the required positions “0” or “1” is written instead of “x”, for example writePortMode(0b11111111); will simultaneously set all pins of the chip to the output direction, and writePortMode(0b00000000); accordingly to the entrance.


#### Control functions



### Basic schematic

The image below shows a basic MIC74 application with LED. You can control up to 8 LEDs. The I²C bus address is set to 0x27. You can select another I²C address by dealing with the A0, A1 and A2 pins (from 0x20 to 0x27). This circuit uses the MIC74 GPIO PINs as output.   

#### GPIO as output setup

![Basic Schematic with LEDs](extras/images/MIC74_LEDs.GIF)




Also this library has other functions that make the job easier to build applications. See [API documentation](https://www.youtube.com/watch?v=muUAhf5DGE8).



