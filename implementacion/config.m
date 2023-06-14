clear; clc; close all; 
%% Configuracion
addpath("controllers"); 
addpath("data"); 

%% Cargar los datos 
load("data/TF.mat"); 
load("data/observer.mat"); 
load("data/getTF.mat"); 

%% Diseñadores 
addpath("controllers/SMC"); addpath("controllers/SMC/design"); 
addpath("controllers/Hinf"); addpath("controllers/Hinf/design"); 
%% Controladores 
load("controllers/SMC/SMC.mat")
load("controllers/Hinf/Hinf.mat")

%% Display
clc;
disp("Se han cargado correctamente los valores para la simulacion ert!"); 
pause(0.1)
disp("Abriendo mind.slx"); 
open("mind.slx")


%% Señales de prueba 
u = @(t) t>= 0;  % Funcion paso
Ts = 2.5e-3;     % Tiempo de muestreo  
T = 35;          % Tiempo final de sim

tsim = 0:Ts:T;    
x_cte = 60;
tbt = 7;         % Separacion entre señales
refsim =  30 * u(t - tbt)  +  10 * u(t - 2 * tbt) - 30 * u(t - 3 * tbt) + x_cte - 70 * u(t - 4 * tbt); % Señal de referencia 
plot(t, x); 




