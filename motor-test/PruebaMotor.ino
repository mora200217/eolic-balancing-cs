//#inlude "BTS7960"



#include <Wire.h>
#include "./AS5600.h"

const uint8_t EN1 = 12;  //L_EN = R_EN -> Mismo pin
const uint8_t EN2 = 13;  //L_EN = R_EN -> Mismo pin
const uint8_t L_PWM = 9;
const uint8_t R_PWM = 8;

#define pwmRes 12   
#define pwmMax 4095 

//BTS7960 motorController(EN1, L_PWM, R_PWM);

AMS_5600 sensor; 


int pwmReal = 0;

void setup() {
  // put your setup code here, to run once:

  Wire.begin(); 

  Serial.begin(115200);
  // Enables from the H bridge
  digitalWrite(EN1,HIGH);
  digitalWrite(EN2,HIGH);

  // 
  setupPWMadj();
  analogWriteADJ(L_PWM, 0);
}

void loop() {
  // put your main code here, to run repeatedly:
  if(millis()<5000){
    pwmReal = 5;
  }
  else if (millis()<7000){
    pwmReal = 30;
  }

  else if(millis()<10000){
    pwmReal = 5;
  }

  else{
    pwmReal = 0;
  }

  analogWriteADJ(L_PWM, (int)((pwmReal/100.0)*pwmMax));
  //analogWrite(L_PWM,pwmReal);

  // Serial.println(pwmReal);
  
  // Serial.print("PWM: "); 
  Serial.print(pwmReal); 
  Serial.print(","); 
  Serial.print(-180); 
  Serial.print(",");
  Serial.print(180); 
  Serial.print(","); 
  // Serial.print("    Angle: ");
  Serial.println(String(sensor.getAngleProcessed(), DEC)); 





}


/* Configure digital pins 9 and 10 as 12-bit PWM outputs (3905 Hz). */
void setupPWMadj() {
  DDRB |= _BV(PB1) | _BV(PB2);        /* set pins as outputs */
  TCCR1A = _BV(COM1A1) | _BV(COM1B1)  /* non-inverting PWM */
      | _BV(WGM11);                   /* mode 14: fast PWM, TOP=ICR1 */
  TCCR1B = _BV(WGM13) | _BV(WGM12)
      | _BV(CS10);                    /* no prescaling */
  ICR1 = 0x0fff;                      /* TOP counter value - SETS RESOLUTION/FREQUENCY */
}

/* 12-bit version of analogWrite(). Works only on pins 9 and 10. (MAX VAL=4095) */
void analogWriteADJ(uint8_t pin, uint16_t val){
  switch (pin) {
    case  9: OCR1A = val; break;
    case 10: OCR1B = val; break;
    }
}
