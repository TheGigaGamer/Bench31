//Matthew Cossins
//Master Program on the Optical Line Following Subsystem
//For use with the Minibot
//Basic if/else program to check basic functionality

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

#define MasterAddress 0x08
#define SlaveAddress 0x01

//Defines 0x08 as MasterAddress
//This is the address of the subsystem Arduino I2C
//Since the Arduino is a master, an address is optional
//However, code can be reused and addresses are helpful later
//when there are more subsystems
//Defines 0x01 as SlaveAddress
//This is the address of the minibot Arduino

#define BAUD 9600

//Defines 9600 as the baud rate

int sensor(float average_right, //Takes float parameters that are the average reading of each side
           float average_left); 
//Function returns an integer that represents an instruction

void transmit_data_i2c(int output); //Function transmits an integer parameter over I2C

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
  Wire.begin(MasterAddress);
  //Joins the I2C bus
  
}

void loop() {
  // put your main code here, to run repeatedly:
  float average_right = (analogRead(R1)+analogRead(R2)+analogRead(R3))/3;
  float average_left = (analogRead(L1)+analogRead(L2)+analogRead(L3))/3;
  int output = sensor(average_right, average_left);
  transmit_data_i2c(output);
}

int sensor(float average_right, float average_left)
{
  int output;
  if (average_right < average_left )
  {
    if ((average_right + 500) > average_left) //Gives a tolerance
      output = 0;
      //Move Forwards
    else
      output = 1;
      //Turn Right
  }
  else if (average_left < average_right)
  {
    if ((average_left + 500) > average_right) //Gives a tolerance
      output = 0;
      //Move Forwards
    else
      output = 2;
      //Turn Left
  }
  else
    output = 0;
    //Move Forwards
  return output;
}

void transmit_data_i2c(int output) 
{
  Wire.beginTransmission(SlaveAddress);
  Wire.write(output);
  Wire.endTransmission();
  //Data sent over I2C to slave Arduino
}
