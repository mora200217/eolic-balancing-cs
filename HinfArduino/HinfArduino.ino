//Definición salidas para el motor
#include"./AS5600.h"
#include<Wire.h>

const byte numChars = 32;
char receivedChars[numChars];
boolean newData = false;

int IN3 = 9;    // In3 (L298N) conectada al pin 5
int IN4 = 8;    // In4 (L298N) conectada al pin 4
int ENB1 = 11;    // ENB conectada al pin 3 de Arduino
int ENB2 = 12;    // ENB conectada al pin 3 de Arduino
int sensorPin = A0;    // select the input pin for the potentiometer
int sensorValue = 0;  // variable to store the value coming from the sensor
double ang = 0;
double Ref = 90;

long previousMillis2 = 0; // For auxiliary functions (squarewaves)
long StartTime=0;
bool up = true;
int i = 0;
AMS_5600 sensor;

long previousMillis = 0;
double directCmd=40;
int Uunits=4;
int pwmMax=255;
unsigned int pwmDuty = 0;
double Ts = 2.5; //ms

//Hinf con Ts = 2.5e-3
double f1 = 12.04;
double f2 = -32.62;
double f3 = 17.32;
double f4 = 23.84;
double f5 = -29.36;
double f6 = 8.782;
double f7 = 1;
double f8 = -4.2;
double f9 = 6.966;
double f10 = -5.685;
double f11 = 2.273;
double f12 = -0.3543;

double un5,un4,un3,un2,un1,CmdHinf,CmdLim = 0.0;
double en5,en4,en3,en2,en1,error = 0.0;

//Pruebas enviando el PWM
double cmd = 0.0;

bool inicio=0;


void setup()
{
  //Configuración de los pines como salidas
  pinMode (ENB1, OUTPUT);
  pinMode (ENB2, OUTPUT);
  digitalWrite(ENB1, HIGH);
  digitalWrite(ENB2, HIGH);
  //pinMode (IN3, OUTPUT);
  pinMode (IN4, OUTPUT);
  Wire.begin();
  Serial.begin(115200);

}
void loop()
{
  //Salida para que el motor gire en un sentido
  //digitalWrite (IN3, HIGH);
  
  digitalWrite (IN4, LOW);

  if(!inicio){
    while(!newData){recvWithStartEndMarkers();}
    if (newData == true) {
      inicio=1;
      StartTime=millis();
      
      parseData();
      newData = false;

    }
  }
  
  controlHinf();
}

void caracterizacion() {
  unsigned long currentMillis = millis() - StartTime;
  ang = sensor.getAngleProcessed();
  pwmDuty = int((cmd) * pwmMax);
  analogWrite(IN3, pwmDuty);

  
    Serial.print(currentMillis / 1000.0,4);
    Serial.print("\t");
    Serial.print(cmd,3);
    Serial.print("\t");
    Serial.println(ang,3);


    recvWithStartEndMarkers();
      if (newData == true) {
        parseData();
        newData = false;
      }
    
}

void controlHinf() {

  unsigned long currentMillis = millis()-StartTime;
  if (currentMillis - previousMillis >= Ts ) {

    previousMillis = currentMillis;
    ang = sensor.getAngleProcessed();
    double Cmd = 255.0*(directCmd+CmdHinf)/100.0;  
    CmdLim = min(max(Cmd, 0), 1); // Saturated Control Output
    pwmDuty = int((CmdLim/1) * pwmMax);
    analogWrite(IN3, pwmDuty);

    if (currentMillis >= 5000) {

      error = (Ref - ang)*PI/180;
      double E2= (Ref - ang);
      //U Hinf
      CmdHinf=(f1*error+f2*en1+f3*en2+f4*en3+f5*en4+f6*en5-f8*un1-f9*un2-f10*un3-f11*un4-f12*un5)/f7;
      //Actualizamos los errores
      en5 = en4;
      en4 = en3;
      en3 = en2;
      en2 = en1;
      en1 = error;
      //Actualizamos las señales de control
      un5 = un4;
      un4 = un3;
      un3 = un2;
      un2 = un1;
      un1 = CmdHinf;

    }

    Serial.print(currentMillis / 1000.0,4);
    Serial.print("\t");
    Serial.print(CmdLim*100,3);
    Serial.print("\t");
    Serial.print(ang,3);
    Serial.print("\t");
    Serial.println(Ref);
  }

  recvWithStartEndMarkers();
  if (newData == true) {
    parseData();
    newData = false;
  }
}

void RefChange() {
  unsigned long currentMillis = millis();
  if (currentMillis >= 5000 && currentMillis - previousMillis2 >= 20000) {
    previousMillis2 = currentMillis; // refresh the last time you RUN
    if (up) {
      Ref = 90;
      up = false;
    } else {
      Ref = 120;
      up = true;
    }
  }
}


void recvWithStartEndMarkers() {
  static boolean recvInProgress = false;
  static byte ndx = 0;
  char startMarker = '<'; // Serial input must start with this character
  char endMarker = '>'; // Serial input must end with this character
  char rc;

  while (Serial.available() > 0 && newData == false) {
    rc = Serial.read();

    if (recvInProgress == true) {
      if (rc != endMarker) {
        receivedChars[ndx] = rc;
        ndx++;
        if (ndx >= numChars) {
          ndx = numChars - 1;
        }
      }
      else {
        receivedChars[ndx] = '\0'; // terminate the string
        recvInProgress = false;
        ndx = 0;
        newData = true;
      }
    }

    else if (rc == startMarker) {
      recvInProgress = true;
    }
  }
}

void parseData() {      // split the data into its parts
  Ref = atof(receivedChars);     // convert serial input to a float and update System Reference value with that value
  //cmd = atof(receivedChars);     // convert serial input to a float and update System Reference value with that value
}
