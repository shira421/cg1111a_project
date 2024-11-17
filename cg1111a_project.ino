#define TIMEOUT 1900        // max microseconds to wait (US)
#define SPEED_OF_SOUND 340  // speed of sound in m/s

#define BASE 255 // base speed
#define TURN 180 // turn speed

/*
Logic Table
Pin 2 - A, Pin 3 - B
IR Emitter Y0 (A LOW B LOW)
Blue LED Y1 (A HIGH B LOW)
Green LED Y2 (A LOW B HIGH) 
Red LED Y3 (A HIGH B HIGH)
*/

#define ULTRASONIC 10  // assigning Ultrasonic to port 2
#define IR A0 // IR Receiver OUT
#define LDR A1 // LDR OUT
#define A A2 // Logic Pin A
#define B A3 // Logic Pin B

// musical notes (frequencies in Hz)
#define B4  494
#define C4  277
#define D4  294
#define E4  330
#define F4  370
#define A4  440
#define B3  247

#include "MeMCore.h"

MeLineFollower lineFinder(PORT_1); //assigning lineFinder to port 1
MeDCMotor leftMotor(M1);   // assigning leftMotor to port M1
MeDCMotor rightMotor(M2);  // assigning rightMotor to port M2
MeBuzzer buzzer; //buzzer

void playThickOfItMelody() {
  // 1
  buzzer.tone(B4, 400); delay(100); 
  buzzer.tone(B4, 400); delay(100);
  buzzer.tone(B4, 400); delay(100);
  // 2
  buzzer.tone(A4, 400); delay(100);
  buzzer.tone(F4, 400); delay(100);
  buzzer.tone(F4, 800); delay(100);
  buzzer.tone(E4, 400); delay(100);
  buzzer.tone(E4, 400); delay(100);
  buzzer.tone(D4, 400); delay(100);
  buzzer.tone(E4, 400); delay(100);
  // 3
  buzzer.tone(F4, 800); delay(400);
  buzzer.tone(F4, 400); delay(100);
  buzzer.tone(F4, 400); delay(100);
  buzzer.tone(F4, 400); delay(100);
  buzzer.tone(D4, 400); delay(100);
  buzzer.tone(E4, 400); delay(100);
  // 4
  buzzer.tone(F4, 600); delay(100);
  buzzer.tone(D4, 600); delay(100);
  buzzer.tone(E4, 400); delay(100);
  buzzer.tone(F4, 600); delay(100);
  buzzer.tone(D4, 600); delay(100);
  buzzer.tone(E4, 400); delay(100);
  // 5
  buzzer.tone(F4, 800); delay(800);
  buzzer.tone(F4, 400); delay(100);
  buzzer.tone(F4, 400); delay(100);
  buzzer.tone(D4, 400); delay(100);
  buzzer.tone(E4, 400); delay(100);
  // 6
  buzzer.tone(F4, 600); delay(100);
  buzzer.tone(D4, 600); delay(100);
  buzzer.tone(E4, 400); delay(100);
  buzzer.tone(F4, 600); delay(100);
  buzzer.tone(D4, 600); delay(100);
  buzzer.tone(E4, 400); delay(100);
  // 7
  buzzer.tone(F4, 800); delay(800);
  buzzer.tone(F4, 400); delay(100);
  buzzer.tone(F4, 400); delay(100);
  buzzer.tone(D4, 400); delay(100);
  buzzer.tone(E4, 400); delay(100);
  // 8
  buzzer.tone(F4, 400); delay(100);
  buzzer.tone(F4, 400); delay(100);
  buzzer.tone(D4, 400); delay(100);
  buzzer.tone(E4, 400); delay(100);
  buzzer.tone(F4, 600); delay(100);
  buzzer.tone(D4, 600); delay(100);
  buzzer.tone(C4, 400); delay(100);
  buzzer.tone(B3, 400); delay(100);
  buzzer.noTone();
}

// button status
int status = 0;

// variables for PID
double past_error = 0;
double KPir = 29.0;
double KDir = 17.0;
double KPus = 22.0;
double KDus = 11.0;

double detectedArray[3] = { 0, 0, 0 }; // temp color array for readings
double whiteArray[3] = { 919.0, 878.0, 858.0 }; // calibrated white array
double blackArray[3] = { 874.0, 655.0, 659.0 }; // calibrated black array
double greyDiff[3] = { 45.0, 223.0, 199.0 }; // white - black array

// for Euclidean Distance
double distance[6] = { 0, 0, 0, 0, 0, 0 };
double logic[6] = { 0, 1, 2, 3, 4, 5 };

// pink, red, orange, blue, green, white checkpoints (ordered)
double colorArray[6][3] = {{ 260.0, 224.0, 221.0 }, { 230.0, 112.0, 98.0 }, //pink 272 236 230
                          { 238.0, 165.0, 110.0 }, { 90.0, 197.0, 219.0 }, 
                          { 107.0, 209.0, 169.0 }, { 235.0, 256.0, 253.0 }};

// basic movement logic
void forward(uint8_t speed, int time) {
  leftMotor.run(-speed);
  rightMotor.run(speed);
  delay(time);
}

void reverse(uint8_t speed, int time) {
  leftMotor.run(speed);
  rightMotor.run(-speed);
  delay(time);
}

void stop() {
  leftMotor.stop();
  rightMotor.stop();
}

void sqRight(uint8_t speed, int time) {
  leftMotor.run(-speed);
  rightMotor.run(-speed);
  delay(time);
}

void sqLeft(uint8_t speed, int time) {
  leftMotor.run(speed);
  rightMotor.run(speed);
  delay(time);
}

void nudge(uint8_t speed, double c) {
  leftMotor.run(-speed - c);
  rightMotor.run(speed - c);
}

// turn on component (by logic table)
void turnOnIR() {
  digitalWrite(A, LOW);
  digitalWrite(B, LOW);
}

void turnOnRed() {
  digitalWrite(A, HIGH);
  digitalWrite(B, HIGH);
}

void turnOnGreen() {
  digitalWrite(A, LOW);
  digitalWrite(B, HIGH);
}

void turnOnBlue() {
  digitalWrite(A, HIGH);
  digitalWrite(B, LOW);
}

// average reading from LDR
int LDRreading(int times) {
  int reading;
  int total = 0;
  for (int i = 0; i < times; i++) {
    reading = analogRead(LDR);
    total = reading + total;
    delay(10);
  }
  return total / times;
}

// calibrate color sensor
void calibrate() {
  // white
  Serial.println("Put White Sample For Calibration ...");
  delay(5000);

  turnOnRed();
  delay(200);
  whiteArray[0] = LDRreading(10);

  turnOnGreen();
  delay(200);
  whiteArray[1] = LDRreading(10);

  turnOnBlue();
  delay(200);
  whiteArray[2] = LDRreading(10);

  // black
  Serial.println("Put Black Sample For Calibration ...");
  delay(5000);

  turnOnRed();
  delay(200);
  blackArray[0] = LDRreading(10);

  turnOnGreen();
  delay(200);
  blackArray[1] = LDRreading(10);

  turnOnBlue();
  delay(200);
  blackArray[2] = LDRreading(10);

  greyDiff[0] = whiteArray[0] - blackArray[0];
  greyDiff[1] = whiteArray[1] - blackArray[1];
  greyDiff[2] = whiteArray[2] - blackArray[2];
  for (int i = 0; i < 3; i++) {
    Serial.print(whiteArray[i]);
    Serial.print(blackArray[i]);
    Serial.print(greyDiff[i]);
    Serial.println();
  }
}

// get distance from US
double USdist() {
  pinMode(ULTRASONIC, OUTPUT);
  digitalWrite(ULTRASONIC, LOW);
  delayMicroseconds(2);
  digitalWrite(ULTRASONIC, HIGH);
  delayMicroseconds(10);
  digitalWrite(ULTRASONIC, LOW);

  pinMode(ULTRASONIC, INPUT);
  double duration = pulseIn(ULTRASONIC, HIGH, TIMEOUT);
  double USdistance = ((duration/2.0)/1000000) * SPEED_OF_SOUND * 100;
  return USdistance;
}

// get distance from IR
double IRdist() {
  turnOnBlue(); // reading from ambient ir
  double x = analogRead(IR);
  turnOnIR(); // reading from reflected ir pulse (with ambient ir)
  double y = analogRead(IR);
  /* 
  x = reading for ambient ir, y = reading for reflected ir pulse (with ambient ir)
  x-y = voltage drop due to ir pulse, this will filter out the ambient ir
  scale from experimentation is 60 to 510 for readings from 1.5cm to 9.0 cm
  values are normalised to reflect distance
  */
  double scaled = (((x-y)-60.0)/450.0) * 7.5 + 1.5;
  return scaled;
}

// get reading from color sensor
void detectColor() {
  // Serial.println("put sample");
  // delay(3000);

  turnOnRed();
  delay(200);
  detectedArray[0] = LDRreading(5);
  detectedArray[0] = (detectedArray[0] - blackArray[0])/(greyDiff[0])*255;

  turnOnGreen();
  delay(200);
  detectedArray[1] = LDRreading(5);
  detectedArray[1] = (detectedArray[1] - blackArray[1])/(greyDiff[1])*255;

  turnOnBlue();
  delay(200);
  detectedArray[2] = LDRreading(5);
  detectedArray[2] = (detectedArray[2] - blackArray[2])/(greyDiff[2])*255;

  // Serial.print(detectedArray[0]);
  // Serial.print(" ");
  // Serial.print(detectedArray[1]);
  // Serial.print(" ");
  // Serial.println(detectedArray[2]);
}

/* 
Euclidean Distance
square the difference in R, G and B values between
detected color and checkpoint color individually
add them up and sqrt that to get distance magnitude
*/

void findDistance(){
  for (int i = 0; i < 6; i++){
    double sum = 0;
    for (int j = 0; j < 3; j++){
      double x = detectedArray[j] - colorArray[i][j];
      sum = sum + x*x;
    }
    distance[i] = sqrt(sum);
  }
}

// get minimum distance from array to find nearest color
int findClosest(){
  int min = 0;
  for (int i = 0; i < 6; i++){
    if (distance[i] < distance[min]){
      min = i;
    }
  }
  return min;
}

void setup() {
  Serial.begin(9600);
  pinMode(IR, INPUT);
  pinMode(LDR, INPUT);
  pinMode(A, OUTPUT);
  pinMode(B, OUTPUT);
  pinMode(A7, INPUT);
  // calibrate();
}

void loop() {
  // detectColor();
  if (analogRead(A7) < 100) { // if push button is pushed, the value will be very low
    status = 1 - status;
    delay(500); // delay 500ms so that a button push won't be counted multiple times.
  }
  if (status == 0) {
    return;
  }

  // logic for checkpoint
  int sensorState = lineFinder.readSensors();
  if (sensorState == S1_IN_S2_IN) {
    stop();
    detectColor();
    findDistance();
    int s = findClosest();
    switch(s) {
      case 0:
      sqLeft(TURN, 420);
      forward(TURN, 1030);
      sqLeft(TURN, 420);
      break;
      case 1: // red
      sqLeft(TURN, 420);
      break;
      case 2: // orange
      sqLeft(TURN, 870);
      break;
      case 3: // blue
      sqRight(TURN, 420);
      forward(TURN, 1030);
      sqRight(TURN, 420);
      break;
      case 4: // green
      sqRight(TURN, 420);
      break;
      case 5: // white
      playThickOfItMelody();
      break;
    }
  }
  // get distances from US and IR
  double USr = USdist();
  double IRr = IRdist();
  double error = 0;
  double correction = 0;
  Serial.print(USr);
  Serial.print(' ');
  Serial.println(IRr);

  // if US out of range, set back US reading to stable point
  if (USr == 0) {
    USr = 11.0;
  }

  if (IRr < 4.0) { // use IR if the reading is below 4.0 cm (IR readings don't scale well past this value)
    error = 9.0 - IR; // stable point from IR pov = 9.0cm
    correction = error * KPir + ((error-past_error)/TIMEOUT) * KDir; // PID controller for IR
  }
  else {
    error = (USr - 11.0); // stable point from US pov = 11.0cm
    correction = error * KPus + ((error-past_error)/TIMEOUT) * KDus; // PID controller for US
  }
  nudge(BASE, correction); // movement with correction
  past_error = error; // for D
}
