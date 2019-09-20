close all;
load('FL20SEP_1.mat')
figure 

yyaxis left

plot(FL20SEP_1.t, FL20SEP_1.alt, FL20SEP_1.t, FL20SEP_1.KMF);
xlabel('Time');
ylabel('Altitude');
title('PARACHUTE TEST - 20th SEP')

yyaxis right
plot(FL20SEP_1.t, FL20SEP_1.v_kmf);

figure
plot(FL20SEP_1.t, FL20SEP_1.alt, FL20SEP_1.t, FL20SEP_1.KMF);
xlabel('Time');
ylabel('Altitude');
title('PARACHUTE TEST - 20th SEP')

figure 
plot(FL20SEP_1.t, FL20SEP_1.v_kmf);

