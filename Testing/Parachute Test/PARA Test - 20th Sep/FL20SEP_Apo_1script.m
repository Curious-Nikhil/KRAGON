clc;
clear all;
close all;
load('FL20SEP_Apo_1.mat')
load('FL20SEP_Apo_2.mat')
figure 

plot(FL20SEP_Apo_1.t, FL20SEP_Apo_1.agl, FL20SEP_Apo_1.t, FL20SEP_Apo_1.agl_kmf)

figure

plot(FL20SEP_Apo_2.t, FL20SEP_Apo_2.agl, FL20SEP_Apo_2.t, FL20SEP_Apo_2.agl_kmf)
xlabel('Time (s)')
ylabel('AGL (m)')
title('TEJAS - FLIGHT COMPUTER - APOGEE TEST')
grid on

figure

yyaxis left
plot(FL20SEP_Apo_2.t, FL20SEP_Apo_2.agl,'black',FL20SEP_Apo_2.t, FL20SEP_Apo_2.agl_kmf)
xlabel('Time (s)')
ylabel('AGL (m)')
title('TEJAS - FLIGHT COMPUTER - APOGEE TEST')
grid on

yyaxis right

plot(FL20SEP_Apo_2.t, FL20SEP_Apo_2.apo);


figure

yyaxis left
plot(FL20SEP_Apo_2.t, FL20SEP_Apo_2.agl,'black',FL20SEP_Apo_2.t, FL20SEP_Apo_2.agl_kmf)
xlabel('Time (s)')
ylabel('AGL (m)')
title('TEJAS - FLIGHT COMPUTER - APOGEE TEST')
grid on

yyaxis right

plot(FL20SEP_Apo_2.t, FL20SEP_Apo_2.ax, FL20SEP_Apo_2.t, FL20SEP_Apo_2.ay, FL20SEP_Apo_2.t, FL20SEP_Apo_2.az);