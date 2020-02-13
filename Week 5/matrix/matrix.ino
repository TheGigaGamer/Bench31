#include <MaxMatrix.h>

#define trigger 5
#define echo 6
#define DIN 2
#define CS 3
#define CLK 4
#define maxdist 50

MaxMatrix m(DIN, CS, CLK, 1);
NewPing sonar(trigger, echo, maxdist);

char num[11][7] = {{5, 8, B01111100, B10100010, B10010010, B10001010, B01111100},
                {5, 8, B00000000, B10000100, B11111110, B10000000, B00000000},
                {5, 8, B11000100, B10100010, B10010010, B10010010, B10001100},
                {5, 8, B01000010, B10000010, B10010010, B10011010, B01100110},
                {5, 8, B00110000, B00101000, B00100100, B11111110, B00100000},
                {5, 8, B01001110, B10001010, B10001010, B10001010, B01110010},
                {5, 8, B01111000, B10010100, B10010010, B10010010, B01100010},
                {5, 8, B00000010, B11100010, B00010010, B00001010, B00000110},
                {5, 8, B01101100, B10010010, B10010010, B10010010, B01101100},
                {5, 8, B10001100, B10010010, B10010010, B01010010, B00111100},
                {5, 8, B01100100, B10000000, B10000000, B10000000, B01100100}};
                
void setup() {
  // put your setup code here, to run once:
  m.init();
  m.setIntensity(8);
  pinMode(echo, INPUT);
  pinMode(trigger, OUTPUT);
  Wire.begin(0x68);
  Wire.onRequest(requestEvent);
}

void loop() {
  // put your main code here, to run repeatedly:
  m.clear();
  delay(100);
  for (int i = 0; i < 11; i++) {
    m.writeSprite(8,0,num[i]);
    for (int j = 0; j < 7; j++) {
      m.shiftLeft(false, false);
      delay(50);
    }
    delay(500);
  }
  delay(1000);
}
