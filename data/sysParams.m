%% SysParams 
clear; clc; close all; 
wb=0.105948%-5.66E-4
wcp= 0.014145;% 0.044145; 
wm=0.030411; 
d1=130.75e-3
d2=50.75e-3

e=41.91e-3; % Offset centro de masa 

I=5.8e-5; 

Coeficiente = wcp*d2-wm*d1-wb*e
thetap = 0; 
correccion = 0.5; 
k=0.0694%0.0177%0.1008154434*correccion
Gamma = 1.25;%0.816685;%0.85
b=Gamma*2*I
s=tf('s');
c = (((wcp*d2-wm*d1-wb*e)/I)*(-cosd(thetap)));
G=((d1)/I)/((s^2)+(b/I)*s+(((wcp*d2-wm*d1-wb*e)/I)*(-cosd(thetap))))
pole(G)

% step(G)
Gnum = cell2mat(G.Numerator)
Gden = cell2mat(G.Denominator)
pwm=0.5
timeR = 0.4

[Gm Pm Wcg Wcp] = margin(G)
Gm = 20*log10(Gm)

PWM_INPUT = 10; 
MAX_PWM = 80; 
DZ = 3; 

%% Save the data
save("NLsysParams"); 

