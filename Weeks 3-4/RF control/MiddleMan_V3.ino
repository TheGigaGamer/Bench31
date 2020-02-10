#include "nRF24L01.h"
#include "RF24.h"
#include <Wire.h>
#include "SPI.h"

#define channel 0x05
#define park 0x02
#define line 0x03
#define nav 0x04
const byte address[6] = "01101";
uint8_t input[3] = {6, 2, 2};
char command;
uint8_t jamie[10][4];
RF24 radio(9, 10); // NRF24L01 used SPI pins + Pin 9 and 10 on the NANO



void setup() {
  radio.begin();
  radio.setPALevel(RF24_PA_LOW);
  radio.setDataRate(RF24_250KBPS);
  radio.setChannel(69);
  radio.openReadingPipe(1, address);
  radio.startListening(); // Listen to see if information received
  Wire.begin();
  Serial.begin(9600);
}

void require(uint8_t input[3])
{
  delay(10);
  if (input[0] == 4)
  { Serial.print("check");
    Wire.requestFrom(park, sizeof(input) + 1);
  }
  else if (input[0] == 6)
  { Serial.print("check");
    Wire.requestFrom(line, sizeof(input) + 1);
  }
  else if (input[0] == 5)
  { Serial.print("check");
   Wire.requestFrom(nav, sizeof(input) + 1);
  }
  while (Wire.available())
  { input[0] = Wire.read();
    input[1] = Wire.read();
    input[2] = Wire.read();
  }

}

void loop() {
  Serial.print("Mid:");
  if (radio.available())
  {
    radio.read(&input, sizeof(input));
    Serial.flush();
  }
  //input[0] = 6;

  if (input[0] > 3)
    require(input);


  for (int i = 0; i < 3; i++)
  {
    Serial.print(input[i]);
    Serial.print("\t");
  }
  Serial.println(' ');


  Wire.beginTransmission(channel);
  Wire.write(input[0]);
  Wire.write(input[1]);
  Wire.write(input[2]);
  Wire.endTransmission();
}
