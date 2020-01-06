# Skidude88-G29-PS4-LoadCell-Arduino
DIY load cell conversion for Logitech G29 on PS4 using Arduino Pro Micro

This Project was developed in conjunction with an analog version by GeekyDeaks

https://github.com/GeekyDeaks/g29-load-cell

# Initial Shopping List:

Arduino Pro Micro

'Bathroom Scales' style Load Cell

Load Cell Amplifier : HX711

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

The HX711 board can run at either 10hz (By default) or 80hz. Some boards have a select jumper to solder/break track. To acheive 80hz without this selector (on cheaper version boards) the Rate pin requires +v. Cut the track immediatly next to pin15 "Rate" (Some forum posts go to the trouble of lift pin15 pin off the board!) and then solder pin15 to pin16 (Vcc) directly above. 



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

Usefull Info:

https://learn.sparkfun.com/tutorials/load-cell-amplifier-hx711-breakout-hookup-guide?_ga=2.166487294.1982490767.1578266625-357928328.1568243893


https://www.instructables.com/id/How-to-Interface-HX711-Balance-Module-With-Load-Ce/


# Single Load Cell

Usefull Info:

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


