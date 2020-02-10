//Matthew Cossins
//Slave Program on the Optical Line Following Subsystem
//For use with the Mainbot
//Weighted average and PID Code

#include <Wire.h>
//Allows for use of I2C functionality

#define R1 A7
#define R2 A0
#define R3 A3
#define L1 A6
#define L2 A1
#define L3 A2

//Defines each analogue pin as the correct LED, photodiode pair
//The sensor array is labelled as follows
//  L3 --- L2 --- L1 --- R1 --- R2 --- R3
//This is the reverse perspective to the minibot

#define SlaveAddress 0x03

//Defines 0x03 as SlaveAddress
//This is the address of the subsystem Arduino

#define BAUD 9600

//Defines 9600 as the baud rate

#define L3MAX 830
#define L2MAX 800
#define L1MAX 840
#define R3MAX 790
#define R2MAX 812
#define R1MAX 812

#define L3MIN 0
#define L2MIN 40
#define L1MIN 45
#define R3MIN 0
#define R2MIN 30
#define R1MIN 0

//Maximum and minimum readings for each sensor designed

#define P 20
#define D 400

//placeholder values for K and D multipliers defined

#define init_right_motor 100
#define init_left_motor 100
#define MAX 255
//Defines the initial speeds and the max speed of each motor


float PID(); 
//Function returns a float that represents motor speed

void transmit_data_i2c(float motor_speed); //Function transmits an integer parameter over I2C

void setup() 
{
  // put your setup code here, to run once:
  pinMode(L3, INPUT);
  pinMode(L2, INPUT);
  pinMode(L1, INPUT);
  pinMode(R3, INPUT);
  pinMode(R2, INPUT);
  pinMode(R1, INPUT);
  //Sets the mode of the analogue pins to input
  Serial.begin(9600);
  Wire.begin(SlaveAddress);
  //Joins the I2C bus
  Wire.onRequest(requestEvent);
  
}

float previous_error = 0;

void loop() {
  // put your main code here, to run repeatedly:
}

void requestEvent() {
  float motor_speed = PID();
  transmit_data_i2c(motor_speed);
}

float PID()
{
  float L1y = ((analogRead(L1)-L1MAX)*1000)/(L1MIN-L1MAX);
  float L2y = ((analogRead(L2)-L2MAX)*1000)/(L2MIN-L2MAX);
  float L3y = ((analogRead(L3)-L3MAX)*1000)/(L3MIN-L3MAX);
  float R1y = ((analogRead(R1)-R1MAX)*1000)/(R1MIN-R1MAX);
  float R2y = ((analogRead(R2)-R2MAX)*1000)/(R2MIN-R2MAX);
  float R3y = ((analogRead(R3)-R3MAX)*1000)/(R3MIN-R3MAX);

  //Mapped readings calculated

  float weighted_average_right = (R1y + 2*R2y + 3*R3y)/(R1y + R2y + R3y);
  float weighted_average_left = -(L1y + 2*L2y + 3*L3y)/(L1y + L2y + L3y);
  float proportional = weighted_average_left + weighted_average_right;

  //weighted average formula used

  float motor_speed = P*proportional + D*(proportional-previous_error);
  float previous_error = proportional;

  //PID used and motor_speed returned

  return motor_speed;
}

void transmit_data_i2c(float motor_speed) 
{
  int state;
  
  int right_motor = init_right_motor + motor_speed;
  int left_motor = init_left_motor - motor_speed;
  
  if (right_motor > MAX ) 
    right_motor = MAX; // prevent the motor from going beyond max speed
  if (left_motor > MAX ) 
    left_motor = MAX; // prevent the motor from going beyond max speed
  if (right_motor < 0) 
    right_motor = 0; // keep the motor speed positive
  if (left_motor < 0) 
    left_motor = 0; // keep the motor speed positive

  if (right_motor > left_motor)
    state = 1; //right
  else if (left_motor > right_motor)
    state = 2; //left
  else
    state = 0; //forwards
  Wire.beginTransmission(SlaveAddress);
  Wire.write(state);
  Wire.write(left_motor);
  Wire.write(right_motor);
  Wire.endTransmission();
  //Data sent over I2C to slave Arduino
}
