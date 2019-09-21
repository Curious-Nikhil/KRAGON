//@Nikhil Mishra
// KRAWLER's Computer :D
#include <AFMotor.h>
#include <Servo.h> 

/* DC Motors: */
AF_DCMotor motor1(1);
AF_DCMotor motor2(2);
AF_DCMotor motor3(3);
AF_DCMotor motor4(4);
/* Servo Motors: */
Servo servo1;

void setup() {

    Serial.begin(9600);

    Serial.println(F("KRAWLER BOOTING UP.."));

    servo1.attach(9); //ServoMotor2 on board
 
}


void loop() {

    /**
     * Detect Landing
     * Wait and Charge
     * Move and Avoid Obstacle
     * 
     */

}


void move_forward() {

    motor.run(FORWARD);
    motor.setSpeed(200);



}

void move_backward() {

}

void rover_exit() {
    //Detect landing using a light sensor

    //wait for few 5 minutes:

    //Detect if door is open

    //move out.

    //move far enough and stop

    //wait and Charge

    //send happy signal to ground station!
}