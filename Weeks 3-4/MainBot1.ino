/*State Codes
 * 0 - Forwards
 * 1 - Right
 * 2 - Left
 * 3 - Backwards
 * 4 - Inertial 0x04
 * 5 - Park 0x02
 * 6 - Line Following 0x03
 * 7 - Empty 
 * data intake format {State, Lspeed, Rspeed}
i*/

 
 
#include <H61AEE_S01.h>
#include <Wire.h>
Vehicle car;

#define I2CADDR 0x05
int Lnum, Rnum, State;


void setup() {
  Serial.begin(9600);
  car.setupVehicle();
  car.enableMotors(true); //Gets the vehicle setup and enables the motors
  Wire.begin(I2CADDR); //begins an I2C communication on the adress 0x05 
  Wire.onReceive(receiveEvent); //when data is sent to the bot, code will execute

}

void loop(){Serial.println(".");} //will show  a period when not in the receive event to show time 

void receiveEvent()
{
  State = Wire.read(); 
  Lnum = Wire.read(); //Take in the three numbers
  Rnum = Wire.read();
  Serial.print(State);
  Serial.print("\t");
  Serial.print(Lnum); //print them bois out
  Serial.print("\t");
  Serial.println(Rnum);
  driver(); //switch over to the drive function
}

void driver()
{
  int Ldir,Rdir;
  switch (State) //The switch case will choose the drive direcion inversions based on the state 
   {
    case 0:
        Ldir = forwards;
        Rdir = forwards;
        break;
    case 1:
        Ldir = forwards;
        Rdir = backwards;
        break;
    case 2:
        Ldir = backwards;
        Rdir = forwards;
        break;
    case 3:
        Ldir = backwards;
        Rdir = backwards;
        break;
  }
    car.setDirection(LEFT, Ldir);
    car.setDirection(RIGHT, Rdir); //get the bot moving 
    car.setSpeed(LEFT, Lnum);
    car.setSpeed(RIGHT, Rnum);
  }
}
