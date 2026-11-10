# STM32 Birthday Card

A handmade birthday card built around an STM32 microcontroller. It combines an OLED display, LED animations, and music in an interactive sequence.

## Gallery

**Photo of the finished card**

*Placeholder: Add a photo here, for example at `docs/images/birthday-card.jpg`.*

**Demo video**

*Placeholder: Link or embed a short clip showing the OLED, LEDs, and buzzer in action.*

## Features

- Start the sequence with a button
- Countdown with a pulsing LED heart animation
- Display text and images on the OLED
- LED running-light and build-up animations
- Play “Happy Birthday” through the buzzer
- Display a running clock

## Hardware

| Component | Implementation |
|---|---|
| Microcontroller | STM32F103CBT6 |
| Display | SSD1306 OLED connected over I²C |
| LEDs | 16 LEDs driven by four PWM channels, using NPN transistors as drivers |
| Buzzer | PWM-controlled |
| Input | One button detected via an external interrupt |
| Construction | Hand-soldered on perfboard |

## Software and operation

The application is organized as a state machine. Timers and PWM control time-based animations and sound output, while a timer interrupt also updates the clock. This lets the main loop coordinate multiple activities without relying on fixed delays for each animation step, keeping the main sequence largely non-blocking.

## Build and flash

The repository includes the STM32CubeIDE/CubeMX project (`.ioc`) and Debug and Release build configurations. Open the project in STM32CubeIDE, build it, and flash the firmware to the microcontroller using a supported debugger/programmer.

## Credits and third-party libraries

- **SSD1306 OLED Driver:** Copyright (C) 2020 Guilherme Amorim & Renan Guedes, licensed under GNU GPLv3.
