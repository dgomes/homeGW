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

weather::weather() {
	packet_size = 74;
	END_PACKET = 5000;
	MIN_PACKET = 250;
}

uint8_t weather::getId(uint64_t packet) {
	uint8_t id = (packet >> 24) & 0xFF;
	return id;
}

uint8_t weather::getBattery(uint64_t packet) {
	uint8_t battery = (packet >> 20) & 0x8;
	return battery ? 1 : 0;
}

uint8_t weather::getChannel(uint64_t packet) {
	uint8_t channel = (packet >> 20) & 0x3;
	return channel+1;
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
	uint8_t humidity = getHumidity(ppacket);
	//Specs http://www.amazon.co.uk/gp/product/B00327G0MA/ref=oh_details_o00_s00_i00
	if (humidity > 90  || humidity < 20) { //humidity sensor HR202L/HR31
		return INVALID_HUMIDITY;
	}
	float temperature = getTemperature(ppacket);
	if (temperature < -20.0 || temperature > 50.0) { //sanity check according to specs
		return INVALID_TEMPERATURE;
	}
	return OK;
}

void weather::processPacket() {
	packet = 0;
	for(unsigned i=1; i< bitsRead; i++) {
		unsigned duration = this->timings[i];

		if(duration > weather::ONE) {
			packet = packet << 1;
			bitSet(packet, 0);
		} else if(duration > weather::ZERO) {
			packet = packet << 1;
			bitClear(packet, 0);
		}

	}
#ifdef DEBUG
	Serial.print("~0x");
	Serial.println((long) packet, HEX);
#endif
}

