#include <RF24.h>
//this library is to ensure proper functioning of the nRF module
#include <RF24Network.h>
//Libraries should be imported so that nRF communication is possible between the "Control" and the "Motion" units

#include <SPI.h>
//Library to allow SPI protocol between "Master" and "Slave" in both the units of the Locomotion part

#define Xpin A3 //analog pin to read the joystick value for the motor movement
#define Ypin A4 //analog pin to read the joystick value for the servo control

uint8_t joystick[2]; //variable to store the signal which is to be transmitted to the Locomotion unit

int Xval,Yval;
int dt = 500;

RF24 radio(3, 10);               // nRF24L01 (CE,CSN)
//Here CE is chip enable pin which enables the functioning of the circuit that makes the module. CSN stands for chip select not pin which is responsible for selecting the receiver in the wireless communication 
RF24Network network(radio);      // Include the radio in the network
const uint16_t this_node = 00;   // Address of this node in Octal format ( 04,031, etc)
const uint16_t node01 = 01;   //Adress of the receiver node

void setup() {
  SPI.begin();//beginning of the SPI communication 
  Serial.begin(9600);
  radio.begin(); 
  network.begin(90, this_node);  //(channel, node address)
  
  pinMode(Xpin,INPUT);
  pinMode(Ypin,INPUT);
  
}

void loop() {
  
  joystick[0] = map(analogRead(Xpin), 0, 1023, 0, 255);
  joystick[1] = map(analogRead(Ypin), 0, 1023, 0, 180);
  //reading the joystick values on both the axes and map them in necessary format and put them in an array

  Serial.println("------------------------");
  Serial.print("Motor Value : ");
  Serial.println(joystick[0]);
  Serial.print("Servo Value : ");
  Serial.println(joystick[1]);

  network.update();
  //updating the network between the two nodes so as to update new values on the network
  
  RF24NetworkHeader header(node01);     // (Address where the data is going)
  bool ok = network.write(header, &joystick, sizeof(joystick)); // Send the data
  
  delay(500);
  
}