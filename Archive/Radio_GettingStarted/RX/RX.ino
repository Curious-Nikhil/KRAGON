/*
  Arduino Wireless Communication Tutorial
        Example 1 - Receiver Code

  by Dejan Nedelkovski, www.HowToMechatronics.com

  Library: TMRh20/RF24, https://github.com/tmrh20/RF24/
*/

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(7, 8); // CE, CSN

const byte addresses[][6] = { "00001", "00002"};

float remoteA1;
float previousA1;


struct package
{
  float a1 = 0.0;
  float alt;
  float kmf;
  bool drop = false;
};

typedef struct package Package;
Package data;

String command;
void setup() {
  Serial.begin(9600);
  pinMode(4, INPUT);
  radio.begin();

  radio.openWritingPipe(addresses[0]); // 00001
  radio.openReadingPipe(1, addresses[1]); // 00002

  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();
}

void loop() {

  //checkForTX();
  //printA1();


  delay(5);
  radio.startListening();

  if (radio.available()) {
    while (radio.available()) {
      radio.read(&data, sizeof(data));

      Serial.println(data.a1);
    }
  }

  delay(5);

  radio.stopListening();

  if( digitalRead(4) == HIGH) {
    data.drop = 1;
  }

  Serial.println(digitalRead(4));
  radio.write(&data, sizeof(data));
}

void checkForTX() {

  if ( radio.available()) {
    while (radio.available())
    {
      radio.read(&data, sizeof(data));
      remoteA1 = data.a1;
    }
    Serial.print("\nPackage: ");
    Serial.print("\n");
    Serial.println(data.a1);
  }
}


void printA1() {
  String a1;

  if (remoteA1 != previousA1) {
    if (remoteA1 == 0.0 ) {
      a1 = "---";

    } else {
      a1 = String(remoteA1, 1);
    }

    Serial.println(a1);
    previousA1 = remoteA1;
  }

}
