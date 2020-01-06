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


# Software
Arduino IDE (Desktop) v1.8.10
https://www.arduino.cc/en/main/software

# Dual Load Cells

Initially 2x Load cells were used, each being half of a Wheatstone Bridge

Usefull Info:

https://learn.sparkfun.com/tutorials/load-cell-amplifier-hx711-breakout-hookup-guide?_ga=2.166487294.1982490767.1578266625-357928328.1568243893


https://www.instructables.com/id/How-to-Interface-HX711-Balance-Module-With-Load-Ce/


# Single Load Cell

Usefull Info:

https://www.instructables.com/id/Tutorial-to-Interface-HX711-With-Load-Cell-Straigh/



#G29 Pedal (more detail on GeekyDeaks project)

Pedal off = 3.0v
Pedal on  = 1.9v


Suppliying PWM from the Micro Pro didn't work, so a  basic passive low pass filer was added.
Consisting of :

1x 2.2uF Capacitor
1x 4.7k resistor

Usefull info:

https://provideyourown.com/2011/analogwrite-convert-pwm-to-voltage/


