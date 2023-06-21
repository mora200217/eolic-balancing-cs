#include <Wire.h>
#include "./AS5600.h"
#ifdef ARDUINO_SAMD_VARIANT_COMPLIANCE
  #define SERIAL SerialUSB
  #define SYS_VOL   3.3
#else
  #define SERIAL Serial
  #define SYS_VOL   5
#endif

AMS_5600 ams5600;

int ang, lang = 0;

void setup()
{
  SERIAL.begin(115200);
  Wire.begin();
  SERIAL.println(">>>>>>>>>>>>>>>>>>>>>>>>>>> ");
  if(ams5600.detectMagnet() == 0 ){
    while(1){
        if(ams5600.detectMagnet() == 1 ){
            SERIAL.print("Current Magnitude: ");
            SERIAL.println(ams5600.getMagnitude());
            break;
        }
        else{
            SERIAL.println("Can not detect magnet");
        }
        delay(1000);
    }
  }
}

void loop()
{ 
  
    SERIAL.print(180); 
    SERIAL.print("; "); 
    SERIAL.print(-180); 
   // SERIAL.print(millis() / 1000.0); 
   SERIAL.print("; "); 
//     SERIAL.print(";angle: ");
    SERIAL.println(String(ams5600.getAngleProcessed(),DEC));
}