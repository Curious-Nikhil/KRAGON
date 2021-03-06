//@Nikhil Mishra
//BaseStation Radio Program

#include <RF24Network.h>
#include <RF24.h>
#include <SPI.h>


RF24 radio(7, 8); //CE, CSN

RF24Network network(radio);

const int this_node = 00; //BaseStation
const int SatNode = 01;    //BalloonSat
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

int i = 1;

basecommands BaseCommands; //stupid naming schemes!

void setup() {

    Serial.begin(9600);

    SPI.begin();
    radio.begin();
    network.begin(90, this_node);

}

void loop() {

    network.update();

    //====Recieving====//
    while(network.available()) {
        RF24NetworkHeader header;
       network.read(header, &IncomingData00, sizeof(IncomingData00));
    }


    Serial.print("Time: ");
    Serial.print(IncomingData00.ms);
    Serial.print("\t MQ: ");
    Serial.println(IncomingData00.mq135);


    BaseCommands.abort = 1;
    BaseCommands.mode = i++;

    //====SendingtoSat====//
    RF24NetworkHeader header2(SatNode);
    bool ok = network.write(header2, &BaseCommands, sizeof(BaseCommands));
    Serial.println(ok);

    //====SendingtoLander====//
    RF24NetworkHeader header3(LanderNode);
    bool ok2 = network.write(header3, &BaseCommands, sizeof(BaseCommands));
    Serial.println(ok2);
    
    //====SendingtoRover====//

}