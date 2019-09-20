close all;

figure 
yyaxis left
plot(PARAFL2.t, PARAFL2.alt,PARAFL2.t, PARAFL2.KMF);
xlabel("Time(s)")
ylabel("Height(m) - Filter")
title("PARA Flight Log - 1")

yyaxis right

plot(PARAFL2.t, PARAFL2.v);
ylabel("Velocity(m/s)")

%Accel Data
figure 
yyaxis left
plot(PARAFL2.t, PARAFL2.alt,PARAFL2.t, PARAFL2.KMF);
xlabel("Time(s)")
ylabel("Height(m) - Filter")
title("PARA Flight Log - 1")

yyaxis right

plot(PARAFL2.t, PARAFL2.ax, PARAFL2.t, PARAFL2.ay,PARAFL2.t, PARAFL2.az);
ylabel("Acceleration")

