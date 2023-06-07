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

int ang, lang = 0;

float thetaP = 0;
float omegaP = 0;
float angleP = 0;
float uP = 0;

float thetaHat;
float omegaHat;
float angle; //VERIFICAR NOMBRE

//SMC
int a = 1;
int beta = 40;
int directBeta = 40;
float Ref = 90;//VERIFICAR NOMBRE
float delta;

//Hinf con Ts = 2.5e-3
float f1 = 12.04;
float f2 = -32.62;
float f3 = 17.32;
float f4 = 23.84;
float f5 = -29.36;
float f6 = 8.782;
float f7 = 1;
float f8 = -4.2;
float f9 = 6.966;
float f10 = -5.685;
float f11 = 2.273;
float f12 = -0.3543;

float un5,un4,un3,un2,un1,CmdHinf = 0.0;
float en5,en4,en3,en2,en1,error = 0.0;

long previousMillis = 0;
double directCmd=40;
int Uunits=4;
int pwmMax=255;
unsigned int pwmDuty = 0;
float Ts = 2.5; //ms

void observer(){
  angle = ams5600.getAngleProcessed();
  thetaHat = A[1][1]*thetaP+A[1][2]*omegaP+B[1][1]*uP+L[1][1]*angleP-L[1][1]*thetaP;
  omegaHat = A[2][1]*thetaP+A[2][2]*omegaP+B[2][1]*uP+L[2][1]*angleP-L[2][1]*thetaP;
  thetaP = thetaHat;
  omegaP = omegaHat;
  angleP = angle;
}

int sign(float num){
  if (num = 0){
    return 0;
  } else if (num>0){
    return 1;
  } else if (num<0){
    return -1;
  }
}

void SMCController(){
  observer();
  delta = Ref; //En grados
  float inSign = a*(PI/180)*(angle-delta)+omegaHat;
  int signo = sign(inSign);
  int uSMC = -beta*signo;
  int u = uSMC + directBeta;
  //Valor entre 0 y 80 falta pasarlo al motor en forma de PWM
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

void controlHinf(){
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= Ts ) {
    previousMillis = currentMillis;
    //sensorValue = analogRead(sensorPin);
    ang=ams5600.getAngleProcessed();//(202.3460388183-(float(sensorValue)*360/1023));
    double Cmd = 255*(directCmd+CmdHinf)/100;  
    double CmdLim = min(max(Cmd, 0), 1); // Saturated Control Output
    pwmDuty = int((CmdLim/1)*pwmMax);
    //analogWrite(IN3,pwmDuty);

    if (currentMillis >= 5000) {
     
      error = (Ref - ang)*PI/180;
      float E2= (Ref - ang);
      //Red de adelanto
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
    Serial.print(currentMillis/1000.0,DEC);
    Serial.print(",");
    Serial.print(Ref,DEC);
    Serial.print(",");
    Serial.println(ang,DEC);  
   
   }
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






