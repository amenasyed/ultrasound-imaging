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
const int MOTOR_IN1_PIN = 2;
const int MOTOR_IN2_PIN = 3;
const int MOTOR_IN3_PIN = 4;
const int MOTOR_IN4_PIN = 5;
const int STEPS_PER_REVOLUTION = 2048;
TinyStepper_28BYJ_48 stepper;
float angle = -256;


void setup() {
  Serial.begin(9600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(led, OUTPUT);
  pinMode(led2, OUTPUT);

  // connect and configure the stepper motor to its IO pins
  stepper.connectToPins(MOTOR_IN1_PIN, MOTOR_IN2_PIN, MOTOR_IN3_PIN, MOTOR_IN4_PIN);

  stepper.setSpeedInStepsPerSecond(100);
  stepper.setAccelerationInStepsPerSecondPerSecond(500);
}

void loop() {
  float dist = senseDistance();
  Serial.println(dist);
  
  // delay(2000);
  // for( int n = 0; n <= 8; n++ )  
  // {
  //   stepper.moveToPositionInSteps(angle);
  //   Serial.print("Angle: ");
  //   Serial.println(stepper.getCurrentPositionInSteps());

  //   float dist1 = senseDistance();
  //   float dist2 = senseDistance();
  //   float dist3 = senseDistance();

  //   float dist = (dist1 + dist2 + dist3)/3;

  //   float theta = (angle/1024)*3.14159;

  //   dist = dist*cos(theta);

  //   Serial.print("Distance: ");
  //   if (dist >= 1000 || dist <= 0){
  //     Serial.println(outsideRange);
  //   }
  //   else {
  //     Serial.println(dist);
  //   }

  //   delay(100);

  //   angle = angle + 64;
  // }

  // stepper.moveToPositionInSteps(0);
  // Serial.println(stepper.getCurrentPositionInSteps());
  // delay(3000);

  // stepper.disableMotor();
  // delay(100000000);
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