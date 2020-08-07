#include <HX711.h>    // HX711-0.7.3.zip
#include <Wire.h>     //Include the Wire library to talk I2C

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

const float brakemax = -500; //comfortably pressing hard on load cell, with single cell, inverted is -around 400. To Stiffen brake go down to -800
const float arduino_vcc = 4.7; //USB 5v Arduino
const float G29BrakeFull_v = 1.9;
const float G29BrakeOff_v = 3.1;
const float dac_BrakeFullPerc = G29BrakeFull_v / arduino_vcc;
const float dac_BrakeOffPerc = G29BrakeOff_v / arduino_vcc;

const int dac_BrakeFull = 4096 * dac_BrakeFullPerc;  // Lower Voltage
const int dac_BrakeOff = 4096 * dac_BrakeOffPerc;    // Higher voltage
const int dac_Range = dac_BrakeOff - dac_BrakeFull;

const float dac_step = arduino_vcc / 4096;

HX711 brake_pedal;

unsigned int dacValue; 
byte buffer[3];  

void setup()

{
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
  int32_t brake_value ;
  float brakePercOn;

  buffer[0] = 0b01000000;            //Sets the buffer0 with control byte (010-Sets in Write mode)

  // Turn off Tx & RX LEDS
  TXLED0;
  RXLED0;
  
  brake_value = brake_pedal.get_value(1);

  brake_value /= BRAKE_PEDAL_LOAD_CELL_SCALING;
  brake_value *= -1; // invert so off pedal is a higher value (0) than full pedal 

  if (brake_value > 0){
    brake_value = 0;  
  }
  
  //Serial.println (brake_value);
  
  brakePercOn = brake_value/brakemax; // determine applied brake pedal percentage
  //Serial.println (brakePercOn);
  
  dacValue = dac_BrakeOff - (dac_Range *  brakePercOn);

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

  //Serial.println(dacValue);
  
} 
