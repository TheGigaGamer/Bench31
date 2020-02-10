#include "nRF24L01.h"
#include "RF24.h"
#include "SPI.h"

const int pinButtonL1 = 6;
const int pinButtonL2 = 5;
const int pinButtonR1 = 3;
const int pinButtonR2 = 4;
const int pinClickL = 8;
const int pinClickR = 7; //declare all the pins needed for the inputs
const int pinRightX = A2;
const int pinRightY = A3;
const int pinLeftX = A0;
const int pinLeftY = A1;

RF24 radio(9, 10); // NRF24L01 used SPI pins + Pin 9 and 10 on the NANO
const byte address[6] = "01101";

void setup() {
  pinMode(pinButtonL1, INPUT);
  pinMode(pinButtonL2, INPUT);
  pinMode(pinButtonR1, INPUT); //six buttons ready to use
  pinMode(pinButtonR2,  INPUT);
  pinMode(pinClickL, INPUT);
  pinMode(pinClickR, INPUT);

  radio.begin(); // Start the NRF24L01
  radio.setPALevel(RF24_PA_LOW);
  radio.setDataRate(RF24_250KBPS);
  radio.setChannel(69);
  radio.openWritingPipe(address); // Get NRF24L01 ready to transmit
  radio.stopListening();
  Serial.begin(9600);
}

void loop () {
  Serial.flush();
  uint8_t L1 = digitalRead(pinButtonL1);
  uint8_t L2 = digitalRead(pinButtonL2);
  uint8_t R1 = digitalRead(pinButtonR1);
  uint8_t R2 = digitalRead(pinButtonR2);        //take in all the digital inputs
  uint8_t ClickL = digitalRead(pinClickL);
  uint8_t ClickR = digitalRead(pinClickR); 
  int Ry = (analogRead(pinRightY) - 512) * .5;
  int Rx = (analogRead(pinRightX) - 512) * .5;
  int Ly = (analogRead(pinLeftY) - 512) * .5;  //place the analog inputs in variables but alter them to be [-256,256]
  int Lx = (analogRead(pinLeftX) - 512) * .5;
  uint8_t mode = 0;

  if (L1 == 1)
    mode = 4;
  else if (L2 == 1)
    mode = 5;
  else if (R1 == 1)
    mode = 6;
  else if (R2 == 1)
    mode = 7;
  else if ((Ly > 0) && (Ry > 0))    //choose a state based on the inputs from the y on each stick
    mode = 0;
  else if ((Ly > 0) && (Ry < 0))
    mode = 1;
  else if ((Ly < 0) && (Ry > 0))
    mode = 2;
  else if ((Ly < 0) && (Ry < 0))
    mode = 3;


  Ry = (constrain(abs(Ry) / 5, 0, 50));
  Ly = (constrain(abs(Ly) / 5, 0, 50)); //set a max speed of 50 
  
  if (Ry < 20)
    Ry = 0;
  if (Ly < 20) //reduce noise off the inputs
    Ly = 0;
    
  uint8_t Ly2 = Ly;
  uint8_t Ry2 = Ry;
  uint8_t message[3] = {mode, Ly2, Ry2}; //format for how the data will be sent across the entire system
  
  if (mode == 4) //L1 = 1  //All of these conditionals will see if a button is pressed, send that signal, and then hold the program in place until it is released
  { radio.write(&message, sizeof(message));
    L1 = 0;
    delay(100);
    while (L1 == 1)
      Serial.println(L1);
    L1 = digitalRead(pinButtonL1);
  }
  else if (mode == 5) //L2 = 1
  {
    radio.write(&message, sizeof(message));
    L2 = 0;
    delay(100);
    while (L2 == 1)
      L2 = digitalRead(pinButtonL2);
  }
  else if (mode == 6) //R1 = 1
  {
    radio.write(&message, sizeof(message));
    R1 = 0;
    delay(100);
    while (R1 == 1)
      R1 = digitalRead(pinButtonR1);
  }
  else if (mode == 6) //R2 = 1
  {
    radio.write(&message, sizeof(message));
    R2 = 0;
    delay(100);
    while (R2 == 1)
      R2 = digitalRead(pinButtonR2);
  }
  else
  { //if no buttons are pressed just send the data off 
    radio.write(&message, sizeof(message)); 
    for (int i = 0; i < 3; i++)
    {
      Serial.print(message[i]);
      Serial.print("\t");
    }
    Serial.println(' ');
  }
}
