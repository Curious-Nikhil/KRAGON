close all;

figure 
yyaxis left
plot(PARAFL1.t, PARAFL1.alt,PARAFL1.t, PARAFL1.KMF,'r');
xlabel("Time(s)")
ylabel("Height(m) - Filter")
title("Parachute Test Flight Log - 1")
grid on

hold on

plot(PARAFL1.t(327), PARAFL1.KMF(327), '*r')
legend('Raw Alt','Alt(Kalman)', 'Final Velocity','Velocity')

yyaxis right

plot(PARAFL1.t, PARAFL1.v.*-1000, 'black');
ylabel("Velocity(m/s)")

%Accel Data
figure 
yyaxis left
plot(PARAFL1.t, PARAFL1.alt,PARAFL1.t, PARAFL1.KMF, 'black');
xlabel("Time(s)")
ylabel("Height(m) - Filter")
title("PARA Flight Log - 1")

yyaxis right

plot(PARAFL1.t, PARAFL1.ax, 'r', PARAFL1.t, PARAFL1.ay, 'b',PARAFL1.t, PARAFL1.az,'g');
ylabel("Acceleration")

legend('RAW Alt', 'Alt Kalman', 'AX', 'AY', 'AZ')
