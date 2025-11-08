# Pico Reaction Time Game

A simple reaction time game built for the Raspberry Pi Pico microcontroller in C/C++. Test your reflexes by pressing a button as soon as an LED turns off after a random delay.

<p align="center">
  <img src="https://github.com/user-attachments/assets/bc1bb2a2-c2c2-40c3-bbeb-f8c765605c51" width="45%" />
  <img src="https://github.com/user-attachments/assets/91c87541-9299-43c6-be8d-de899f90983f" width="45%" />
</p>


## How to Play

1. Press the button to start the game.
2. The LED will turn on, then off after a random delay (1-6 seconds).
3. Press the button as quickly as possible when the LED turns off.
4. Repeat for 5 rounds.
5. View your average reaction time on the LCD display.

## Hardware Requirements

- Raspberry Pi Pico
- HD44780 LCD with PCF8574 I2C backpack
- Push button
- LED
- Jumper wires

### Wiring

- Button: GPIO 15
- LED: GPIO 14
- LCD I2C: SDA (GPIO 18), SCL (GPIO 19), Address 0x27
