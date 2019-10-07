//@NIKHIL MISHRA
/*****************
 * BALLOON-SAT 
 * 
 * KRAGON-COMPUTER
 ****************/

//Header Files
#include "Adafruit_BMP280.h"
#include <Wire.h>
#include "SimpleKalmanFilter.h"

#define RLED 6// Green LED
#define GLED 7// Green LED
#define buzzer 8 


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


// ================================================================
// ===                Initialize FUNCTIONS                      ===
// ================================================================

// ================================================================
// ===                         BAROMETER                       ===
// ================================================================

void initializeBMP() {
  Serial.print(F("BMP"));

  if (!bmp280.begin()) {
    Serial.println(F("0"));
    while (1);
  }

  /*For drop detection
    osrs_p = 2
    osrs_t = 1
    IIR = 0 (off)
    t_sb = 0 (0.5ms)
  */

  /* Default settings from datasheet. */
  bmp280.setSampling(Adafruit_BMP280::MODE_NORMAL,     /* Operating Mode. */
                     Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
                     Adafruit_BMP280::SAMPLING_X1,    /* Pressure oversampling */
                     Adafruit_BMP280::FILTER_OFF,      /* Filtering. */
                     Adafruit_BMP280::STANDBY_MS_1); /* Standby time. */

  Serial.println(F("BMPInit1"));
}

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