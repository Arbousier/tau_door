# README

This little program allows to trigger a relay on a SeeedStudios relay board (pin 7) when the /openthedoor action is requested on the specific IP.

This code is based on "Web Server" Examples by "David A. Mellis" and "Tom Igoe" (http://arduino.cc/en/Tutorial/WebServer), modified using Florian Le Goff's code and suggestions, and my personnal needs.

## tailor

You mainly have to change the IPAddress. If you use a different relay setup then you probably should check the const switchOpen.

## Hardware

This code is working with an Arduino Uno Board, an ethernet shield and a 4 Relay Shield from SeeedStudios.
In order to see the relays being triggered you need a 9V power supply connected to the Arduino. Check the SeeedStudio wiki for more : http://seeedstudio.com/wiki/Relay_Shield_V1.0
