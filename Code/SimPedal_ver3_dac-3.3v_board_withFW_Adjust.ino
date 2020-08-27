// Pedal Gradient Maths
// Back to school....
// Using DiView 2 linear gradients were determine as being used. Gradient2 being the informous brake grab\rubber bung fudge
// Sample data :
// Gradient1 (noted from 0% - around 70%) 
// x1 (G29% output) = 0.0706, y1 (LC %) =  0.15
// x2 = 0.6843, y2 = 0.8
// x2-x1 = 0.6137, y2-y1 = 0.65
// gradient1 (m1) = 0.65/0.6137 = 1.0591
// y_intersect (b1) = y1-(m1*x1)
// b1 = 0.0752
//
// Gradient2 (noted from around 70% - 100% brake) 
// x1 (G29% output) = 0.7235, y1 (LC %) = 0.825
// x2 = 1, y2 = 1
// x2-x1 = 0.2765, y2-y1 = .175
// gradient1 (m2) = 0.175/0.2765 = 0.6329
// y_intersect (b2) = y1-(m1*x1)
// b2 = 0.3671
//
// Gradient1 & Gradient2 intersect - using y = mx+b
// y = (1.0591 * x) + 0.0752
// and
// y = (0.6329 * x) + 0.3671
// (1.0591 * x) + 0.0752 = (0.6329 * x) + 0.3671
// (1.0591 * x) - (0.6329 * x) = 0.3671 - 0.0752
// (1.0591 * x) - (0.6329 * x) = 0.2919
//  0.4261 * x  = 0.2919
// Intersect x = 0.6848 (68.48%)
// y = (mx)+b  - using either gradient
// LC%_y = (m1  * .6848) +b1
// LC%_y = (1.0591 * 0.6848) + 0.0752 = 0.8004 (80%)

// Code for 3.3v Micro Pro Board with 1x load cell, 1x HX711 module and 1x MCP4725 DAC

#include <HX711.h>    // HX711-0.7.3.zip
#include <Wire.h>     //Include the Wire library to talk I2C
//#include <avr/power.h> // Was needed for the 5v board and div2, now using 3.3v board

//This is the I2C Address of the MCP4725, by default (A0 pulled to GND).
//Please note that this breakout is for the MCP4725A0. 
#define MCP4725_ADDR 0x60   
//For devices with A0 pulled HIGH, use 0x61
// MCP4725 is 12bit inout and output at default 100Kbps (also available 400Kbps mode)

// How many samples to take at initialization of the library to zero
// out the offset of the load cell.
const int BRAKE_PEDAL_LOAD_CELL_TARE_REPS =  10;

// Arbitary scaling 1100. Single Load Cell.
// Testing with set at 1100, pressing hard -  max brake_value is a value around 400 to 800 
const int BRAKE_PEDAL_LOAD_CELL_SCALING =  1100;

// LOCAL VARIABLES

// HX711 circuit wiring
const int LOADCELL_DOUT_PIN = 1;
const int LOADCELL_SCK_PIN = 0;

const float brakemax = -600; //comfortably pressing hard on load cell, with single cell, inverted is -400. To Stiffen brake go down to -800
const float arduino_vcc = 3.3; //G29 3.3v
const float G29BrakeFull_v = 1.48;
// GTS SPORT 2.86v to 1.93v
// ACC 2.88v to 1.49v - 18.08.2020 2.76v
// ACC 21.08.2020 2.89v off was too sensative, resting foot on pedal registered as brake slightly on. 
const float G29BrakeOff_v = 2.93;
const float dac_BrakeFullPerc = G29BrakeFull_v / arduino_vcc;
const float dac_BrakeOffPerc = G29BrakeOff_v / arduino_vcc;

const float dac_step = arduino_vcc / 4096;

const int dac_BrakeFull = G29BrakeFull_v / dac_step;  // Lower Voltage
const int dac_BrakeOff = G29BrakeOff_v / dac_step;    // Higher voltage
const int dac_Range = dac_BrakeOff - dac_BrakeFull;

// linear equation y = mx+b
const float base_gradient_m1 = 1.0591;
const float base_intersect_b1 = 0.0752;

const float adj_gradient_m2 = 0.6329;
const float adj_intersect_b2 = 0.3671;

HX711 brake_pedal;

unsigned int dacValue; 
byte buffer[3];  

void setup()

{
  // Div 2 was needed for 5v board , now using 3.3v board
  //Div_2 - 16MHz /2 = 8Mhz
  //clock_prescale_set(clock_div_2);
  
  // Zero the pedal offset. This is is to compensate for the own weight
  // of the pedal. It is important not to apply any force to the pedal
  // While this is happening
  brake_pedal.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
  brake_pedal.tare(BRAKE_PEDAL_LOAD_CELL_TARE_REPS);
  
  Wire.begin();                       // Begins the I2C communication
  //TWBR = 12;                        // Seems OK at 100kbps, otherwise MCP4725 can run at 400KHz using TWBR=12
} 

void loop()

{
  int32_t brake_value;
  float LC_brakePercOn;
  float brakePercOn;
  float G29fw_Perc;

  buffer[0] = 0b01000000;            //Sets the buffer0 with control byte (010-Sets in Write mode)

  // Turn off Tx & RX LEDS
  TXLED0;
  RXLED0;
  
  brake_value = brake_pedal.get_value(1);
  
  brake_value /= BRAKE_PEDAL_LOAD_CELL_SCALING;
  brake_value *= -1; // invert so off pedal is a higher value (0) than full pedal 

  //brake_value = -300;
  
  if(brake_value > 0){
    brake_value = 0;
  }

  //Serial.println (brake_value);
  
  LC_brakePercOn = brake_value/brakemax; // determine applied brake pedal percentage(base gradient1)
  
  if (LC_brakePercOn > .8){
    // from 80% Calculate the G29fw_% using gradient1 and recalculate LC% using gradient2
    // x = (y-b)/m with gradient1
    G29fw_Perc = (LC_brakePercOn - base_intersect_b1) / base_gradient_m1;
    // y = (mx)+b with gradient2
    LC_brakePercOn = (adj_gradient_m2 * G29fw_Perc )+ adj_intersect_b2 ; 
  }
  //Serial.println(LC_brakePercOn);

  //LC_brakePercOn = .5;
  
  dacValue = dac_BrakeOff - (dac_Range *  LC_brakePercOn);

//diVIew
  //  dacValue = 3661;
  
  //  dacValue = 3339;
  //  dacValue = 3091;
  //  dacValue = 2842;
  //  dacValue = 2594;
   
  //  dacValue = 2346;
  //  dacValue = 2098;
  //  dacValue = 1974;
  //  dacValue = 1849;
  //  dacValue = 1837;

 //dacValue = 1837; // 1.48v

//ACC
  //dacValue = 3426; //2.76v
  //dacValue = 1849; //1.49v

  //  dacValue = 3415; // flicker red
  //  dacValue = 3636; 
  //  dacValue = 3388;
  //  dacValue = 3140;
  //  dacValue = 2892;
  //  dacValue = 2643;
  //  dacValue = 2395;
  //  dacValue = 2147;
  //  dacValue = 1850;
  //  dacValue = 1849;

// GTS
  //  dacValue = 3550; // Not On 
  //  dacValue = 3537; //ABS LED FLicker
  //  dacValue = 3636; 
  //  dacValue = 3388;
  //  dacValue = 3140;
  //  dacValue = 2892;
  //  dacValue = 2643;
  //  dacValue = 2395;
  //  dacValue = 2147;
  //  dacValue = 2395;
  //  dacValue = 1837;

  // Check not supplying over voltage
  if (dacValue * dac_step > G29BrakeOff_v) {
    dacValue = G29BrakeOff_v;
  }

  buffer[1] = dacValue >> 4;              //Puts the most significant bit values
  buffer[2] = dacValue << 4;              //Puts the Least significant bit values

  Wire.beginTransmission(MCP4725_ADDR);
  Wire.write(buffer[0]);            //Sends the control byte to I2C 
  Wire.write(buffer[1]);            //Sends the MSB to I2C 
  Wire.write(buffer[2]);            //Sends the LSB to I2C  
  Wire.endTransmission();

//  Serial.println(dacValue);
  
} 
