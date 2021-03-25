# AVR SVPWM Controller
SVPWM controller prototype implemented using AVR 8-bit microcontroller as a part of EV car conversion graduation project for Mechatronics Engineering Bachelor - Fall 2018.

## Overview
This section gives an overview of the system.

### Concept
Space vector pulse width modulation (SVPWM) is an algorithm for the control of pulse width modulation (PWM).

It is used for the creation of alternating current (AC) waveforms; most commonly to drive 3 phase AC powered motors at varying speeds from DC using multiple MOSFET/IGBT transistors. 

### System Layout
System is divided into two separate modules that communicate with each other to acheive the desired output, which are:

#### Pilot Module

Pilot module receives operating parameters from co-pilot (output frequency) and then outputs PWM signals to IGBT bridge, also it checks for current fault signal and in case of current overload it shuts down waiting for a manual reset.

#### Co-Pilot Module

Co-Pilot module reads user input via rotary potentiometer for output frequency, current sensor for current overload signal, then sends operating parameters to Pilot module and finally outputs system status (frequency, RPM, current,...etc.) to a character LCD.

## Discussion
This section discusses desicions made regarding design and implementation.

### Microcontroller

1. We opted for ATmega128 for pilot module as it runs at 16MHZ frequency and also has two 16-bit timers each having three output compare pins.
2. We opted for ATmega32 for co-pilot module as it runs at 16MHZ frequency and has enough pins for interfacing character LCD and current sensors via ADC.

### Communication Protocol

It is found that best communcation type (protocol) for such system would be a parallel communication bus because of the following reasons:

- Distance between modules is so short (<5m)
- Data sent is time senstive
- Pilot module is starving for CPU clock cycles to calculate and update timer's compare registers values and going with parallel would be ideal for this situation as it only costs one CPU clock cycle to read PINx register 

So we opted for an 8-bit wide parallel communication bus (including READY and ACK signals) which would be much faster than any serial communication available (USART, I2C and SPI).

### Timer Mode

We used Phase and Frequency correct PWM mode with ICR1 as TOP value as it mimics the center aligned PWM mode required for the space vector algorithm.

## Sample Output

### Non-Filtered Output

Below is the ouput of the system without attaching low pass LC filter:

![SVPWM non-filtered](https://user-images.githubusercontent.com/40588058/112097013-fcae1b00-8ba7-11eb-9de0-81aa6ed4a276.gif)

### Filtered Output

Below is the output of the system with low pass LC filter attached:

![SVPWM filtered](https://user-images.githubusercontent.com/40588058/112097103-1e0f0700-8ba8-11eb-857c-8ab76146bc39.gif)

## Built With
- Atmel Studio 7 - The official AVR IDE
- AVR-gcc V5.4 - C compiler
- GNU Make - auto-generated makefile
- CadSoft EAGLE - PCB design tool

## Authors
Loay Ashraf - <loay.ashraf.96@gmail.com>

Mohamed Gamal - <muhammed.gamal051@gmail.com>

## License
This project is licensed under the MIT License - see the LICENSE.md file for details
