%% Import data 
addpath("resultados-finos/")
mat = readtable("resultados-finos/Hinf.xlsx"); 

% Lectura de los datos 
time = table2array(mat(:, 1));   % Tiempo
ref = table2array(mat(:, 2));    % Referencia de control
theta = table2array(mat(:, 3));  % lectura del sensor 
u = table2array(mat(:, 4));      % Accion de control 

%% Validacion 
plot(time, ref, time, theta); 
legend("reference", "theta")
