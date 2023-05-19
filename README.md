# Arduino-2-Joysticks-to-PPM
Converter for 2 identical Logitech Attack 3 USB Joysticks input to a 8 channel PPM output signal by an Arduino UNO (ATmega328P) with USB Host Shield. These 2 arduino libraries are needed: 
  felis/USB-Host-Shield-20@^1.6.0
	schinken/PPMEncoder@^0.3.0
	
This is the code table for the buttons values, wenn pressed:
Z2	button
value	number
1	1
2	2
4	3
8	4
16	5
32	6
64	7
128	8

Rz
value	button
1	9
2	10
4	11
