clear; clc; close all; 
%% Configuracion
addpath("controllers"); 
addpath("data"); 

%% Cargar los datos 
load("data/TF.mat"); 
load("data/observer.mat"); 

%% Diseñadores 
addpath("controllers/SMC"); addpath("controllers/SMC/design"); 
addpath("controllers/Hinf"); addpath("controllers/Hinf/design"); 
%% Controladores 
load("controllers/SMC/SMC.mat")
load("controllers/Hinf/Hinf.mat")

%% Display
clc;
disp("Se han cargado correctamente los valores para la simulacion ert!"); 

