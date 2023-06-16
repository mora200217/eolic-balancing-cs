% 
syms a
A = [0 1 0 0; 0 0 1 -2; 0 2 0 1; -1 -12 -12 -6]; 
B = [0 0 1 -6 + 2]'; 
C = [1 0 0 0]; 
D = 0; 

G = ss(A, B, C, D); 
tf(G)


%% 
if det(obsv(G)) ~= 0
    disp("Es observable "); 
else 
    disp("No "); 
end 