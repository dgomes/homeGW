#include "homeGW.h"

#define DEBUG
#define RF_RECEIVER_PIN 3

homeGW station;

void setup()
{
  Serial.begin( 115200 );   //using the serial port at 115200bps for debugging and logging

  station.setup(RF_RECEIVER_PIN, 0x3F);
}

void loop()
{
  delayMicroseconds(1000);

  if(station.available()) {
    uint64_t p = 0;
    if((p = station.getWeatherPacket())) {
      int r = station.isValidWeather(p);

	  switch(r) {

		case OK:
	        Serial.print("{\"code\": 200, \"Humidity\": ");
			Serial.print(station.getHumidity(p));
			Serial.print(", \"Temperature\": ");
			Serial.print(station.getTemperature(p));
#ifdef DEBUG
    	    Serial.print(String(", \"Channel\": ") + station.getChannel(p));
#endif
			break;
		case INVALID_HUMIDITY:
      		Serial.print("{\"code\": 500, \"Humidity\": " + station.getError());
			break;
		case INVALID_TEMPERATURE:
      		Serial.print("{\"code\": 500, \"Temperature\": " + station.getError());
			break;
		case INVALID_SYNC:
      		Serial.print("{\"code\": 500, \"Sync\": false");
			break;
      }
#ifdef DEBUG
    	    Serial.print(", \"Packet\": \"0x");
    	    Serial.print((long unsigned int) p, HEX);
#endif
	        Serial.println(String("\"}"));
    } else if((p = station.getDoorPacket())) {
	  if(p != 0x7F7B04)
		return;
      Serial.print("{\"code\": 200, \"DoorOpen\": true");
#ifdef DEBUG
      Serial.print(", \"Packet\": \"0x");
      Serial.print((long unsigned int) p, HEX);
#endif
      Serial.println(String("}"));
    }
  }
}

