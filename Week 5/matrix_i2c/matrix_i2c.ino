#include <MaxMatrix.h>
#include <Wire.h>
#include <NewPing.h>
#include <math.h>

#define trigger 5
#define echo 6
#define DIN 2
#define CS 3
#define CLK 4
#define maxdist 50

MaxMatrix m(DIN, CS, CLK, 1);
NewPing sonar(trigger, echo, maxdist);

char num[10][5] = {{3, 8, B01111100, B01000100, B01111100},
                {3, 8, B00000000, B00000000, B01111100},
                {3, 8, B01110100, B01010100, B01011100},
                {3, 8, B01010100, B01010100, B01111100},
                {3, 8, B00011100, B00010000, B01111100},
                {3, 8, B01011100, B01010100, B01110100},
                {3, 8, B01111100, B01010100, B01110100},
                {3, 8, B00000100, B00000100, B01111100},
                {3, 8, B01111100, B01010100, B01111100},
                {3, 8, B01011100, B01010100, B01111100}};
                
char sad[] = {8, 8, B00000000, B01100110, B00010000, B00010000, B00010000, B00010000, B01100110, B00000000};

void setup() {
  // put your setup code here, to run once:
  m.init();
  m.setIntensity(8);
  pinMode(echo, INPUT);
  pinMode(trigger, OUTPUT);
  Serial.begin(9600);
  Wire.begin(0x68);
  Wire.onRequest(requestEvent);
}

uint8_t result;

void loop() {
  // put your main code here, to run repeatedly:
  result = findMedianReading();
  m.clear();
  Serial.println(result);
  if (result == 0) {
    m.writeSprite(0, 0, sad);
  }
  else {
    m.writeSprite(0, 0, num[(int)floor((float)result/10)]);
    m.writeSprite(5, 0, num[result%10]);
  }
  delay(100);
}

void requestEvent() {
  Serial.println(result);
  Wire.write(result);
}

int findMedianReading() {
  int arrayOfValues[3];
  for (int i = 0; i < 3; i++) {
    arrayOfValues[i] = sonar.ping_cm();
  }
  int flag = 1;
  int bufferVal;
  while (flag == 1) {
    flag = 0;
    if arrayOfValues[0] > arrayOfValues[1] {
      flag = 1;
      bufferVal = arrayOfValues[1];
      arrayOfValues[1] = arrayOfValues[0];
      arrayOfValues[0] = bufferVal;
    }
    if arrayOfValues[1] > arrayOfValues[2] {
      flag = 1;
      bufferVal = arrayOfValues[2];
      arrayOfValues[2] = arrayOfValues[1];
      arrayOfValues[1] = bufferVal;
    }
  }
  return arrayOfValues[1];
}
