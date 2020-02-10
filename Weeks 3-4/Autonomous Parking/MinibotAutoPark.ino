#include <NewPing.h>
#include <math.h>
#include <Wire.h>

#define dataPin1 2
#define latchPin1 3
#define clockPin1 4
#define dataPin2 5
#define latchPin2 6
#define clockPin2 7
#define sideTrigger 8
#define sideEcho 9
#define backTrigger 10
#define backEcho 11
#define frontTrigger 12
#define frontEcho 13
#define maxdist 50 // define pins for everything needed
//
//int ReadSideSensor(void);
//void WriteToWire(char command, byte b, byte c);
//int ReadSideSensorUnfiltered(void);
//void WriteToDisplay(int displaynum, byte data);
//void TurnLeft(float deg);
//void TurnRight(float deg);
//int ReadBackSensor(void);
//int BubbleSortReadings(int goodReadings[]);

const byte number[10] = {
  B00000011,
  B10011111,
  B00100101,
  B00001101,
  B10011001,
  B01001001,
  B01000001,
  B00011111,
  B00000001,
  B00001001
};  // serial data to be sent to the 7 seg displays (digits)

const byte warning[6] = {
  B01111111,
  B10111111,
  B11011111,
  B11101111,
  B11110111,
  B11111011
}; // 'frames' for warning animation

void setup() {
  // initialise pins
  pinMode(clockPin1, OUTPUT);
  pinMode(latchPin1, OUTPUT);
  pinMode(dataPin1, OUTPUT);
  pinMode(clockPin2, OUTPUT);
  pinMode(latchPin2, OUTPUT);
  pinMode(dataPin2, OUTPUT);
  pinMode(sideTrigger, OUTPUT);
  pinMode(sideEcho, INPUT);
  pinMode(backTrigger, OUTPUT);
  pinMode(backEcho, INPUT);
  pinMode(frontTrigger, OUTPUT);
  pinMode(frontEcho, INPUT);
  Wire.begin();
  Serial.begin(9600);
}

NewPing sonar0(sideTrigger, sideEcho, maxdist);
NewPing sonar1(backTrigger, backEcho, maxdist);
NewPing sonar2(frontTrigger, frontEcho, maxdist);
// Initialise 3 sonar sensors

void loop() {
  int distance = floor(ReadSideSensor()); // get an initial reading for distance to obstacle
  int tens_digit = floor(distance/10);
  float front_dist;
  float back_dist;
  front_dist = sonar2.ping_cm();
  back_dist = sonar1.ping_cm(); // get distances in front of and behind car
  static int last_distance = distance;
  WriteToWire(0, 12, 12); // move forward slowly
  Serial.println("Forwards!");
  while (distance - last_distance <= 10) { // repeat until moving past the edge of an object
    last_distance = distance;
    distance = ReadSideSensorUnfiltered(); // read the sensor
    WriteToDisplay(1, number[(int)floor(distance/10)]);
    WriteToDisplay(2, number[distance % 10]); // display the distance read on the 7 segments
    delay(100);
  }
  int starttime = millis();
  while (last_distance - distance <= 10) { // repeat until another obstacle is found
    last_distance = distance;
    distance = ReadSideSensorUnfiltered(); // read the sensor
    WriteToDisplay(1, number[(int)floor(distance/10)]);
    WriteToDisplay(2, number[distance % 10]); // display distance read on 7 segments
    delay(100);
  }
  int endtime = millis();
  while (endtime - starttime < 500) { // check that the gap is big enough, based on time passed
    WriteToWire(0, 0, 0); // if it isn't big enough, stop the car
    WriteToDisplay(1, B11010101); // and display 'no' on the 7 segments
    WriteToDisplay(2, B11000101);
    while (1) { // and indefinitely stop the code (testing purposes)
      delay(10);
    }
  }
  int sensorMin = 8;
  int angle = 43;
  WriteToWire(3, 10, 10); // reverse the car slowly 
  delay(500);
  TurnRight(angle); // turn the car to allow reversing into the spot
  WriteToWire(3, 7, 7); // reverse even more slowly
  distance = ReadSideSensor();
  back_dist = ReadBackSensor(); // read the back sensor
  while (back_dist > sensorMin+5) {
    Serial.print("Back: ");
    Serial.println(back_dist);
    delay(100);
    back_dist = ReadBackSensor(); // keep reading the back sensor until it gets below a set value
  }
  TurnLeft(angle); // turn left
  WriteToWire(0, 7, 7); // move forward very slowly
  while (abs(front_dist - back_dist) > 7) { // until front and back distance are approximately equal
    back_dist = sonar1.ping_cm();
    front_dist = sonar2.ping_cm(); // keep getting front and back distance
    Serial.print("Back: ");
    Serial.println(back_dist);
    Serial.print("Front: ");
    Serial.println(front_dist);
    delay(50);
  }
  float score = (back_dist + front_dist)/2; // score displayed based on how tight the space was
  WriteToDisplay(1, number[(int)floor(score/10)]);
  WriteToDisplay(2, number[(int)score % 10]);
  WriteToWire(0, 0, 0); // stop the car
  while(1) { // delay indefinitely (program end)
    delay(10);
  }
  last_distance = distance;
}

void WriteToWire(byte command, byte b, byte c) {
  Wire.beginTransmission(0x05); // begin transmission to bot
  Wire.write(command);
  Wire.write(b);
  Wire.write(c); // write all necessary info
  Wire.endTransmission(); // finish transmission
}

void TurnRight(float deg) {
  WriteToWire(1, 60, 60); // turn right quickly - overcomes friction
  delay(30*deg/9); // wait then stop
  WriteToWire(1, 0, 0);
}

void TurnLeft(float deg) {
  WriteToWire(2, 55, 55); // turn left quickly
  delay(30*deg/9); // wait then stop
  WriteToWire(2, 0, 0);
}

int ReadSideSensor(void) {
  int reading = 0;
  int goodReading[3];
  for (int i = 0; i < 3; i++) {  
    while (reading == 0) { // take 3 non-zero readings from the side sensor
      reading = sonar0.ping_cm();
      Serial.print("Side: ");
      Serial.println(reading);
      delay(50);
    }
    goodReading[i] = reading;
    reading = 0;
  }
  return BubbleSortReadings(goodReading); // return the median of these 3 values
}

int ReadBackSensorBasic(void) {
  int reading = 0;
  while (reading == 0) { // take one non-zero reading from sensor
    reading = sonar1.ping_cm();
    Serial.print("Side: ");
    Serial.println(reading);
    delay(70);
  }
  return reading;
}

int ReadBackSensor(void) {
  int reading = 0;
  int goodReading[3];
  for (int i = 0; i < 3; i++) {  
    while (reading == 0) { // take 3 non-zero readings from back sensor
      reading = sonar1.ping_cm();
      Serial.print("Back: ");
      Serial.println(reading);
      delay(50);
    }
    goodReading[i] = reading;
    reading = 0;
  }
  return BubbleSortReadings(goodReading); // return the median of the 3 readings
}

int ReadSideSensorUnfiltered(void) {
  int reading = sonar0.ping_cm();
  int goodReading[3];
  for (int i = 0; i < 3; i++) { // take 3 readings, give 51 if reading is 0
    if (reading == 0) {
      reading = 51;
    }
    goodReading[i] = reading;
  }
  return BubbleSortReadings(goodReading); // return median of 3 values
}

void WriteToDisplay(int displaynum, byte data) {
  if (displaynum == 1) { // shift out data to first display
    digitalWrite(latchPin1, LOW);
    shiftOut(dataPin1, clockPin1, LSBFIRST, data);
    digitalWrite(latchPin1, HIGH);
  }
  else { // shift out data to second display
    digitalWrite(latchPin2, LOW);
    shiftOut(dataPin2, clockPin2, LSBFIRST, data);
    digitalWrite(latchPin2, HIGH);
  }
}

int BubbleSortReadings(int goodReadings[]) {
  int flag = 1; 
  while (flag == 1) {
    flag = 0;
    int temp;
    if (goodReadings[0] > goodReadings [1]) {
      temp = goodReadings[1];
      goodReadings[1] = goodReadings[0];
      goodReadings[0] = temp;
      flag = 1;
    }
    if (goodReadings[1] > goodReadings[2]) {
      temp = goodReadings[2];
      goodReadings[2] = goodReadings[1];
      goodReadings[1] = temp;
      flag = 1;
    }
  } // simple bubble sort algorithm - repeats until sorted
  return goodReadings[1]; // return the 2nd value (i.e. the median)
}
