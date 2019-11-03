//@NIKHIL MISHRA
/*****************
 * BALLOON-SAT 
 * 
 * KRAGON-COMPUTER
 ****************/

//Header Files
#include "I2Cdev.h"
#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
    #include "Wire.h"
#endif
#include "MPU6050_6Axis_MotionApps20.h"
//Radio
#include <RF24Network.h>
#include <RF24.h>
#include <SPI.h>
//Sonar
//#include <NewPing.h>

// ===================================================
// ===                 Pin Layouts                 ===
// ===================================================
/*  Door Servo - 6
    Trigger Servo 1 - 5
    Trigger Servo 2 - 3

    Sonar trig - 8
          ECHO - 9

    LED - 
    A2 - Red
    A1 - 
 
    buzz - A0

    Relay Fire - A3/0
 */
// ===================================================
// ===                 MISC Vars                   ===
// ===================================================
//Timers Vars
unsigned long previousMillis = 0;
unsigned long previousMillis2 = 0;
unsigned long currentMillis;

const int RLED = A2;
const int buzz = A0;

bool engine_fire = false;
bool tilt_abort = false;

const int RELAY = A3; // ACTIVE LOW

// ===================================================
// ===               NewPing - SONAR               ===
// ===================================================
#define TRIGGER_PIN  8  
#define ECHO_PIN     9 
#define MAX_DISTANCE 400 

long duration;
int distance;

// ===================================================
// ===               RADIO - NRF24                 ===
// ===================================================
// RF24 radio(7,8); // CE, CSN          
// RF24Network network(radio); 

// ===================================================
// ===                      MPU6050                ===
// ===================================================
MPU6050 mpu;
#define maxrollangle 45
#define maxpitchangle 45

//gimbal lock
#define OUTPUT_READABLE_YAWPITCHROLL

#define INTERRUPT_PIN 2  // use pin 2 on Arduino Uno & most boards

bool dmpReady = false;  // set true if DMP init was successful
uint8_t mpuIntStatus;   // holds actual interrupt status byte from MPU
uint8_t devStatus;      // return status after each device operation (0 = success, !0 = error)
uint16_t packetSize;    // expected DMP packet size (default is 42 bytes)
uint16_t fifoCount;     // count of all bytes currently in FIFO
uint8_t fifoBuffer[64]; // FIFO storage buffer

Quaternion q;           // [w, x, y, z]         quaternion container
VectorFloat gravity;    // [x, y, z]            gravity vector
float euler[3];         // [psi, theta, phi]    Euler angle container
float ypr[3];           // [yaw, pitch, roll]   yaw/pitch/roll container and gravity vector

volatile bool mpuInterrupt = false;     
void dmpDataReady() {
    mpuInterrupt = true;
}

// ================================================================
// ===                      INITIAL SETUP                       ===
// ================================================================

void setup() {
    Serial.begin(115200);

    //Startup Sound
    tone(buzz, 2500, 300);
    delay(1000);
    tone(buzz, 2000, 300);
    delay(500);
    tone(buzz, 3000, 300);
    tone(buzz, 3500, 300);
    delay(500);
    tone(buzz, 2500, 300);
    delay(1000);
    delay(2000);

    delay(1000);

    pinMode(RELAY, OUTPUT);
    pinMode(buzz, OUTPUT);
    //Disable Mission Critical Functions:

    digitalWrite(RELAY, HIGH); //ACTIVE LOW

    //SONAR
    pinMode(TRIGGER_PIN, OUTPUT); // Sets the trigPin as an Output
    pinMode(ECHO_PIN, INPUT); // Sets the echoPin as an Input
    //setup MPU6050
    mpusetup();

    delay(1000);
}



// ================================================================
// ===                    MAIN PROGRAM LOOP                     ===
// ================================================================

void loop() {

    if (tilt_abort == false && engine_fire == false ) {
        GREEN();
        noTone(buzz);
    }

    if (engine_fire ==true) {

        POST_LAND();
        
    }
    mpudata();
    sonarr();

    //Tilt Abort Program
    if (ypr[1] * 180/M_PI > maxrollangle || ypr[1] * 180/M_PI < -maxrollangle) {
        Serial.println("MAX Roll Tilted!!");
        tilt_abort = true;
    }
    if (ypr[2] * 180/M_PI > maxpitchangle || ypr[2] * 180/M_PI < -maxpitchangle) {
        Serial.println("MAX Pitch Tilted!!");
        tilt_abort = true;
    }


    //Descent Engine Fire
/* && tilt_abort == true && engine_fire == false */
    if (distance < 20 && distance != 0  && engine_fire == false) {
            //Tilt Abort Program
        if ((ypr[1] * 180/M_PI > maxrollangle || ypr[1] * 180/M_PI < -maxrollangle ) || (ypr[2] * 180/M_PI > maxpitchangle || ypr[2] * 180/M_PI < -maxpitchangle)) {
            Serial.println("MAX Roll or Pitch Tilted!!");
            tilt_abort = true;
        }

        if (tilt_abort == false) {
        Serial.println("FIRING!");
        delay(10);
        digitalWrite(RELAY, LOW); //ACTIVE LOW

        RED();

        delay(5000);

        engine_fire = true;

        digitalWrite(RELAY, HIGH); //REST ENGINE

        digitalWrite(buzz, HIGH);
        }
    }
    
}

void mpusetup() {

    // join I2C bus (I2Cdev library doesn't do this automatically)
    #if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
        Wire.begin();
        Wire.setClock(400000); // 400kHz I2C clock. Comment this line if having compilation difficulties
    #elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
        Fastwire::setup(400, true);
    #endif


    // initialize device
    Serial.println(F("Initializing I2C devices..."));
    mpu.initialize();
    pinMode(INTERRUPT_PIN, INPUT);

    // verify connection
    Serial.println(F("Testing device connections..."));
    Serial.println(mpu.testConnection() ? F("MPU6050 connection successful") : F("MPU6050 connection failed"));

    //MPU delay 
    delay(3000);

    // load and configure the DMP
    Serial.println(F("Initializing DMP..."));
    devStatus = mpu.dmpInitialize();

    // supply your own gyro offsets here, scaled for min sensitivity
    mpu.setXGyroOffset(220);
    mpu.setYGyroOffset(76);
    mpu.setZGyroOffset(-85);
    mpu.setZAccelOffset(1788); // 1688 factory default for my test chip

    // make sure it worked (returns 0 if so)
    if (devStatus == 0) {
        // Calibration Time: generate offsets and calibrate our MPU6050
        mpu.CalibrateAccel(6);
        mpu.CalibrateGyro(6);
        mpu.PrintActiveOffsets();
        // turn on the DMP, now that it's ready
        Serial.println(F("Enabling DMP..."));
        mpu.setDMPEnabled(true);

        // enable Arduino interrupt detection
        Serial.print(F("Enabling interrupt detection (Arduino external interrupt "));
        Serial.print(digitalPinToInterrupt(INTERRUPT_PIN));
        Serial.println(F(")..."));
        attachInterrupt(digitalPinToInterrupt(INTERRUPT_PIN), dmpDataReady, RISING);
        mpuIntStatus = mpu.getIntStatus();

        // set our DMP Ready flag so the main loop() function knows it's okay to use it
        Serial.println(F("DMP ready! Waiting for first interrupt..."));
        dmpReady = true;

        // get expected DMP packet size for later comparison
        packetSize = mpu.dmpGetFIFOPacketSize();
    } else {
        // ERROR!
        // 1 = initial memory load failed
        // 2 = DMP configuration updates failed
        // (if it's going to break, usually the code will be 1)
        Serial.print(F("DMP Initialization failed (code "));
        Serial.print(devStatus);
        Serial.println(F(")"));
    }

}

void mpudata() {
    // if programming failed, don't try to do anything
    if (!dmpReady) return;

    // wait for MPU interrupt or extra packet(s) available
    while (!mpuInterrupt && fifoCount < packetSize) {
        if (mpuInterrupt && fifoCount < packetSize) {
          // try to get out of the infinite loop 
          fifoCount = mpu.getFIFOCount();
        }  
        // other program behavior stuff here
        // .
        // .
        // .
        // if you are really paranoid you can frequently test in between other
        // stuff to see if mpuInterrupt is true, and if so, "break;" from the
        // while() loop to immediately process the MPU data
        // .
        // .
        // .
    }

    // reset interrupt flag and get INT_STATUS byte
    mpuInterrupt = false;
    mpuIntStatus = mpu.getIntStatus();

    // get current FIFO count
    fifoCount = mpu.getFIFOCount();
	if(fifoCount < packetSize){
	        //Lets go back and wait for another interrupt. We shouldn't be here, we got an interrupt from another event
			// This is blocking so don't do it   while (fifoCount < packetSize) fifoCount = mpu.getFIFOCount();
	}
    // check for overflow (this should never happen unless our code is too inefficient)
    else if ((mpuIntStatus & _BV(MPU6050_INTERRUPT_FIFO_OFLOW_BIT)) || fifoCount >= 1024) {
        // reset so we can continue cleanly
        mpu.resetFIFO();
      //  fifoCount = mpu.getFIFOCount();  // will be zero after reset no need to ask
        Serial.println(F("FIFO overflow!"));

    // otherwise, check for DMP data ready interrupt (this should happen frequently)
    } else if (mpuIntStatus & _BV(MPU6050_INTERRUPT_DMP_INT_BIT)) {

        // read a packet from FIFO
	while(fifoCount >= packetSize){ // Lets catch up to NOW, someone is using the dreaded delay()!
		mpu.getFIFOBytes(fifoBuffer, packetSize);
		// track FIFO count here in case there is > 1 packet available
		// (this lets us immediately read more without waiting for an interrupt)
		fifoCount -= packetSize;
	}
        #ifdef OUTPUT_READABLE_YAWPITCHROLL
            mpu.dmpGetQuaternion(&q, fifoBuffer);
            mpu.dmpGetGravity(&gravity, &q);
            mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);
            // Serial.print("ypr\t");
            // Serial.print(ypr[0] * 180/M_PI);
            // Serial.print("\t");
            // Serial.print(ypr[1] * 180/M_PI);
            // Serial.print("\t");
            // Serial.println(ypr[2] * 180/M_PI);
        #endif
        
    }
}

void sonarr() {
    // Clears the trigPin
  digitalWrite(TRIGGER_PIN, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(TRIGGER_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGGER_PIN, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(ECHO_PIN, HIGH);
  // Calculating the distance
  distance = duration * 0.034 / 2;
  // Prints the distance on the Serial Monitor
  Serial.println(distance);
}

void red_buzz() {
        digitalWrite(buzz, HIGH);
        delay(200);
        digitalWrite(buzz, LOW);
                digitalWrite(buzz, HIGH);
        delay(200);
        digitalWrite(buzz, LOW);
                digitalWrite(buzz, HIGH);
        delay(200);
        digitalWrite(buzz, LOW);
                digitalWrite(buzz, HIGH);
        delay(200);
        digitalWrite(buzz, LOW);
}

void RED()
{
  digitalWrite(RLED, HIGH);
  tone(buzz, 2500, 100);
  delay(200);
  digitalWrite(RLED, LOW);

  tone(buzz, 2500, 100);
  delay(200);
  digitalWrite(RLED, HIGH);

  tone(buzz, 2000, 100);
  delay(500);
  tone(buzz, 2000, 100);
  noTone(buzz);
}



void GREEN() {
  //Everything is fine.. signal.
  unsigned long interval = 1000;
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis > interval) {
    previousMillis = currentMillis;

    digitalWrite(RLED, HIGH);
    tone(buzz, 2500, 100);
  }
  else {
    digitalWrite(RLED, LOW);
  }
}

void POST_LAND() {
  //Everything is fine.. signal.
  unsigned long interval = 2000;
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis > interval) {
    previousMillis = currentMillis;

    digitalWrite(RLED, HIGH);
    tone(buzz, 2500, 100);
  }
  else {
    digitalWrite(RLED, LOW);
  }
}
