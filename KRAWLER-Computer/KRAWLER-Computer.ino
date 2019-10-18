//@Nikhil Mishra
// KRAWLER's Computer :D
//SONAR - ECHO - 9 TRIG - 4
/*
  NRF24L01 - 
  CE - 8
  CSN - 2
  MISO - 11
  MOSI - 12
  SCK - 13
 */
#include <AFMotor.h>
#include <NewPing.h>


//Timers
unsigned long last;
unsigned long now;
unsigned long period = 1000;
int i = 0;

//MISC Vars
bool door;
bool rover_out = false;
int mspeed;

/* DC Motors: */
AF_DCMotor motor1(1); //Back Left
AF_DCMotor motor2(2); //Back Right
AF_DCMotor motor3(3); //Front Right
AF_DCMotor motor4(4); //Front Left

//SONAR:
#define TRIGGER_PIN  4
#define ECHO_PIN     9
#define MAX_DISTANCE 400

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);


void setup() {

  Serial.begin(9600);
  Serial.println(F("KRAWLER BOOTING UP.."));
  //servo1.attach(9); //ServoMotor2 on board


  //Motors:
  motor1.setSpeed(255);
  motor2.setSpeed(255);
  motor3.setSpeed(255);
  motor4.setSpeed(255);

  motor1.run(RELEASE);
  motor2.run(RELEASE);
  motor3.run(RELEASE);
  motor4.run(RELEASE);

  // delay(5000);

}


void loop() {

  /**
     Detect Landing Detect landing using a light sensor
     Wait and Charge
     Move and Avoid Obstacle

  */

  // move_forward();
  // delay(1000);
  // motor_release();
  // delay(1000);
  // move_backward();
  // delay(1000);
  // motor_release();

  //delay(50);                     
  Serial.println(sonar.ping_cm()); 


    //move_left();

    // if (sonar.ping_cm() > 15 && rover_out == false) {
    //     Serial.println("DOOR IS CLOSED!");

    //     door = false; //opened
    // }

    // if(door == false && rover_out == false) {
    //     rover_exit();
    //     Serial.println("ROVER IS OUT! GOOD LUCK!");
    // }

}


void move_forward(int mspeed) {

  //Back Left - BACKWARD
  //Back Right - FORWARD
  //Front Right - BACKWARD
  //Front Left - FORWARD
  motor1.setSpeed(mspeed);
  motor2.setSpeed(mspeed);
  motor3.setSpeed(mspeed);
  motor4.setSpeed(mspeed);

  motor1.run(FORWARD);
  motor2.run(FORWARD);
  motor3.run(BACKWARD);
  motor4.run(FORWARD);
}

void motor_release() {
  motor1.run(RELEASE);
  motor2.run(RELEASE);
  motor3.run(RELEASE);
  motor4.run(RELEASE);
}

void move_backward(int mspeed) {
  motor1.setSpeed(mspeed);
  motor2.setSpeed(mspeed);
  motor3.setSpeed(mspeed);
  motor4.setSpeed(mspeed);

  motor1.run(BACKWARD);
  motor2.run(BACKWARD);
  motor3.run(FORWARD);
  motor4.run(BACKWARD);

}

void move_left() {
  motor4.run(BACKWARD);
  //motor1.run(BACKWARD);

  motor2.run(FORWARD);
  motor3.run(BACKWARD);
}

void move_right() {
  motor4.run(FORWARD);
  motor1.run(FORWARD);

  motor2.run(BACKWARD);
  motor3.run(FORWARD);

}


void rover_exit() {
  Serial.println("ROVER_EXIT");
  delay(2000);

  //beep

  move_forward(255);

  delay(5000);
  
  //motor_release();
  //wait for few 5 minutes:
  //delay(1000);

  //wait and Charge

  //send happy signal to ground station!

  rover_out = true;
}
