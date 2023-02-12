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
#ifndef fanju_h
#define fanju_h

#if ARDUINO < 100
  #include <WProgram.h>
#else
  #include <Arduino.h>
#endif

#include <plugin.h>

#define INVALID_HUMIDITY	1
#define INVALID_TEMPERATURE	2
#define INVALID_SYNC	3

class fanju : public Plugin {

	private:
		const static unsigned int ONE = 3650; 
		const static unsigned int ZERO = 1750;
		
	public:
		fanju();
		int binarray[MAX_CHANGES] = {0};
  	int SYNC = 1000;
  	int HEAD = 7900;

		static void detectPacket(unsigned int, Plugin *);
		void processPacket();

		uint8_t isValidWeather(uint64_t packet);

		uint8_t getId(uint64_t packet);
    uint8_t getBattery(uint64_t packet);
		uint8_t getChannel(uint64_t packet);
		float 	getTemperature(uint64_t packet);
		uint8_t getHumidity(uint64_t packet);
		bool		ChkSum();

};

#endif
