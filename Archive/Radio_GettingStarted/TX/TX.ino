//https://www.hackster.io/jrance/arduino-nrf24l01-wireless-weather-station-f6c63f

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(7, 8); // CE, CSN

const byte addresses[][6] = { "00001", "00002"};

int i = 0;

struct package
{
  float a1 = 0.0;
  float alt;
  float kmf;
  bool drop = false;
};

typedef struct package Package;
Package data;


void setup() {
  Serial.begin(9600);

  radio.begin();
  radio.openWritingPipe(addresses[1]);
  radio.openReadingPipe(1, addresses[0]);
  radio.setPALevel(RF24_PA_MIN);

  pinMode(A1, INPUT);

}

void loop() {

  //SEND
  radio.stopListening();
  data.a1 = analogRead(A1);
  Serial.println(data.a1);
  radio.write(&data, sizeof(data));
  delay(5);

  
  //Recieve
  radio.startListening();

  while(!radio.available());

  radio.read(&data, sizeof(data));
  Serial.println(data.drop);
  if(data.drop == true) { 
    Serial.println("DROP! \n DROP!");
  }
}
