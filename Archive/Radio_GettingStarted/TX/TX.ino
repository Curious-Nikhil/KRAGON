//https://www.hackster.io/jrance/arduino-nrf24l01-wireless-weather-station-f6c63f

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(7, 8); // CE, CSN

const byte address[6] = "00001";

int i = 0;

struct package
{
  float a1 = 0.0;
};

typedef struct package Package;
Package data;


void setup() {
  Serial.begin(9600);

  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MIN);
  radio.stopListening();

  pinMode(A1, INPUT);

}

void loop() {

  data.a1 = analogRead(A1);
  Serial.println(data.a1);
  radio.write(&data, sizeof(data));
  delay(10);
}
