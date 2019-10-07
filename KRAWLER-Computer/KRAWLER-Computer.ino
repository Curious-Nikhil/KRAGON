//@Nikhil Mishra
// KRAWLER's Computer :D
#include <AFMotor.h>
#include <Servo.h> 

/* DC Motors: */
AF_DCMotor motor1(1); //Back Left
AF_DCMotor motor2(2); //Back Right
AF_DCMotor motor3(3); //Front Right
AF_DCMotor motor4(4); //Front Left
/* Servo Motors: */
Servo servo1;

void setup() {

    Serial.begin(9600);
    Serial.println(F("KRAWLER BOOTING UP.."));
    servo1.attach(9); //ServoMotor2 on board


    //Motors:
    motor1.setSpeed(200);
    motor2.setSpeed(200);
    motor3.setSpeed(200);
    motor4.setSpeed(200);

    motor1.run(RELEASE);
    motor2.run(RELEASE);
    motor3.run(RELEASE);
    motor4.run(RELEASE);
 
}


void loop() {

    /**
     * Detect Landing Detect landing using a light sensor
     * Wait and Charge
     * Move and Avoid Obstacle
     * 
     */
    
    move_forward();

}


void move_forward() {

    //Back Left - BACKWARD
    //Back Right - FORWARD
    //Front Right - BACKWARD
    //Front Left - FORWARD

    motor1.run(BACKWARD);
    motor2.run(FORWARD);
    motor3.run(BACKWARD);
    motor4.run(FORWARD);
 



}

void move_backward() {

    motor1.run(FORWARD);
    motor2.run(BACKWARD);
    motor3.run(FORWARD);
    motor4.run(BACKWARD);

}

void rover_exit() {
    
    delay(10000); 

    //beep
    move_forward();
    delay(5000);



    //wait for few 5 minutes:

    //Detect if door is open

    //move out.

    //move far enough and stop

    //wait and Charge

    //send happy signal to ground station!
}