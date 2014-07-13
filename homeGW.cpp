/*
  Written by Diogo Gomes, diogogomes@gmail.com

  Copyright (c) 2014 Diogo Gomes.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

*/
#include <homeGW.h>

unsigned int homeGW::sync = 0;
uint64_t homeGW::weatherPacket = 0;
uint64_t homeGW::doorPacket = 0;
unsigned int homeGW::timings[MAX_CHANGES];
uint8_t homeGW::error = OK;
String homeGW::error_str = "";

bool homeGW::setup(uint8_t pin, unsigned int sync) {
  if(!(pin == 3 || pin == 2)) {
    return false;
  }

  homeGW::sync = sync;
  homeGW::pin = pin;

  pinMode(pin, INPUT);
  digitalWrite(pin, LOW);

  attachInterrupt(pin-2, homeGW::handleInterrupt, CHANGE); // 1 = PIN3

  return true;
}

uint64_t homeGW::getWeatherPacket() {
  uint64_t p = homeGW::weatherPacket;
  homeGW::weatherPacket = 0;
  return p;
}

uint64_t homeGW::getDoorPacket() {
  uint64_t p = homeGW::doorPacket;
  homeGW::doorPacket = 0;
  return p;
}

uint8_t homeGW::getChannel(uint64_t packet) {
  uint8_t channel = (packet >> 20) && 0xF;
  return channel;
}

float homeGW::getTemperature(uint64_t packet) {
  float temperature = float((packet >> 8) & 0xFFF) / 10;
  return temperature;
}

uint8_t homeGW::getHumidity(uint64_t packet) {
  uint8_t humidity = packet & 0xFF;
  return humidity;
}
  /*
    chks 0xFF
    ch 0x F
    temperature 0xFFF
    hum 0xFF
  */

uint8_t homeGW::isValidWeather(uint64_t ppacket) {
  //check we are synced (1st byte is always the same until you replace batteries)
  if (((ppacket >> 24) & 0xFF) != sync) {
    error_str = String("0x" + String((int) (ppacket >> 24) & 0xFF, HEX));
    return INVALID_SYNC;
  }

  uint8_t humidity = getHumidity(ppacket);
  //Specs http://www.amazon.co.uk/gp/product/B00327G0MA/ref=oh_details_o00_s00_i00
  if (humidity < 0 || humidity > 100) { //sanity check according to specs
  error_str = String(humidity);
    return INVALID_HUMIDITY;
  }
  float temperature = getTemperature(ppacket);
  if (temperature < -20.0 || temperature > 50.0) { //sanity check according to specs
  error_str = String((int) temperature) + "." + String(((int) (temperature * 10)) % 10);
    return INVALID_TEMPERATURE;
  }
  return OK;
}

void homeGW::detectWeatherPacket(int bitsRead) {
	weatherPacket = 0;
	for(int i=1; i< bitsRead; i++) {
		int duration = homeGW::timings[i];

		if(duration > ONE) {
	      weatherPacket = weatherPacket << 1;
          bitSet(weatherPacket, 0);
        } else if(duration > ZERO) {
          weatherPacket = weatherPacket << 1;
          bitClear(weatherPacket, 0);
        }

	}
#ifdef DEBUG
      Serial.print("~0x");
      Serial.println((long) weatherPacket, HEX);
#endif
}

void homeGW::detectDoorPacket(int bitsRead) {
	doorPacket = 0;
	for(int i=1; i< bitsRead-1; i+=2) {
		int duration = homeGW::timings[i];
		int next_duration = homeGW::timings[i+1];

		if(duration > 800 && next_duration < 800) {
	      doorPacket = doorPacket << 1;
          bitSet(doorPacket, 0);
        } else if(duration < 800 && next_duration > 800) {
          doorPacket = doorPacket << 1;
          bitClear(doorPacket, 0);
        }
  }
#ifdef DEBUG
      Serial.print("|0x");
      Serial.println((long) doorPacket, HEX);
#endif
}

void homeGW::handleInterrupt() {
  static unsigned long lastTime;
  static unsigned int bitsRead;

  long time = micros();
  unsigned int duration = time - lastTime;

  if(duration > END_PACKET) {

	if(bitsRead >= 72) {
	  detectWeatherPacket(bitsRead);
	} else if(bitsRead >= 48) {
	  detectDoorPacket(bitsRead);
	}
    bitsRead = 0;

  }

  if(duration > 250) {
  	homeGW::timings[bitsRead] = duration;
  	bitsRead++;
  }

  if(bitsRead == MAX_CHANGES) {
	bitsRead = 0;
  }
  lastTime = time;
}

bool homeGW::available() {
  if(homeGW::weatherPacket != 0 || homeGW::doorPacket != 0) {
    if(error == OK)
      return true;
  }
  return false;
}

uint8_t homeGW::getErrno() {
  return error;
}

String homeGW::getError() {
  switch(error) {
    case INVALID_SYNC:
      return "Invalid checksum: " + error_str;
    case INVALID_TEMPERATURE:
      return "Invalid temperature: " + error_str;
    case INVALID_HUMIDITY:
      return "Invalid humidity: " + error_str;
    default:
      return "unknown error";
  }
}
