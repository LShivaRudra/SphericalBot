#include <RF24.h>
#include <RF24Network.h>
#include <SPI.h>
#include <Servo.h> 
//importing the servo library

#define EN12 5
#define EN34 6
//enable pins on the L298N motor driver

#define IN1 2
#define IN2 3
#define IN3 4
#define IN4 7
//input pins which hold the directional data on the motor driver

#define servoPin 9
//holds the value of the directional out

RF24 radio(8, 10);               // nRF24L01 (CE,CSN)
RF24Network network(radio);      // Include the radio in the network
const uint16_t this_node = 01;   // Address of our node in Octal format ( 04,031, etc)
Servo myservo;  // create servo object to control a servo
int servoVal;
int motorVal;

uint8_t buf[2];


void setup() {
  Serial.begin(9600);
  SPI.begin();
  radio.begin();
  network.begin(90, this_node); //(channel, node address)
  
  pinMode(EN12, OUTPUT);
  pinMode(EN34, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(servoPin, OUTPUT);
  
  myservo.attach(servoPin);   // (servo pin)
  
}

void loop() {
  network.update();
  myservo.write(servoVal);
  while ( network.available() ) {     // Is there any incoming data?
    RF24NetworkHeader header;
    int incomingData;
    network.read(header, &buf, sizeof(buf)); // Read the incoming data
    
    motorVal = buf[0];
    servoVal = buf[1];
   // storing the array values in variables
    
    Serial.println("----------------------");
    Serial.print("Motor Value : ");
    Serial.println(motorVal);
    Serial.print("Servo Value : ");
    Serial.println(servoVal);
    
    if (motorVal > 127){
      //Forward
      
      analogWrite(EN12, map(motorVal, 128, 255, 0, 255));
      analogWrite(EN34, map(motorVal, 128, 255, 0, 255));
      
      digitalWrite(IN1, HIGH);
      digitalWrite(IN2, LOW);
      digitalWrite(IN3, HIGH);
      digitalWrite(IN4, LOW);
      
    }
    else{
      //Backward
      
      analogWrite(EN12, map(motorVal, 0, 127, 255, 0));
      analogWrite(EN34, map(motorVal, 0, 127, 255, 0));
      
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, HIGH);
      digitalWrite(IN3, LOW);
      digitalWrite(IN4, HIGH);
      
    }
    
    myservo.write(servoVal);
    
  }
  //checking certain conditions and executing the necessary functions.
  
}