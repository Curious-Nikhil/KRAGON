%Balloon-Sat Program: @NikhilMishra

close all;

Da = 1.29; %Density of Air - kg/m3
DHe = 0.179; %Density of He - kg/m3
Mba = 0.1; % Weight of Balloon's Skin - grams

r = [0.1,0.2,0.3,0.4,0.5,0.6,0.7,0.8,0.9,1.0,1.1,1.2,1.3,1.4,1.5];

Mp = ((4/3)*pi*power(r, 3))*(Da - DHe) - Mba;

plot(r, Mp)
title("Radius vs Mass");
xlabel("Radius of Balloon");
ylabel("Mass lift Capabality");


