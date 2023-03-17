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
#include <digoo.h>

digoo::digoo() {
	packet_size = 37;
    END_PACKET = 3000;
	MIN_PACKET = 650;
}


uint8_t digoo::getId(uint64_t packet) {
  uint8_t id = (packet >> 28) & 0xFF;
  return id;
}

uint8_t digoo::getBattery(uint64_t packet) {
  uint8_t batt = (packet >> 24) & 0x8;
  return batt ? 1 : 0;
}

uint8_t digoo::getChannel(uint64_t packet) {
  uint8_t channel = (packet >> 24) & 0x3;
  return channel+1;
}

float digoo::getTemperature(uint64_t packet) {
	int16_t t = packet >> 12 & 0x0FFF;
	t = 0x0800 & t ? 0xF000 | t  : t;
	float temperature = float(t) / 10;
  return temperature;
}

uint8_t digoo::getHumidity(uint64_t packet) {
  uint8_t humidity = packet & 0xFF;
  return humidity;
}

uint8_t digoo::isValidWeather(uint64_t ppacket) {
  uint8_t humidity = getHumidity(ppacket);
  //humidity sensor HR202L/HR31
  if ( (humidity > 100 || humidity < 5) && (humidity != 0) ) { //working range 20%-90% or 0% for temperature only Nexus sensor 
    return INVALID_HUMIDITY;
  }
  float temperature = getTemperature(ppacket);
  if (temperature < -40.0 || temperature > 90.0) { //sanity check according to specs
    return INVALID_TEMPERATURE;
  }
  return OK;
}

void digoo::processPacket() {
	packet = 0;
	for(unsigned i=1; i< bitsRead; i++) {
		unsigned duration = timings[i];
		if(duration > digoo::ONE) {
	      packet = packet << 1;
          bitSet(packet, 0);
        } else if(duration > digoo::ZERO) {
          packet = packet << 1;
          bitClear(packet, 0);
        }

	}
	#ifdef DEBUG
    Serial.print("~0x");
    Serial.println((unsigned long) packet, HEX);
	if (packet == 0) {
		for(unsigned i=0; i < bitsRead; i++)
			Serial.println(timings[i]);
	}
	#endif
}
