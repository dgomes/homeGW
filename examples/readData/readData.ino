#include "homeGW.h"

#define RF_RECEIVER_PIN 3

homeGW station;

void setup()
{
  Serial.begin( 115200 );   //using the serial port at 115200bps for debugging and logging

  station.setup(RF_RECEIVER_PIN);
}

void loop()
{
  delayMicroseconds(1000);

  if(station.available()) {
	Serial.println(station.getTemperature());
	station.clear();
  }
}

