#ifndef Controller
    #include "arduino.h"
    #define Controller
    
    // Matrices de observador 
    double A[2][2];
    double B[2][1];
    double C[1][2];
    double D;
    
    double L[2][1] = {{42.6055},{857.6136}};

    // Funciones de observador 
    void foundTF(double angleIn); 
    void observer(); 
#endif


