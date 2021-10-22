#include <HX711.h>    // https://github.com/aguegu/ardulibs/tree/master/hx711

// How many samples to take at initialization of the library to zero
// out the offset of the load cell.
const int BRAKE_PEDAL_LOAD_CELL_TARE_REPS =  10;

// Sanity check for the load cell readings. This will typically be to
// prevent from loading negative values if/when the load cell is pulled
// in the wrong direction
const int32_t BRAKE_PEDAL_LOAD_CELL_MAX_VAL = 1100000;


// The hardware is 3D printed and I do not recommend this value
// Be set any lower than 1100. Any lower value will require more force to
// be applied to the pedal, and may damage the 3D printed parts.
const int BRAKE_PEDAL_LOAD_CELL_SCALING =  1100;
// Testing with set a 1100 pressing hard -  max brake value is a value around 900



// LOCAL VARIABLES

// HX711 circuit wiring
const int LOADCELL_DOUT_PIN = 14;
const int LOADCELL_SCK_PIN = 15;

const int db93 = 10; // Output PWM micro pro pin10 to G29 Brake DB9 pin 3

const float brakemax = -900.0; //pressing hard on load cell, with inverted scaling gives -900

const float arduino_vcc = 5;

const float G29BrakeFull_v = 1.9;
const float G29BrakeOff_v = 3.0;

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
} 

void loop()

{
  int32_t brake_value ;
  float brakePercOn;
  
  brake_value = brake_pedal.get_value(1);

  if(brake_value > BRAKE_PEDAL_LOAD_CELL_MAX_VAL) 
  {
    brake_value = BRAKE_PEDAL_LOAD_CELL_MAX_VAL;
  }

  if(brake_value < 0) 
  {
    brake_value = 0;
  }
  
  
  brake_value /= BRAKE_PEDAL_LOAD_CELL_SCALING;
  brake_value *= -1; // invert so off pedal is a higher value (0) than full pedal 
  //Serial.println (brake_value);
  
  brakePercOn = brake_value/brakemax; // determine applied brake pedal percentage
  //Serial.println (brakePercOn);
  

  analogWriteValue = pwm_BrakeOff - (pwm_Range *  brakePercOn);

// Check not supplying over voltage
  if (analogWriteValue * pwm_step > 3.3) {
    analogWriteValue = 0;
  }


  //analogWrite(db93, analogWriteValue);
  Serial.println (analogWriteValue);
  
  
} 
