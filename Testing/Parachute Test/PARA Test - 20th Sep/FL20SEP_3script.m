clc;
clear all;
close all;

load('FL20SEP_3.mat')
figure

plot(FL20SEP_3.t, FL20SEP_3.agl, FL20SEP_3.t, FL20SEP_3.agl_kmf);