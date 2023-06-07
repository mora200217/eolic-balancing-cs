#include <Wire.h>
#include "./AS5600.h"
#ifdef ARDUINO_SAMD_VARIANT_COMPLIANCE
#define SERIAL SerialUSB
#define SYS_VOL 3.3
#else
#define SERIAL Serial
#define SYS_VOL 5
#endif

AMS_5600 ams5600;

double A[2][2];
double B[2][1];
double C[1][2];
double D;

double L[2][1] = {{42.6055},{857.6136}};
double Ts = 2.5e-3;

int ang, lang = 0;

float thetaP = 0;
float omegaP = 0;
float angleP = 0;
float uP = 0;

float thetaHat;
float omegaHat;
float angle; //VERIFICAR NOMBRE

void observer(){
  thetaHat = A[1][1]*thetaP+A[1][2]*omegaP+B[1][1]*uP+L[1][1]*angleP-L[1][1]*thetaP;
  omegaHat = A[2][1]*thetaP+A[2][2]*omegaP+B[2][1]*uP+L[2][1]*angleP-L[2][1]*thetaP;
  thetaP = thetaHat;
  omegaP = omegaHat;
  angleP = angle;
}



void foundTF(int angleLin) {
  //Pesos [N]
  float wb = 0.105948;
  float wcp = 0.014145;
  float wm = 0.030411;

  //Distancias [m]
  float d1 = 130.75e-3;
  float d2 = 50.75e-3;
  float e = 41.91e-3;

  //Momento de inercia [kg-m^2]
  float I = 15e-5;

  //Coeficiente término \theta
  float Coeficiente = wcp * d2 - wm * d1 - wb * e;

  //Constante del motor
  float km = 0.001118;

  //Constante de Fricción
  float beta = 1.667;
  float miu = beta * 2 * I;

  //Variables de estado
  float ass = (km * d1) / I;
  float bss = (wcp * d2 - wm * d1 - wb * e) / I;
  float css = -miu / I;

  /*float A [2] [2]= {{0, 1}, {bss*cos(angleLin*PI/180), css}};
float B [2] [1]= {{0}, {ass}};
float C [1] [2]= {1, 0};
float D = 0;*/

  // Se llena la matriz A
  A[0][0] = 0;
  A[0][1] = 1;
  A[1][0] = bss * cos(angleLin * PI / 180);
  A[1][1] = css;

  // Se llena la matriz B
  B[0][0] = 0;
  B[1][0] = ass;

  // Se llena la matriz C
  C[0][0] = 1;
  C[0][1] = 0;

  // Se llena la matriz D
  D = 0;
}


void setup() {

  SERIAL.begin(115200);
  foundTF(89);

  Serial.println("A: ");
  for (int i = 0; i <= 1; i++) {
    Serial.print(A[i][0]);
    Serial.print(" ");
    Serial.println(A[i][1]);
  }
  Serial.println("B: ");
  Serial.println(B[0][0]);
  Serial.println(B[1][0]);
  
  Serial.println("c: ");
  Serial.print(C[0][0]);
  Serial.println(C[0][1]);

  Serial.println(Ts);
  


/*
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
  }*/
}

void loop() {
  /*
    SERIAL.print(180); 
    SERIAL.print("; "); 
    SERIAL.print(-180); 
   // SERIAL.print(millis() / 1000.0); 
   SERIAL.print("; "); 
//     SERIAL.print(";angle: ");
    SERIAL.println(String(ams5600.getAngleProcessed(),DEC));
    */
}






