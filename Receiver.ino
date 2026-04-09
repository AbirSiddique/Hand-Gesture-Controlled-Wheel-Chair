#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#define CE_PIN 2
#define CSN_PIN 4
RF24 radio(CE_PIN, CSN_PIN);
const byte address[6] = "00001";

#define LEFT_RPWM 8
#define LEFT_LPWM 9
#define RIGHT_RPWM 5
#define RIGHT_LPWM 6

unsigned char Rx_Array[2];
unsigned char command = 0;
unsigned char speed = 0;

void setup()
{
  pinMode(LEFT_RPWM, OUTPUT);
  pinMode(LEFT_LPWM, OUTPUT);
  pinMode(RIGHT_RPWM, OUTPUT);
  pinMode(RIGHT_LPWM, OUTPUT);

  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_LOW);
  radio.setDataRate(RF24_250KBPS);
  radio.startListening();
}

void loop()
{
  if (radio.available())
  {
    radio.read(&Rx_Array, sizeof(Rx_Array));
    command = Rx_Array[0];
    speed = Rx_Array[1];

    switch (command)
    {
      case 1: forward(speed); break;
      case 2: backward(speed); break;
      case 3: right(speed); break;
      case 4: left(speed); break;
      default: stopMotor(); break;
    }
  }
}

void forward(unsigned char spd)
{
  analogWrite(LEFT_RPWM, spd);
  analogWrite(LEFT_LPWM, 0);

  analogWrite(RIGHT_RPWM, spd);
  analogWrite(RIGHT_LPWM, 0);
}

void backward(unsigned char spd)
{
  analogWrite(LEFT_RPWM, 0);
  analogWrite(LEFT_LPWM, spd);

  analogWrite(RIGHT_RPWM, 0);
  analogWrite(RIGHT_LPWM, spd);
}

void right(unsigned char spd)
{
  analogWrite(LEFT_RPWM, spd);
  analogWrite(LEFT_LPWM, 0);

  analogWrite(RIGHT_RPWM, 0);
  analogWrite(RIGHT_LPWM, spd);
}

void left(unsigned char spd)
{
  analogWrite(LEFT_RPWM, 0);
  analogWrite(LEFT_LPWM, spd);

  analogWrite(RIGHT_RPWM, spd);
  analogWrite(RIGHT_LPWM, 0);
}

void stopMotor()
{
  analogWrite(LEFT_RPWM, 0);
  analogWrite(LEFT_LPWM, 0);

  analogWrite(RIGHT_RPWM, 0);
  analogWrite(RIGHT_LPWM, 0);
}
