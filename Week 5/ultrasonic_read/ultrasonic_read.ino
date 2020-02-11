#include <Wire.h>
#include <NewPing.h>

#define echo 2
#define trigger 3
#define maxdist 50

int result;

void setup() {
  // put your setup code here, to run once:
    pinMode(echo, INPUT);
    pinMode(trigger, OUTPUT);
    Wire.begin(0x01);
    Serial.begin(9600);    
    Wire.onReceive(receiveEvent);
}

NewPing sonar(trigger, echo, maxdist);

void loop() {
  result = sonar.ping_cm();
  delay(100);
}

void receiveEvent() {
  while (Wire.available()) {
    Wire.read();
  }
  Wire.write(result);
}
