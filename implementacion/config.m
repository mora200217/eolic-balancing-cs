
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
addpath("controllers/MRAC"); addpath("controllers/MRAC/design");
%% Controladores 
load("controllers/SMC/SMC.mat")
load("controllers/Hinf/design/Hinf.mat")
load("controllers/MRAC/design/MRAC.mat")
%% Display
clc;
disp("Se han cargado correctamente los valores para la simulacion ert!"); 
pause(0.1)
disp("Abriendo mind.slx"); 
open("mind.slx")





