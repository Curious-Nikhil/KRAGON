%Parachute of Lander: @NikhilMishra

%Vars:
g = 9.8; % Acceleration due to Gravity m/s2
u = 0;

t = 0.1:0.1:6;
s = 0.5*g*power(t, 2);

v = u + g*t;

plot(t, v);

