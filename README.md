# Microwave Prototype

## Overview

This project implements the control logic behind a microwave oven appliance. The features of focus include: displaying wall clock time, a cooking functionality for a specific time, and a timer functionality. A majority of the underlying complexities in this system stems from managing how each of these functions overlap. The hardware setup consists of a 7-segment LED display, push buttons, breadboard, and an AVR-BLE development board. The user interacts with the system through these buttons, and the wall-clock time is displayed when the system is idle.

## Development Process
(1) Completing the hardware setup:
![circuit diagram](https://drive.google.com/file/d/1GcKOSwWOXI2DNlMywmE-gnp_hehzCk4P/view?usp=sharing)

(2) Implementing the essential features
- Modular design: separating the cooking and timer functionalities from the wall clock display function
- Modes: encoding each **state** of the system as a mode (e.g. COOKING, CLOCK, etc.)
- 7-segment display: utilizing I2C write operations to display time
- Time and countdown display: ensuring accurate calculations, taking into account "roll over" scenarios (e.g. when minute 59 ends, hour increments by 1)
- User input: implementing Interrupt Service Routines (ISRs) to manage button inputs from the user

## Concepts Learned
**I2C communication**: I2C is a two-wire serial communication protocol. In this project, the 7-segment display uses the SDA (serial data line) for transferring the data packets, and SCL (serial clock line) for the bus clock, to receive commands about displaying data. The AVR-BLE dev board has a TWI peripheral, which was used in this project as the I2C host, controlling how the information about either wall clock time or countdown time is displayed.

**RTC and periodic interrupts**: The AVR-BLE contains an RTC peripheral offering periodic interrupt generation functionality. This was used to control wall clock time, and it still actively tracks the current time even when the microwave is being used to cook / when a timer has been set.

**Integration in embedded systems**: implementing each feature and completing unit tests, followed by thorough integration testing to ensure overlaps between the functionalities produce the expected results
