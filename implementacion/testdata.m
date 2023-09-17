%% Import data 
addpath("resultados-finos/")
mat = readtable("resultados-finos/mrac.xlsx"); 

% Lectura de los datos 
%time = table2array(mat(:, 1));   % Tiempo
%ref = table2array(mat(:, 2));    % Referencia de control
%theta = table2array(mat(:, 3));  % lectura del sensor 
%u = table2array(mat(:, 4));      % Accion de control 

mrac = {}; 

mrac.time = table2array(mat(:, 1)); 
mrac.k_theta= table2array(mat(:, 2)); 
mrac.k_ref = table2array(mat(:, 3)); 
mrac.ref = table2array(mat(:, 4)); 
mrac.theta = table2array(mat(:, 5)); 
mrac.u = table2array(mat(:, 6)); 


%%
addpath("resultados-finos/")
mat = readtable("resultados-finos/mrac+ff.xlsx"); 

% Lectura de los datos 
%time = table2array(mat(:, 1));   % Tiempo
%ref = table2array(mat(:, 2));    % Referencia de control
%theta = table2array(mat(:, 3));  % lectura del sensor 
%u = table2array(mat(:, 4));      % Accion de control 
mrac_ff = {}; 

mrac_ff.time = table2array(mat(:, 1)); 
mrac_ff.k_theta= table2array(mat(:, 2)); 
mrac_ff.k_ref = table2array(mat(:, 3)); 
mrac_ff.ref = table2array(mat(:, 4)); 
mrac_ff.theta = table2array(mat(:, 5)); 
mrac_ff.u = table2array(mat(:, 6)); 

%% Validacion 
t = mrac.time; 
ref = mrac.ref; 

%% Seguimiento 
close all; 
figure; 
R = 0.7; 
plot(t, ref,"o--","LineWidth", 2 * R, "MarkerSize", R); 
hold on; 
plot(t, mrac.theta, "b-.","LineWidth", 2 * R, "MarkerSize", R); 
plot(mrac_ff.time, mrac_ff.theta, "--*r","LineWidth", R, "MarkerSize",0.8); 

hold off; 
xaxis([0 164.9]); 
legend("Ref", "MRAC", "MRAC+FF", "Location","southeast")
title("Feedforward effect on MRAC Controller. \gamma_x = 7 \gamma_r = 0.3 ")
xlabel("time [s]"); 
ylabel("measured angle \theta_m [º]"); 
grid on; 


