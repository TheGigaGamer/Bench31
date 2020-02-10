#include "Wire.h" // This library allows you to communicate with I2C devices.
#include "nRF24L01.h"
#include "RF24.h"
#include "SPI.h"
const int MPU_ADDR = 0x68; // I2C address of the MPU-6050. If AD0 pin is set to HIGH, the I2C address will be 0x69.
int16_t gyro_x, gyro_y, gyro_z; // variables for gyro raw data
int16_t previous[2][3] ={{0,0,0}, {0, 0, 0}};
int16_t commands[] = {0, 0, 0};
uint8_t finalmode[3] = {0,0,0};
RF24 radio(9, 10); // NRF24L01 used SPI pins + Pin 9 and 10 on the NANO
const byte address[6] = "01101";
int Yindex = 1;
void setup() {
  Serial.begin(9600);
  radio.begin(); // Start the NRF24L01
  radio.setPALevel(RF24_PA_LOW);
  radio.setDataRate(RF24_250KBPS);
  radio.setChannel(69);
  radio.openWritingPipe(address); // Get NRF24L01 ready to transmit
  radio.stopListening();
  Wire.begin();
  Wire.beginTransmission(MPU_ADDR); // Begins a transmission to the I2C slave (GY-521 board)
  Wire.write(0x6B); // PWR_MGMT_1 register
  Wire.write(0); // set to zero (wakes up the MPU-6050)
  Wire.endTransmission(true);
}
void loop() {
  int16_t data[] = {0, 0, 0};
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x43); // starting with register 0x3B (ACCEL_XOUT_H) [MPU-6000 and MPU-6050 Register Map and Descriptions Revision 4.2, p.40]
  Wire.endTransmission(false); // the parameter indicates that the Arduino will send a restart. As a result, the connection is kept active.
  Wire.requestFrom(MPU_ADDR, 3 * 2, true); // request a total of 7*2=14 registers

  gyro_x = Wire.read() << 8 | Wire.read(); // reading registers: 0x43 (GYRO_XOUT_H) and 0x44 (GYRO_XOUT_L)
  gyro_y = Wire.read() << 8 | Wire.read(); // reading registers: 0x45 (GYRO_YOUT_H) and 0x46 (GYRO_YOUT_L)
  gyro_z = Wire.read() << 8 | Wire.read(); // reading registers: 0x47 (GYRO_ZOUT_H) and 0x48 (GYRO_ZOUT_L)
  data[0] += gyro_x;
  data[1] += gyro_y;
  data[2] += gyro_z;
  delay(10);
  for (int i = 0; i < 3; i++)
  {
    data[i] *= .1;
    Serial.print(data[i]);
    Serial.print("\t");
  }
  Serial.print(' ');
  if (abs(data[0]) < 500)
    data[0] = 0;
  if (abs(data[1]) < 300)
    data[1] = 0;
  if (data[0] != 0);
  {
    if(data[0] > 0)
        previous[0][1] = 1;
    else if (data[0] < 0)
        previous[0][1] = -1;
    else if (data[0] == 0)
        previous[0][1] = 0;
  }
  if (data[1] != 0);
  {
    if(data[1] > 0)
        previous[0][2] = 1;
    else if (data[1] < 0)
        previous[0][2] = -1;
    else if (data[1] == 0)
        previous[0][2] = 0;  
  }
  
  int speeds[3][3] = {{30,30,3},{0,0,0},{30,30,0}};
  if (previous[0][1] > previous[1][1])
   Yindex++;
  else if (previous[0][1] < previous[1][1])
    Yindex--;
  if (previous[0][2] > previous[1][2])
   speeds[Yindex][2] = 1;
  else if (previous[0][2] < previous[1][2])
   speeds[Yindex][2] = 2;
  Yindex = constrain(Yindex,0,2);
  if ((data[0] != 0) || (data[0] != 0))
  {
    for (int i = 0; i < 3; i++)
      finalmode[i] = speeds[Yindex][i];
  }
  Serial.print(finalmode[0]);
  Serial.print("\t");
  
  Serial.print(finalmode[1]);
  Serial.print("\t");

  
  Serial.print(finalmode[2]);
  Serial.println ("\t");

  for (int i = 0; i < 3; i++)
    previous[1][i] = previous[0][i];

  Serial.flush();
  radio.write(&finalmode, sizeof(finalmode));

}
