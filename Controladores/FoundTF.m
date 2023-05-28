function [Gss, G, Gnum, Gden] =FoundTF(thetap)
%Pesos [N]
wb=0.105948;
wcp= 0.014145;
wm=0.030411; 

%Distancias [m]
d1=130.75e-3;
d2=50.75e-3;
e=41.91e-3;

%Momento de inercia [kg-m^2]
I=15e-5; 

%Coeficiente término \theta
Coeficiente = wcp*d2-wm*d1-wb*e;

%Constante del motor
km=0.00118;

%Constante de Fricción
beta = 1.667;
miu=beta*2*I;

%TransferFunction
s=tf('s');
c = (((wcp*d2-wm*d1-wb*e)/I)*(-cosd(thetap)));
G=((km*d1)/I)/((s^2)+(miu/I)*s+(((wcp*d2-wm*d1-wb*e)/I)*(-cosd(thetap))));


Gnum = cell2mat(G.Numerator);
Gden = cell2mat(G.Denominator);

%Parametros del motor
DZ = 3;
MAX_PWM = 80;

%Variables de estado
ass = (km*d1)/I;
bss = (wcp*d2-wm*d1-wb*e)/I;
css = miu/I;
A = [0 1; bss*cosd(thetap) -css];
B = [0 ass]';
C = [0 1];
D = [0];

Gss = ss(A,B,C,D);
Gss.StateName= {'Velocity','Position'};
Gss.OutputName= {'Position'};

save('TF.mat');

end