#include <Wire.h>

int z=0;

void(* resetFunc) (void) = 0;
void setup()
{
  Wire.begin();

  Serial.begin(9600);
  while (!Serial);
 
 
}
 
 
void loop()
{
  byte error, address;
  int nDevices;

  Serial.print("Starting....");
  nDevices = 0;
  for(address = 10; address < 127; address++ )
  {

    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
 
    if (error == 0)
    {
   
      Serial.print(0,z++);
      Serial.print("Found I2C at 0x");
      if (address<16)
       
        Serial.print("0");
        Serial.print(address,HEX);
       
 
      nDevices++;
    }
    else if (error==4)
    {
      Serial.print("Unknown error at address 0x");
      if (address<16)
        Serial.print("0");
      Serial.println(address,HEX);
    }   
  }
  if (nDevices == 0)
    Serial.println("No I2C devices found\n");
  else
    delay(10000);

    Serial.print("Resetting....");
    delay(5000);           // wait 5 seconds for next scan
    resetFunc();
}
