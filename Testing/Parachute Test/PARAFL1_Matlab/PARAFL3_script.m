close all;

figure 
yyaxis left
plot(PARAFL3.t, PARAFL3.alt,PARAFL3.t, PARAFL3.KMF);
xlabel("Time(s)")
ylabel("Height(m) - Filter")
title("PARA Flight Log - 1")

yyaxis right

plot(PARAFL3.t, PARAFL3.vf);
ylabel("Velocity(m/s)")

%Accel Data
figure 
yyaxis left
plot(PARAFL3.t, PARAFL3.alt,PARAFL3.t, PARAFL3.KMF);
xlabel("Time(s)")
ylabel("Height(m) - Filter")
title("PARA Flight Log - 1")

yyaxis right

plot(PARAFL3.t, PARAFL3.ax, PARAFL3.t, PARAFL3.ay,PARAFL3.t, PARAFL3.az);
ylabel("Acceleration")

