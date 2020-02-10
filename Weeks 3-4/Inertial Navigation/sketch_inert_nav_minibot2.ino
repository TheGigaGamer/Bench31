
#include <MPU9250.h>
#include <LiquidCrystal.h>
MPU9250 IMU(Wire,0x68);
#include "I2CControl.h"
#include <Wire.h>
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
int status;
int i = 2;
int index;
float x0;
bool flag;
float gyz;
float gpmax;
float gnmax;
float rads[] = {21,50,-20};
int tims[] = {1000,3000,2000};

void setup() 
{
  lcd.begin(20, 4);
 
  // serial to display data
  Serial.begin(9600);
  Wire.begin(0x04);
  while(!Serial) {}

  // start communication with IMU 
  status = IMU.begin();
  if (status < 0) 
  {
    Serial.println("IMU initialization unsuccessful");
    Serial.println("Check IMU wiring or try cycling power");
    Serial.print("Status: ");
    Serial.println(status);
    while(1) {}
   }
  
  // setting the accelerometer full scale range to +/-8G 
  IMU.setAccelRange(MPU9250::ACCEL_RANGE_8G);
  // setting the gyroscope full scale range to +/-500 deg/s
  IMU.setGyroRange(MPU9250::GYRO_RANGE_500DPS);
  // setting DLPF bandwidth to 20 Hz
  IMU.setDlpfBandwidth(MPU9250::DLPF_BANDWIDTH_20HZ);
  // setting SRD to 19 for a 50 Hz update rate
  IMU.setSrd(19);
}


void loop() 
{
  flag = false;
  while (i = 0)
  {
    
  }
   
  x0 = rads[index];

  
  if (x0 > 0)
  {
    Wire.beginTransmission(0x05);
    Wire.write(1);
    Wire.write(50);
    Wire.write(50);
    Wire.endTransmission();
    while (flag == false)
    {
      gyz = IMUR ();
      Serial.print(gyz,3);
      Serial.print("\n");
      delay (50);
      if (x0 < gyz)
      {
         flag = true;
      }
    }

  }

  
  if (x0 < 0)
  {
    Wire.beginTransmission(0x05);
    Wire.write(2);
    Wire.write(50);
    Wire.write(50);
    Wire.endTransmission();
    while (flag == false)
    {
      gyz = IMUR ();
      Serial.print(gyz,3);
      Serial.print("\n");
      delay (50);
      if (x0 > gyz)
      {
        flag = true;
      }
    }
  }
  Wire.beginTransmission(0x05);
  Wire.write(0);
  Wire.write(0);
  Wire.write(0);
  Wire.endTransmission();
  delay (1000);
  x0 = tims[index];
  Wire.beginTransmission(0x05);
  Wire.write(0);
  Wire.write(20);
  Wire.write(20);
  Wire.endTransmission();
  delay (x0);
  Wire.beginTransmission(0x05);
  Wire.write(0);
  Wire.write(0);
  Wire.write(0);
  Wire.endTransmission();
  delay (200);
  index++;
  i--;
}

int IMUR ()
{
  IMU.readSensor();
  
  if (IMU.getGyroZ_rads() > 0.1)
  {
    gpmax = IMU.getGyroZ_rads()+gpmax;
  }
  if (IMU.getGyroZ_rads() < -0.1)
  {
    gnmax = IMU.getGyroZ_rads()+gnmax;
  }
  
  gyz = gpmax + gnmax;
  Serial.print(gyz,3);
  Serial.print("\n");
  
  return (gyz);
}
