//Matthew Cossins
//Slave Program on the Optical Line Following Subsystem
//For use with the Minibot
//Motor control program for use with weighted average/PID code

#include <Wire.h>
//Enables I2C functionality

// name the motor control pins
#define PWMa 7
#define PWMb 5
#define PWMc 8
#define PWMd 6

#define SlaveAddress 0x01
//The address of this slave Arduino is defined

#define init_right_motor 100
#define init_left_motor 100
#define MAX 255
//Defines the initial speeds and the max speed of each motor

void setup() {
  // put your setup code here, to run once:
  pinMode(PWMa, OUTPUT);
  pinMode(PWMb, OUTPUT);
  pinMode(PWMc, OUTPUT);
  pinMode(PWMd, OUTPUT);
  //Sets the PWM pins to output mode
  Wire.begin(SlaveAddress);
  //Joins the I2C bus
  Serial.begin(9600);
  delay(500); 
  //Short delay so that the vehicle doesn't move immediately
  Wire.onReceive(receiveEvent);
  //When data is received, the receiveEvent function occurs
}
float c;
void loop() {
  // put your main code here, to run repeatedly
  // The loop function is not required for this program
}

void receieveEvent()
{
  while (Wire.available()) { //loop whilst slave sends data
  //i.e. whilst bus is busy
      c = Wire.read(); //receive data byte by byte
  }
  float right_motor = init_right_motor + c;
  float left_motor = init_left_motor - c;
  
  if (right_motor > MAX ) 
    right_motor = MAX; // prevent the motor from going beyond max speed
  if (left_motor > MAX ) 
    left_motor = MAX; // prevent the motor from going beyond max speed
  if (right_motor < 0) 
    right_motor = 0; // keep the motor speed positive
  if (left_motor < 0) 
    left_motor = 0; // keep the motor speed positive
  
  digitalWrite(PWMa, right_motor);
  digitalWrite(PWMb, LOW);
  digitalWrite(PWMc, left_motor);
  digitalWrite(PWMd, LOW);

}
  
