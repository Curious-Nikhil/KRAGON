%Balloon-Sat Program: @NikhilMishra

close all;

Da = 1.29; %Density of Air - kg/m3
DHe = 0.179; %Density of He - kg/m3
Mba = 0.1; % Weight of Balloon's Skin - grams

r = 0.1:0.1:2;

Mp = ((4/3)*pi*power(r, 3))*(Da - DHe) - Mba;

plot(r, Mp)
title("Mass vs Radius");
xlabel("Radius of Balloon (meters)");
ylabel("Mass lift Capabality (Kilograms)");

text(1, 10  , 'Optimal Radius', 'Color', 'k')
grid on

