# pedalSHIELD MEGA

pedalSHIELD MEGA Nablum is based off of pedalSHIELD MEGA by electrosmash; a lo-fi programmable guitar pedal that works with the Arduino Mega 2560 board. My repository contains all of the information needed to create your own board from scratch unlike pedalSHIELD MEGA who only share effect code.

## How Is this Repository different to the original pedalSHIELD MEGA?

The difference is that my version also has open source PCB files, Protective Case Design files, and anything else needed to make the project yourself (as well as example code of various plugins), unlike electrosmash who claims to be open source but will not share or does not produce certain files (such as PCB files and Protective case designs).

# How Does the Circuit Work?

This  [_hat_](https://www.hifiberry.com/blog/what-is-a-raspberry-pi-hat/) has three parts:

-   Analog Input Stage: The weak guitar signal is amplified and filtered, making it ready for the Arduino MEGA ADC (Analog to Digital Converter).
-   Arduino MEGA Board: It takes the digitalized waveform from the ADC and does all the DSP (Digital Signal Processing) creating effects (distortion, fuzz, volume, delay, etc).
-   The Output Stage: Once the new effected waveform is created inside the Arduino MEGA board, this last stage takes it and using two combined PWMs generates the analog output signal.

![pedalSHIELD MEGA Schematic](https://www.electrosmash.com/images/tech/pedalshield-mega/pedalshield-mega-stages.jpg)

# How to Program it?

To make the programming as easy as possible, the [standard Arduino IDE](https://www.arduino.cc/en/Main/Software) is used. All the effects are programmed on C/C++ (mainly C) using the standard Arduino functions. All tools and programs are free-of-charge/Open Source. The OLED screen uses the [U8glib](https://github.com/olikraus/u8glib) libraries (in the forum there is a tutorial explaining how to [install](http://www.electrosmash.com/forum/pedalshield-mega/290-arduino-mega-u8glib-libraries-installation) the Ug8lib and [use](http://www.electrosmash.com/forum/pedalshield-mega/292-vumeter-on-a-oled-128x64-i2c) them with some examples.)

Basic knowledge of C is needed to understand the codes. The best way to illustrate how to program it is showing a simple example:
-   Clean Volume/Booster Pedal:
![block diagram of the software](https://www.electrosmash.com/images/tech/pedalshield-mega/pedalshield-mega-software-diagram.png)

## Need More Support?

ElectroSmash pedalSHIELD MEGA Website:

- Whilst some may be very outdated, information on how the project works, old schematics and Learning materials can be found [here](https://www.electrosmash.com/pedalshield-mega).