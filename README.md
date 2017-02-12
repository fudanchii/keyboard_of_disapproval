# Keyboard of Disapproval ಠ_ಠ

First of all, I guess you wouldn't want to build your own keyboard,  
but anyway, here it is:

<img src="https://f4.fudanchii.net/shx/2017/02/IMG_20170212_224547.jpg"></img>

In case anyone curious, it's built on scrap collected over the past 8 months, the plate is metal frame with 60% layout (GH-60) from aliexpress,
switches is gateron blue bought from Massdrop, the key caps is randomly bought from local gaming store, 
and the controller is arduino micro clone from Robotdyn.

## The Name

Considering how much time, effort, and money poured on this, I don't think anyone will approve.  
Also, naming is hard.

## The build

The switches are all hand-wired with 8x8 matrix, so it's only use 16 pins. Matrix layout [here](https://github.com/fudanchii/keyboard_of_disapproval/blob/master/arduino_micro/keymap.c).  
 Currently there is only one base keymap, with `[Fn]` modifier to accomodate arrows, `[pageUp]`-`[PageDown]`, `[Esc]`, `[Home]`-`[End]`, and `[Insert]`. (See todo below)

## The Firmware
Please change the makefile according to your environment setup, run `make` to build the firmware. Run `make avrdude` to flash the firmware.
Previously I plan to build the keyboard on ATTiny167 based Digispark Pro, but it's in incomplete state as of current.

## Bill of Material

Here is BOM (price in USD):

Name | Store | Qty | Price (total) | Shipping cost | Link
--- | --- | --- | --- | --- | ---
Metal Frame Plate GH-69 layout + stabilizer (unused) | KBDFans Store (AliExpress) | 1 | $22 | $13.39 | [AliExpress](https://www.aliexpress.com/item/Steel-plate-GH60-mechanical-keyboard-poker2-electro-coating-steel-plate-gaming-keyboard-FACEU-keyboard-metal-case/32722949125.html)
Gateron Blue Switches | Massdrop | 120 pcs | $24.99 | $9.75 | [Massdrop](https://www.massdrop.com/buy/23991)
Novelties keycaps | Massdrop | 6pcs | $19.98 | $6.50 | [Massdrop](https://www.massdrop.com/buy/25017)
PBT OEM profile Keycaps | Local gaming store | 1 pack | $20 | - | -
1N4001 diode | LisuInstrument (Tokopedia) | 61 | $1 | $0.6 | [Tokopedia](https://www.tokopedia.com/lisuinstrument/1n4001-do-41-in4001-1a-50v-rectifie-diode)
Enameled copper wire (0.5mm) 0.5Kg | PCM Elektronik (Tokopedia) | 1 | $6.38 | $0.3 | [Tokopedia](https://www.tokopedia.com/pcmjakarta/kawat-email-tembaga-01-02-03-04-05-06-08-1mm-2mm-3mm-pilih-aja)
Arduino Micro (clone) | RobotDyn | 1 | $3.85 | $1.41 | [RobotyDyn](http://robotdyn.com/catalog/boards/micro_atmega32u4_mu/)

## Todo

- LEDs
- More layers?
- Special layer for input macro (shortcut to input some text)
- Proper mounting/housing for the controller

## Legals

Copyright 2017 Nurahmadie <nurahmadie@gmail.com>

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.


ಠ_ಠ
