# arduino-snake
Snake game in arduino using a TFT ILI9163C

# Table of contents
* [Introduction](# Introduction)
* [Requirements](# Requirements)
* [Wiring diagram](# Wiring)
* [Contributing](# Contributing)

## Introduction
The snake game is a very popular game, and you must of heard of it. The game is simple, you are a snake that eats fruits to grow its length. if you touch your tail then the game is over. So i made it in arduino with a TFT ILI9163C, joystick and a buzzer.

## Requirements
You will need some libraries:
* [Sumotoy's ILI9163C](https://github.com/sumotoy/TFT_ILI9163C)
* [Sumotoy's Adafruit](https://github.com/sumotoy/Adafruit-GFX-Library)
* SPI (Arduino library)

You will also need some electronics:
* Arduino Mega
* TFT ILI9163C
* Joystick module
* Buzzer
* Some Jumper wires

## Wiring
The wiring diagram is simple:

### ILI9163C
VCC -> 5V
GND -> GND
DI -> pin 51
SC(SCK) -> pin 52
CS -> pin 53
RST -> 3V3
RS(DC) -> 9
BLK(LED) -> 3V3

### Joystick
VCC -> 5V
GND -> GND
VRX -> A1
VRY -> A2
SW -> 7

### Buzzer
GND -> GND
Anode -> 6

## Contributing
If you want to discuss about something you would like to change contact me on my [discord server](https://discord.gg/HAGQFjr)
