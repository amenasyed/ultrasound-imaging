/*
HC-SR04 Ping distance sensor]
VCC to arduino 5v GND to arduino GND
Echo to Arduino pin 13 Trig to Arduino pin 12
Red POS to Arduino pin 11
Green POS to Arduino pin 10
560 ohm resistor to both LED NEG and GRD power rail
More info at: http://goo.gl/kJ8Gl
Original code improvements to the Ping sketch sourced from Trollmaker.com
Some code and wiring inspired by http://en.wikiversity.org/wiki/User:Dstaub/robotcar
*/
#include <TinyStepper_28BYJ_48.h>

#define trigPin 13
#define echoPin 12
#define led 11
#define led2 10
#define outsideRange 1000
const int xMOTOR_IN1_PIN = 2;
const int xMOTOR_IN2_PIN = 3;
const int xMOTOR_IN3_PIN = 4;
const int xMOTOR_IN4_PIN = 5;
const int yMOTOR_IN1_PIN = 6;
const int yMOTOR_IN2_PIN = 7;
const int yMOTOR_IN3_PIN = 8;
const int yMOTOR_IN4_PIN = 9;
const int STEPS_PER_REVOLUTION = 2048;

TinyStepper_28BYJ_48 xStepper;
TinyStepper_28BYJ_48 yStepper;

float xMax = 512; // In steps
float yMaxHeight = 40; // In same units as r
float r = 2; // In same units as yMaxHeight
float yMax = (yMaxHeight*2048) / (2*3.14159*r); // y(vert) = 2*pi*r*(steps/2048) // In steps

float xRes = 17;
float yRes = 17;

// CCW is positive
float xRotateStepNum = (xMax*2)/(xRes - 1);
float yRotateStepNum = yMax/(yRes - 1);


void setup() {
  Serial.begin(9600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(led, OUTPUT);
  pinMode(led2, OUTPUT);

  // connect and configure the stepper motor to its IO pins
  xStepper.connectToPins(xMOTOR_IN1_PIN, xMOTOR_IN2_PIN, xMOTOR_IN3_PIN, xMOTOR_IN4_PIN);
  yStepper.connectToPins(yMOTOR_IN1_PIN, yMOTOR_IN2_PIN, yMOTOR_IN3_PIN, yMOTOR_IN4_PIN);

  xStepper.setSpeedInStepsPerSecond(512);
  xStepper.setAccelerationInStepsPerSecondPerSecond(512);

  yStepper.setSpeedInStepsPerSecond(512);
  yStepper.setAccelerationInStepsPerSecondPerSecond(512);
}

void loop() {
  // float dist = senseDistance();
  // Serial.println(dist);
  
  delay(2000);
  float xCurrAngle = xMax;
  float yCurrAngle = 0;

  for( int m = 0; m < yRes; m++ )
  {
    yCurrAngle = m*yRotateStepNum;
    Serial.print("Y: ");
    Serial.println(m);
    yStepper.moveToPositionInSteps(yCurrAngle);
    delay(100);
    yStepper.disableMotor();

    for( int n = 0; n < xRes; n++ )  
    {     
      Serial.print("X: ");
      Serial.println(n);
      xStepper.moveToPositionInSteps(-xCurrAngle);

      float dist1 = senseDistance();
      float dist2 = senseDistance();
      float dist3 = senseDistance();

      float dist = (dist1 + dist2 + dist3)/3;

      // float theta = (angle/1024)*3.14159;

      // dist = dist*cos(theta);

      Serial.print("Z: ");
      if (dist >= 1000 || dist <= 0){
        Serial.println("Out of range.");
      }
      else {
        Serial.println(dist);
      }

      xCurrAngle = xCurrAngle - xRotateStepNum;
    }
    xStepper.moveToPositionInSteps(xMax);
    xStepper.disableMotor();
    xCurrAngle = xMax;
    // xRotateStepNum = - xRotateStepNum;
    // xCurrAngle = xCurrAngle - xRotateStepNum;
  }
  delay(500);
  xStepper.moveToPositionInSteps(0);
  delay(500);
  xStepper.disableMotor();
  delay(5000);
  yStepper.moveToPositionInSteps(0);
  yStepper.disableMotor();
  delay(1000000000);
}

float senseDistance()
{
  float duration, distance;

  digitalWrite(trigPin, LOW);
  delayMicroseconds(2); 
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  distance = duration * (0.343/2);

  delay(10);

  return distance;
}

void ledExtra()
{
  long distance;
  // distance = (duration/2) / 29.1;
  if (distance < 4) {  // This is where the LED On/Off happens
    digitalWrite(led,HIGH); // When the Red condition is met, the Green LED should turn off
    digitalWrite(led2,LOW);
  }
  else {
    digitalWrite(led,LOW);
    digitalWrite(led2,HIGH);
  }
  delay(500);
}