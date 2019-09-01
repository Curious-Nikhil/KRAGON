//@NIKHIL MISHRA
/*****************
 * BALLOON-SAT 
 * 
 * KRAGON-COMPUTER
 ****************/

//Header Files
#include "i2c.h"
#include "i2c_BMP280.h"
#include "MPU6050.h"
#include <Wire.h>
#include "SimpleKalmanFilter.h"
#include <SD.h>

#define INTERRUPT_PIN 2  // 
#define RLED 6// Green LED
#define GLED 7// Green LED
#define buzzer 8 
#define pyroPin 9

// =============================================
// ===          MISC Global Vars             ===
// =============================================

//Timers Vars
unsigned long previousMillis = 0;
unsigned long currentMillis;


//bools
bool release = false;
bool landed = false;
bool pyro = false;


// =============================================
// ===              BAROMETER                ===
// =============================================
BMP280 bmp280;
SimpleKalmanFilter pressureKalmanFilter(1, 1, 0.01);

static float alt;
float est_alt;
float temperature;
float pascal;
float base_alt;
// =============================================
// ===              SD CARD                  ===
// =============================================
String filename;
File myFile;
int sd_count = 0; //flusher
// =============================================
// ===              MPU Vars                 ===
// =============================================

MPU6050 accelgyro;

int16_t ax, ay, az;
int16_t gx, gy, gz;
//BMP280

void setup() {
    Serial.begin(9600);
    
    //Startup Sound
    tone(buzzer, 2500, 300);
    delay(1000);
    tone(buzzer, 2000, 300);
    delay(500);
    tone(buzzer, 3000, 300);
    tone(buzzer, 3500, 300);
    delay(500);
    tone(buzzer, 2500, 300);
    delay(1000);
    delay(2000);

    // configure LED for output

    pinMode(GLED, OUTPUT);
    pinMode(RLED, OUTPUT);
    pinMode(pyroPin, OUTPUT);
    digitalWrite(pyroPin, LOW); //Disable for safety

    Serial.println(FreeRam()); //set a threshold for that.

      //PASS 1 -- confirm the size.
  if (FreeRam() < 275) {
    Serial.println(F("RAM-0"));
    RED();
    while(1);
  }

  // PASS 2: Initialize SD Module
  initializeSD();

  //PASS 3: Initialize Gyroscope and Servo
    //needs calibration method also.
  initializeMPU();

  //PASS 3: Initialize Baromter
  initializeBMP();

  //PASS 4: Pyro Check

  //Get baseline alt
  float sum = 0;
  delay(5000);
  for (int i=0; i < 30; i++) {

    delay(100);

    bmp280.getAltitude(base_alt);
    sum += base_alt;
    Serial.println(base_alt);
  }

  base_alt = sum/30.0;
  Serial.print(F("BASEH: "));
  Serial.println(base_alt);

  delay(2000);

}

void loop() {

      //Disable Pyros
  
  if(release == false && pyro == false && landed == false) {
    GREEN();
  }
  

}
// ================================================================
// ===                RUN FUNCTIONS                      ===
// ================================================================
void get_alt() {
  bmp280.getAltitude(alt);
  bmp280.getPressure(pascal);
  est_alt = pressureKalmanFilter.updateEstimate(alt);
}

void motion() {
    accelgyro.getAcceleration(&ax, &ay, &az);
}


// ================================================================
// ===                Initialize FUNCTIONS                      ===
// ================================================================
// ================================================================
// ===               SD CARD Begin                              ===
// ================================================================
//Create a new filename everytime.
boolean loadSDFile() {
  int i = 0;
  boolean file = false;

  while (!file && i < 1024) {
    filename = (String)i + "FL.csv";

    if (!SD.exists(filename)) {
      myFile = SD.open(filename, FILE_WRITE);
      delay(10);
      myFile.close();
      file = true;
    }
    i++;
  }

  return file;
}

void initializeSD() {

  Serial.print(F("InitSD"));

  if (!SD.begin(4)) {
    Serial.println(F("SDFAIL"));
    RED();
    while (1);
  }
  Serial.println(F("SDinit"));

  //Create a file with new name
  if (!loadSDFile()) {
    Serial.println(F("F-0"));
    while (1);
    RED();
  }
  else {
    Serial.println(F("F-1"));
  }

  Serial.println(filename);

  myFile = SD.open(filename, FILE_WRITE);
  Serial.println(myFile);
  if (myFile) {
    //Print Header Files  - - alt, pascal, est_alt, mpuPitch, mpuRoll, mpuYaw, OutputX, OutputY

    myFile.print("Time");
    myFile.print(",");
    myFile.print("Pascal");
    myFile.print(",");
    myFile.print("alt");
    myFile.print(",");
    myFile.print("KMF");
    myFile.print(",");
    myFile.print("ax");
    myFile.print(",");
    myFile.print("ay");
    myFile.print(",");
    myFile.print("az");
    myFile.print(",");
    myFile.print("LaunchTime");
    myFile.print(",");
    myFile.print("ApoTime");
    myFile.print(",");
    myFile.println("LandTime");

    myFile.close();
    Serial.println(F("Fin-1"));

  } else {
    Serial.print(F("Fin-0"));
    RED();
    while (1);
  }
}


// ================================================================
// ===                         BAROMETER                       ===
// ================================================================

void initializeBMP() {

  Serial.print(F("InintBMP"));
  if (bmp280.initialize()) Serial.println(F("BMP1")); //sensor found
  else
  {
    Serial.println(F("BMP0")); //Sensor not found
    RED();
    while (1) {}
  }

  //Calibration Settings - https://www.best-microcontroller-projects.com/bmp280.html#L1080
  bmp280.setPressureOversampleRatio(10); //Oversampling Ratio!
  bmp280.setTemperatureOversampleRatio(1);
  bmp280.setFilterRatio(4);
  bmp280.setStandby(0);


  // onetime-measure:
  bmp280.setEnabled(0);
  bmp280.triggerMeasurement();

  Serial.println(F("BMPInit1"));
}


// ================================================================
// ===                  MISC FUNCTIONS                          ===
// ================================================================
void RED()
{
  digitalWrite(RLED, HIGH);
  tone(buzzer, 2500, 100);
  delay(200);
  digitalWrite(RLED, LOW);

  tone(buzzer, 2500, 100);
  delay(200);
  digitalWrite(RLED, HIGH);

  tone(buzzer, 2000, 100);
  delay(500);
  tone(buzzer, 2000, 100);
}

void GREEN() {
  //Everything is fine.. signal.
  unsigned long interval = 1000;
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis > interval) {
    previousMillis = currentMillis;
    
    digitalWrite(GLED, HIGH);
    tone(buzzer, 2500, 100);
  }
  else {
    digitalWrite(GLED, LOW);
  }
}