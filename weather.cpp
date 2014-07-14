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
#include <weather.h>

uint64_t weather::packet = 0;
uint8_t weather::error = OK;
String weather::error_str = "";
uint8_t weather::sync = 0;

weather::weather(uint8_t s) {
	sync = s;
}

uint64_t weather::getPacket() {
  uint64_t p = packet;
  packet = 0;
  return p;
}

uint8_t weather::getChannel(uint64_t packet) {
  uint8_t channel = (packet >> 20) && 0xF;
  return channel;
}

float weather::getTemperature(uint64_t packet) {
  float temperature = float((packet >> 8) & 0xFFF) / 10;
  return temperature;
}

uint8_t weather::getHumidity(uint64_t packet) {
  uint8_t humidity = packet & 0xFF;
  return humidity;
}
  /*
    chks 0xFF
    ch 0x F
    temperature 0xFFF
    hum 0xFF
  */

uint8_t weather::isValidWeather(uint64_t ppacket) {
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

void weather::detectPacket(int bitsRead, unsigned int *timings) {
	packet = 0;
	for(int i=1; i< bitsRead; i++) {
		int duration = timings[i];

		if(duration > ONE) {
	      packet = packet << 1;
          bitSet(packet, 0);
        } else if(duration > ZERO) {
          packet = packet << 1;
          bitClear(packet, 0);
        }

	}
	#ifdef DEBUG
      Serial.print("~0x");
      Serial.println((long) packet, HEX);
	#endif
}

bool weather::available() {
  if(weather::packet != 0) {
    if(error == OK)
      return true;
  }
  return false;
}

uint8_t weather::getErrno() {
  return error;
}

String weather::getError() {
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
