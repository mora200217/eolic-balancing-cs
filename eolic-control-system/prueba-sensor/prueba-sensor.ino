#include "Wire.h"
#include "./AS5600.h"


AMS_5600 ams5600;


int ang, lang = 0;

void setup()
{
  Serial.begin(115200);
  
  Wire.setWireTimeout(3000); 
  
  Serial.println(">>>>>>>>>>>>>>>>>>>>>>>>>>> ");
  Serial.println("Presearching ... "); 
  Serial.print("Magnet: "); 
  Serial.println(ams5600.getMagnetStrength());  
  Serial.println("founded!"); 

}
/*******************************************************
/* Function: convertRawAngleToDegrees
/* In: angle data from AMS_5600::getRawAngle
/* Out: human readable degrees as float
/* Description: takes the raw angle and calculates
/* float value in degrees.
/*******************************************************/
float convertRawAngleToDegrees(word newAngle)
{
  /* Raw data reports 0 - 4095 segments, which is 0.087890625 of a degree */
  float retVal = newAngle * 0.087890625;
  return retVal;
}
void loop()
{
    
}
