#include "Controller.h"
#include "arduino.h"


void foundTF(double angleLin) {
  //Pesos [N]
  double wb = 0.105948;
  double wcp = 0.014145;
  double wm = 0.030411;

  //Distancias [m]
  double d1 = 130.75e-3;
  double d2 = 50.75e-3;
  double e = 41.91e-3;

  //Momento de inercia [kg-m^2]
  double I = 15e-5;

  //Coeficiente término \theta
  double Coeficiente = wcp * d2 - wm * d1 - wb * e;

  //Constante del motor
  double km = 0.001118;

  //Constante de Fricción
  double beta = 1.667;
  double miu = beta * 2 * I;

  //Variables de estado
  double ass = (km * d1) / I;
  double bss = (wcp * d2 - wm * d1 - wb * e) / I;
  double css = -miu / I;

  /*float A [2] [2]= {{0, 1}, {bss*cos(angleLin*PI/180), css}};
float B [2] [1]= {{0}, {ass}};
float C [1] [2]= {1, 0};
float D = 0;*/

  // Se llena la matriz A
  A[0][0] = 0.0;
  A[0][1] = 1.0;
  A[1][0] = bss * cos(angleLin * PI / 180.0);
  A[1][1] = css;

  // Se llena la matriz B
  B[0][0] = 0.0;
  B[1][0] = ass;

  // Se llena la matriz C
  C[0][0] = 1.0;
  C[0][1] = 0.0;

  // Se llena la matriz D
  D = 0.0;
}


void observer(){
  thetaHat = A[0][0]*thetaP+A[0][1]*omegaP+B[0][0]*CmdLim+L[0][0]*angleP-L[0][0]*thetaP;
  omegaHat = A[1][0]*thetaP+A[1][1]*omegaP+B[1][0]*CmdLim+L[1][0]*angleP-L[1][0]*thetaP;
  thetaP = thetaHat;
  omegaP = omegaHat;
  angleP = ang*PI/180.0;
}