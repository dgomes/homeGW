#include "homeGW.h"
#include "weather.h"
#include "door.h"

#define DEBUG
#define RF_RECEIVER_PIN 3

homeGW gw;
weather station(0x3F);
door front(0x7F7B04);


void setup()
{
  Serial.begin( 115200 );   //using the serial port at 115200bps for debugging and logging

  gw.setup(RF_RECEIVER_PIN, 0x3F);

  gw.registerPlugin(72, weather::detectPacket);
  gw.registerPlugin(48, door::detectPacket);
}

void loop()
{
  delayMicroseconds(1000);

  if(station.available()) {
    uint64_t p = 0;
    if((p = station.getPacket())) {
      int r = weather::isValidWeather(p);

	  switch(r) {
		case OK:
	        Serial.print("{\"id\": \"weather\", \"code\": 200, \"Humidity\": ");
			Serial.print(weather::getHumidity(p));
			Serial.print(", \"Temperature\": ");
			Serial.print(weather::getTemperature(p));
#ifdef DEBUG
    	    Serial.print(String(", \"Channel\": ") + weather::getChannel(p));
#endif
			break;
		case INVALID_HUMIDITY:
      		Serial.print("{\"id\": \"weather\", \"code\": 500, \"Humidity\": " + station.getError());
			break;
		case INVALID_TEMPERATURE:
      		Serial.print("{\"id\": \"weather\", \"code\": 500, \"Temperature\": " + station.getError());
			break;
		case INVALID_SYNC:
      		Serial.print("{\"id\": \"weather\", \"code\": 500, \"Sync\": false");
			break;
      }
#ifdef DEBUG
   	  Serial.print(", \"Packet\": \"0x");
   	  Serial.print((long unsigned int) p, HEX);
	  Serial.print("\"");
#endif
	  Serial.println("}");
    }
  }


  if(front.available()) {
    uint64_t p = front.getPacket(); //getPacket clears the packet, we keep it to print it further ahead
    if(front.change(p)) {
      Serial.print("{\"id\": \"door\", \"code\": 200, \"change\": 1");
      Serial.println("}");
    } else {
      Serial.print("{\"id\": \"door\", \"code\": 500");
#ifdef DEBUG
      Serial.print(", \"Packet\": \"0x");
      Serial.print((long unsigned int) p, HEX);
      Serial.println("\"}");
#endif
    }
  }
}
