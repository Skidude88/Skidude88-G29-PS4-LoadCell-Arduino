# Skidude88-G29-PS4-LoadCell-Arduino
DIY load cell conversion for Logitech G29 on PS4 using Arduino Pro Micro

This Project was developed in conjunction with an analog version by GeekyDeaks

https://github.com/GeekyDeaks/g29-load-cell

# Initial Shopping List:

Arduino Pro Micro - Ideally 3.3v version. This project used the 5v as a proof of theory.

'Bathroom Scales' style Load Cell(s)

Load Cell Amplifier : HX711

# Further Requirements

Low Pass Filter:

1x Resistror

1x Capacitor

For single loadcell configuration:

2x 1k resistors

# Other Project Requirements
A Mate with a 3D printer & see GeekyDeaks project above for CAD designs.


# HX711

Input Pins from Load Cells:

E+ (Excite +)

E- (Excite -)

A+ (Amplifier/Output + Channel A)

A- (Amplifier/Output - Channel A)

B+ (Amplifier/Output + Channel B) - Not used for this project

B- (Amplifier/Output - Channel B) - Not used for this project

Output Pins:

SCK - Clock

DAT - Data

Vcc/Gnd - Power/Ground

The HX711 board can run at either 10Hz (By default) or 80hz. Some boards have a select jumper to solder/break track. To acheive 80Hz without this selector (on cheaper version boards) the Rate pin requires +v. Cut the track immediatly next to pin15 "Rate" (Some forum posts needlessly go to the trouble of lifting pin15 pin off the board!) and then solder pin15 to pin16 (Vcc) directly above. 



# Software
Arduino IDE (Desktop) v1.8.10
https://www.arduino.cc/en/main/software

# Dual Load Cells

Initially 2x Load cells were used, each being half of a Wheatstone Bridge

LoadCell#1 - White wire, Connect to LoadCell#2 - Black wire

LoadCell#2 - White wire, Connect to LoadCell#1 - Black wire

These are E+ & E-

Then the remaining 2x red wires are A+ & A-

The HX711 has two channels, A & B, this project is using A, so connect to A+ & A-

Note for the Arduino sketch file: 

The hx711.h library file details the .begin command having a gain parameter. If not specified, a default of 128 is used : Channel A

	  	// Initialize library with data output pin, clock input pin and gain factor.
		// Channel selection is made by passing the appropriate gain:
		// - With a gain factor of 64 or 128, channel A is selected
		// - With a gain factor of 32, channel B is selected
		// The library default is "128" (Channel A).
		void begin(byte dout, byte pd_sck, byte gain = 128);
		
If the load cell doesn't perform as expect, switch around either the A+ and A- (red wires) or the E+ and E- (Either option is doing the same thing).		

Usefull Info:

https://learn.sparkfun.com/tutorials/load-cell-amplifier-hx711-breakout-hookup-guide?_ga=2.166487294.1982490767.1578266625-357928328.1568243893


https://www.instructables.com/id/How-to-Interface-HX711-Balance-Module-With-Load-Ce/


# Single Load Cell

One of the Loadcells is replaced by 2x 1K resistors to complete the 2nd half of the Wheatstone Bridge.

Usefull Info: Step 2 on the link below shows the relevant connections between the remaining loadcell and resistors.

https://www.instructables.com/id/Tutorial-to-Interface-HX711-With-Load-Cell-Straigh/



# G29 Pedal (more details on GeekyDeaks project)

Pedal off = 3.0v

Pedal on  = 1.9v


Suppliying PWM from the Micro Pro didn't work, so a  basic passive low pass filer was added.
Consisting of :

1x 2.2uF Capacitor
1x 4.7k resistor

Usefull info:

https://provideyourown.com/2011/analogwrite-convert-pwm-to-voltage/

# Arduino Pro Micro 5v 16Hz

Requires USB power,so usefull to have a PC close to Pedals for (programming and power)

Use any of Data pins to receive the HX711 CLK and Data signals (any two of the 18 "Blue" pins on link below). In this project pin15 was used for Data, pin14 for CLK.

The get_value data received from the HX711 to pin15, when pressing down really hard on the Load cell(s) is the magnitue of 1,000,000. To make thing easier and arbitrary scalar of 1100 was used.



Use any of the 5x PWM pins to output the PWM "voltage" out to the low pass filter (any of the 5 "red tick" pins on the link below). This project used pin10.

It was determined that PWM(255) wasn't 5v,it was 4.7v (Pro Micro Voltage drop due to onboard circuitry). This resulted in calculations being base as 

3.0v - PWM(163) - 0% Brake Pedal
1.9v - PWM(103) - 100% Brake Pedal

https://learn.sparkfun.com/tutorials/pro-micro--fio-v3-hookup-guide/hardware-overview-pro-micro



Note: be carefull of the USB connector, Unfortunately itdoesn't take much to knock it off





