#include <Wire.h>
#include <NewPing.h>

#define echo 2
#define trigger 3
#define maxdist 50

void setup() {
  // put your setup code here, to run once:
    pinMode(echo, INPUT);
    pinMode(trigger, OUTPUT);
    Wire.begin();
    Serial.begin(9600);    
}

NewPing sonar(trigger, echo, maxdist);

void loop() {
  // put your main code here, to run repeatedly:
  int result = sonar.ping_cm();
  Serial.println(sonar.ping_cm());
  delay(100);
}
