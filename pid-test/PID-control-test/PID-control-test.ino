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

long previousMillis2 = 0; // For auxiliary functions (squarewaves)
bool up = true;
int i = 0;
AMS_5600 sensor;

long previousMillis = 0;
double directCmd = 0.3;
int Uunits = 4;
int pwmMax = 255;
unsigned int pwmDuty = 0;

//float Kp=0.015;
//float Ki=0.012;
//float Kd=0.002;

//Las epicas
/*float Kp = 0.0009;
  float Ki = 0.3;
  float Kd = 0.00323;*/

float Kp = 0.4;
float Ki = 0.65;
float Kd = 0.018;

//Red de adelanto
double a = 2.377;
double b = -2.326;
double c = 1;
double d = -0.9485;
//Red de adelanto-atraso
double a1 = 2.13;
double b1 = -4.209;
double c1 = 2.08;
double d1 = 1;
double e1 = -1.947;
double f1 = 0.9471;
//Red de atraso
double a2 = 0.2908;
double b2 = -0.2901;
double c2 = 1;
double d2 = -0.9993;

//double c1=1.00;
//double d1=-0.972268441486412;
double Ref = 90;
double Uin = 0;
double Uinp = 0;
double CmdPI = 0.0;
double Cmd = 0.0;
int N = 100;
long Ts = 2.5;
double E2 = 0;
double E = 0;
double Ep = 0;
double Ep2 = 0;
double Up = 0;
double Upp = 0;
double Up2 = 0;
double CmdIp = 0;
double CmdDp = 0;
double CmdC = 0;  //señal Controlador con red incluida
double CmdCp = 0; //señal Controlador con red incluida
double CmdPIp = 0;
double CmdPIpp = 0;

float cmd = 0;

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
  controlPI();
  
  //RefChange();
  //caracterizacion();
}

void caracterizacion() {
  unsigned long currentMillis = millis()-1000;
  if (currentMillis <= 1000) {
    cmd = 0.2;
  } else if (currentMillis <= 6000) {
    cmd = 0.2;
  } else if (currentMillis <= 11000) {
    cmd = 0.3;
  } else if (currentMillis <= 17000) {
    cmd = 0.1;
  } else {
    cmd = 0.2;
  }
  //ang = sensor.getAngleProcessed();
  pwmDuty = int((cmd) * pwmMax);
  analogWrite(IN3, pwmDuty);
  //Serial.print("time: ");
  Serial.print(currentMillis / 1000.0 + 1, DEC);
  Serial.print(",");
  //Serial.print(" pwm: ");
  Serial.println(cmd, DEC);
  //Serial.print(",");
  //Serial.print(" angle:");
  //Serial.println(ang, DEC);
}

void controlPI() {
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= Ts ) {

    previousMillis = currentMillis;
    //sensorValue = analogRead(sensorPin);
    //ang=(202.3460388183-(float(sensorValue)*360/1023));
    ang = sensor.getAngleProcessed();
    double Cmd = directCmd + CmdC
                 ;
    double CmdLim = min(max(Cmd, 0), 1); // Saturated Control Output
    pwmDuty = int((CmdLim / 1) * pwmMax);
    analogWrite(IN3, pwmDuty);

    if (currentMillis >= 5000) {

      E = (Ref - ang) * PI / 180;
      E2 = (Ref - ang);
      //CmdPI = Kp*(E - Ep) + (Ki*Ts/2)*(E + Ep) + Up;
      //Uin=(a*E+b*Ep)*0.0001+Uinp;
      //CmdPI=(a*E+b*Ep+c*Ep2)*0.00001+a1*Up+a2*Up2;
      //CmdPI=(c*Uin+d*Uinp-d1*Up)/c1;
      double CmdP = Kp * E;
      double CmdI = (Ki * (Ts * 1e-3) * Ep) + CmdIp;
      double CmdD = Kd * N * (E - Ep) + CmdDp * (1 - (N * Ts * 1e-3));

      CmdPI = CmdP + CmdI + CmdD;
      //Red de adelanto
      CmdC = (a * CmdPI + b * CmdPIp - d * Up) / c;
      //Red de adelanto atraso
      //CmdC=(a1*CmdPI+b1*CmdPIp+c1*CmdPIpp -e1*Up-f1*Upp)/d1;
      //Red de atraso
      //CmdC=(a2*CmdPI+b2*CmdPIp-d2*Up)/c2;
      //Uinp=Uin;
      //Ep2=Ep;
      //Up2=Up;
      CmdIp = CmdI;
      CmdDp = CmdD;
      CmdPIpp = CmdPIp;
      CmdPIp = CmdPI;
      Ep = E;
      Upp = Up;
      Up = CmdC;

    }
    //Serial.print("Time:");
    //Serial.print(currentMillis);
    //Serial.print(",");
    //Serial.print("Error:");
    //Serial.print(E2);
    //Serial.print(",");
    //Serial.print("U:");
    //Serial.print(CmdPI);
    //Serial.print(",");
    //Serial.print("Ref:");
    Serial.print(currentMillis / 1000.0, DEC);
    Serial.print(",");
    Serial.print(Ref, DEC);
    Serial.print(",");
    //Serial.print("angle:");
    Serial.println(ang, DEC);

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
}
