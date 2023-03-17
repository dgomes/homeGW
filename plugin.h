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
#ifndef plugin_h
#define plugin_h

#if ARDUINO < 100
  #include <WProgram.h>
#else
  #include <Arduino.h>
#endif
		
#define MAX_CHANGES 128
#define OK	0

class Plugin {

	public:
		unsigned int timings[MAX_CHANGES];
		unsigned int bitsRead;
		unsigned int packet_size;
		
    unsigned int END_PACKET; 
		unsigned int MIN_PACKET;

		uint64_t packet;

		Plugin();
		virtual ~Plugin();

		void detectPacket(unsigned int, Plugin *);

		virtual void 		processPacket() = 0;
		virtual uint8_t getChannel(uint64_t packet) = 0;
		virtual uint8_t isValidWeather(uint64_t packet) = 0;
		virtual uint8_t getBattery(uint64_t packet) = 0;
		virtual float 	getTemperature(uint64_t packet) = 0;
		virtual uint8_t getHumidity(uint64_t packet) = 0;
		virtual	uint8_t getId(uint64_t packet) = 0;

		uint64_t getPacket();
		String getString(uint64_t packet);

		bool available();

};

#endif
