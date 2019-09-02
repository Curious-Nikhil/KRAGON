close all;
clear all;

vt=45.2; %terminal velocity for b=.36
vtn=7.8; %terminal velocity for b=12
y(1)=1000; %initial height
t(1)=0; %initial time
v(1)=0; %initial velocity
b=.6; %strength of drag (parachute closed)
m=75; %mass in kg
N=1000; %number of time to run
dt=.1; %the time step
h=900; %parachute opening height
va(1)=0; %initial velocity


for i=1:N;
    t(i+1)=t(i)+dt; %calculates time
    v(i+1)=v(i)+ dt*(-9.8); %calculates velocity
    fd(i+1)=b*(v(i).^2); %calculates drag with above velocity
    fn(i+1)=735-fd(i); %calulates net force on person
    a(i+1)=fn(i)/m; %uses above net force calc. to calculate       acceleration
    va(i+1)=va(i)-dt*(a(i)); %uses above velocity and calculates actual velocity of person
    y(i+1)=y(i)+(v(i)*dt); %finds the position of the falling person
    if va(i) < -vt; %if terminal velocity is met, all velocities following are equal to -vt (-45.2)
        va(i)= -vt;
    end;
    if y(i) < 0; %makes sure the person doesnt fall into the earth dead or alive
        break;
    end;
    if h > y(i); %checks to see if/when the parachute is opened
       b=12; %changes drag to this value (parachute open)
    end;
    if b==12 && va(i) < -vtn; %checks to see if the parachute is open and terminal velocity is reached for an open parachute
        va(i) = -vtn;
    end;
end;