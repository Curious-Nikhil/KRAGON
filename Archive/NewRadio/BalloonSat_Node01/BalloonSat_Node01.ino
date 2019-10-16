//@Nikhil Mishra
//BalloonSat Radio Program

#include <RF24Network.h>
#include <RF24.h>
#include <SPI.h>


RF24 radio(7, 8); //CE, CSN

RF24Network network(radio);

const int this_node = 01; //SatNode
const int MasterNode = 00;    //BaseStation
const int LanderNode = 011;     //Lander
const int RoverNode = 0111;    //Rover

//datapayload

struct incomingData {
    unsigned long ms;
    float alt;
    float mq135;
    float ax;
    float ay;
    float az;

    bool launch;
    bool release;
    bool green;
    bool red;
}; 

incomingData IncomingData00;


struct basecommands {
    int mode;
    bool release;
    bool abort;
    bool reboot;
};

basecommands BaseCommands; //stupid naming schemes!

void setup() {

    Serial.begin(9600);

    SPI.begin();
    radio.begin();
    network.begin(90, this_node);

    pinMode(A0, INPUT);

}

void loop() {

    network.update();

    //====Recieving====//
    Serial.println(network.available());
    while(network.available()) {
        RF24NetworkHeader header;
       network.read(header, &BaseCommands, sizeof(BaseCommands));
    }
    
    Serial.print("Mode: ");
    Serial.print(BaseCommands.mode);
    Serial.print("\t Abort: ");
    Serial.println(BaseCommands.abort);

    IncomingData00.ms = millis();

    IncomingData00.mq135 = analogRead(A0);

    //====SendingtoBaseStation====//
    RF24NetworkHeader header2(MasterNode);
    bool ok = network.write(header2, &IncomingData00, sizeof(IncomingData00));


}