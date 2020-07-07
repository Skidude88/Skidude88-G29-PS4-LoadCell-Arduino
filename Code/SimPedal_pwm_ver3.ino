#include <HX711.h>    // HX711-0.7.3.zip

// How many samples to take at initialization of the library to zero
// out the offset of the load cell.
const int BRAKE_PEDAL_LOAD_CELL_TARE_REPS =  10;


// Arbitary scaling. Single Load Cell.
// Testing with set at 1100, pressing hard -  max brake_value is a value around 1200 to 1500 
const int BRAKE_PEDAL_LOAD_CELL_SCALING =  1100;


// LOCAL VARIABLES

// HX711 circuit wiring
const int LOADCELL_DOUT_PIN = 1;
const int LOADCELL_SCK_PIN = 0;

const int db93 = 9; // Output PWM micro pro pin9 to G29 Brake DB9 pin 3

const float brakemax = -1200; //comfortably pressing hard on load cell, with single cell, inverted is -1200. To Stiffen brake go down to -1500

const float arduino_vcc = 4.7; //5v Arduino: 255 pwm = 4.7v output

const float G29BrakeFull_v = 1.9;
const float G29BrakeOff_v = 3.1;

const float pwm_BrakeFullPerc = G29BrakeFull_v / arduino_vcc;
const float pwm_BrakeOffPerc = G29BrakeOff_v / arduino_vcc;

const int pwm_BrakeFull = 255 * pwm_BrakeFullPerc;  // Lower Voltage/pwm value
const int pwm_BrakeOff = 255 * pwm_BrakeOffPerc;    // Higher voltage/pwm value
const int pwm_Range = pwm_BrakeOff - pwm_BrakeFull;

const float pwm_step = arduino_vcc / 255;

HX711 brake_pedal;
int analogWriteValue; 


void setup()

{
  // Zero the pedal offset. This is is to compensate for the own weight
  // of the pedal. It is important not to apply any force to the pedal
  // While this is happening
  brake_pedal.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
  brake_pedal.tare(BRAKE_PEDAL_LOAD_CELL_TARE_REPS);
  
  pinMode(db93,OUTPUT); // set the pin IO mode
  TXLED0;
  RXLED0;
} 


void loop()

{
  int32_t brake_value ;
  float brakePercOn;
  
  brake_value = brake_pedal.get_value(1);
   
  brake_value /= BRAKE_PEDAL_LOAD_CELL_SCALING;
  brake_value *= -1; // invert so off pedal is a higher value (0) than full pedal 

  if (brake_value > 0){
    brake_value = 0;  
  }
  //Serial.println (brake_value);
  
  brakePercOn = brake_value/brakemax; // determine applied brake pedal percentage
  //Serial.println (brakePercOn);
  

  analogWriteValue = pwm_BrakeOff - (pwm_Range *  brakePercOn);

// Check not supplying over voltage
  if (analogWriteValue * pwm_step > G29BrakeOff_v) {
    analogWriteValue = G29BrakeOff_v;
  }


  analogWrite(db93, analogWriteValue);
  //Serial.println (analogWriteValue);

  
} 
