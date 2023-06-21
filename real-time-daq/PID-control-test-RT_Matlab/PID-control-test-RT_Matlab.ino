//Definición salidas para el motor
#include"./AS5600.h"
#include<Wire.h>
#include "Controller.h"


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
long StartTime=0;
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

bool inicio=0;

double CmdLim = 0;

//Observador

double A[2][2];
double B[2][1];
double C[1][2];
double D;

double L[2][1] = {{42.6055},{857.6136}};

//int ang, lang = 0;

double thetaP = 0;
double omegaP = 0;
double angleP = 0;
double uP = 0;

double thetaHat;
double omegaHat;



void setup()
{
  //Configuración TF
  foundTF(89.0);
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
  
  controlPI();
  
  //RefChange();
  //caracterizacion();
}


void caracterizacion() {
  unsigned long currentMillis = millis() - StartTime;
  /*if (currentMillis <= 1000) {
    cmd = 0;
  } else if (currentMillis <= 6000) {
    cmd = 0.2;
  } else if (currentMillis <= 11000) {
    cmd = 0.3;
  } else if (currentMillis <= 17000) {
    cmd = 0.1;
  } else {
    cmd = 0.2;
  }*/
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

void controlPI() {

  unsigned long currentMillis = millis()-StartTime;
  if (currentMillis - previousMillis >= Ts ) {

    previousMillis = currentMillis;
    ang = sensor.getAngleProcessed(); // Lectura de angulo
    // Observador 

    // Control 
    double Cmd = directCmd + CmdC;
    CmdLim = min(max(Cmd, 0), 1); // Saturated Control Output
    observer(); 
    pwmDuty = int((CmdLim / 1) * pwmMax);
    analogWrite(IN3, pwmDuty);

    
    if (currentMillis >= 5000) {

      E = (Ref - ang) * PI / 180;
      E2 = (Ref - ang);
      double CmdP = Kp * E;
      double CmdI = (Ki * (Ts * 1e-3) * Ep) + CmdIp;
      double CmdD = Kd * N * (E - Ep) + CmdDp * (1 - (N * Ts * 1e-3));

      CmdPI = CmdP + CmdI + CmdD;
      CmdC = (a * CmdPI + b * CmdPIp - d * Up) / c;
      CmdIp = CmdI;
      CmdDp = CmdD;
      CmdPIpp = CmdPIp;
      CmdPIp = CmdPI;
      Ep = E;
      Upp = Up;
      Up = CmdC;

    }

    Serial.print(currentMillis / 1000.0,4);
    Serial.print("\t");
    Serial.print(CmdLim*100,3);
    Serial.print("\t");
    Serial.print(ang,3);
    Serial.print("\t");
    Serial.print(Ref);
    Serial.print("\t");
    Serial.print(thetaHat);
    Serial.print("\t");
    Serial.println(omegaHat);
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


// void foundTF(double angleLin) {
//   //Pesos [N]
//   double wb = 0.105948;
//   double wcp = 0.014145;
//   double wm = 0.030411;

//   //Distancias [m]
//   double d1 = 130.75e-3;
//   double d2 = 50.75e-3;
//   double e = 41.91e-3;

//   //Momento de inercia [kg-m^2]
//   double I = 15e-5;

//   //Coeficiente término \theta
//   double Coeficiente = wcp * d2 - wm * d1 - wb * e;

//   //Constante del motor
//   double km = 0.001118;

//   //Constante de Fricción
//   double beta = 1.667;
//   double miu = beta * 2 * I;

//   //Variables de estado
//   double ass = (km * d1) / I;
//   double bss = (wcp * d2 - wm * d1 - wb * e) / I;
//   double css = -miu / I;

//   /*float A [2] [2]= {{0, 1}, {bss*cos(angleLin*PI/180), css}};
// float B [2] [1]= {{0}, {ass}};
// float C [1] [2]= {1, 0};
// float D = 0;*/

//   // Se llena la matriz A
//   A[0][0] = 0.0;
//   A[0][1] = 1.0;
//   A[1][0] = bss * cos(angleLin * PI / 180.0);
//   A[1][1] = css;

//   // Se llena la matriz B
//   B[0][0] = 0.0;
//   B[1][0] = ass;

//   // Se llena la matriz C
//   C[0][0] = 1.0;
//   C[0][1] = 0.0;

//   // Se llena la matriz D
//   D = 0.0;
// }








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
