clear all;
close all;
load('FL20SEP_2.mat', 'FL20SEP_2')

figure
yyaxis left
plot(FL20SEP_2.t, FL20SEP_2.agl, FL20SEP_2.t, FL20SEP_2.agl_kmf);
xlabel('Time (seconds)')
ylabel('AGL (m)')
title('PARACHUTE TEST - 20TH SEP');

yyaxis right
plot(FL20SEP_2.t, FL20SEP_2.vkmf)
ylabel('Velocity (KMF) m/s')

figure
yyaxis left
plot(FL20SEP_2.t, FL20SEP_2.agl, FL20SEP_2.t, FL20SEP_2.agl_kmf);
xlabel('Time(s)')
ylabel('AGL (m)')
title('PARACHUTE TEST - 20TH SEP');
yyaxis right
plot(FL20SEP_2.t, FL20SEP_2.ax , 'g', FL20SEP_2.t, FL20SEP_2.ay,'blue', FL20SEP_2.t, FL20SEP_2.az , 'black');
ylabel('Accel - x, y, z')