#include <Wire.h>
#include "MPU6050.h"
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#define CE_PIN 2
#define CSN_PIN 4
RF24 radio(CE_PIN, CSN_PIN);

const byte address[6] = "00001";

MPU6050 accelgyro;
int16_t ax, ay, az, gx, gy, gz;
unsigned char Tx_Array[2];
unsigned char Tx_command = 0;
unsigned char Speed_index = 0;

const int TILT_THRESHOLD = 3000;

void setup()
{
  Serial.begin(115200);
  Wire.begin();

  accelgyro.initialize();

  if (!accelgyro.testConnection())
  {
    Serial.println("MPU6050 failed");
    while (1);
  }

  Serial.println("MPU6050 OK");

  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_LOW);
  radio.setDataRate(RF24_250KBPS);
  radio.stopListening();

  Serial.println("Transmitter ready");
}

void loop()
{
  accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

  Tx_command = 0;   
  Speed_index = 0;

  if (ay < -TILT_THRESHOLD)
  {
    Tx_command = 1; 
    Speed_index = map(-ay, TILT_THRESHOLD, 16384, 50, 255); 
  }
  else if (ay > TILT_THRESHOLD)
  {
    Tx_command = 2; 
    Speed_index = map(ay, TILT_THRESHOLD, 16384, 50, 255);
  }
  else if (ax < -TILT_THRESHOLD)
  {
    Tx_command = 3; 
    Speed_index = 255; 
  }
  else if (ax > TILT_THRESHOLD)
  {
    Tx_command = 4; 
    Speed_index = 255; 
  }
  Speed_index = constrain(Speed_index, 0, 255);

  Tx_Array[0] = Tx_command;
  Tx_Array[1] = Speed_index;

  radio.write(&Tx_Array, sizeof(Tx_Array));

  delay(20);  
}
