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
#ifndef weather_h
#define weather_h

#if ARDUINO < 100
  #include <WProgram.h>
#else
  #include <Arduino.h>
#endif

#define OK	0
#define INVALID_HUMIDITY	1
#define INVALID_TEMPERATURE	2
#define INVALID_SYNC	3

#define ONE		3000 // 4000 +- 1000
#define ZERO	1000 // 2000 +- 1000

class weather {

	private:
		static uint8_t sync;
		static uint64_t packet;

		static uint8_t error;
		static String error_str;
	public:
		weather(uint8_t sync);

		static void detectPacket(int readBits, unsigned int *timings);
		uint64_t getPacket();

		static uint8_t isValidWeather(uint64_t packet);
		static uint8_t getChannel(uint64_t packet);
		static float getTemperature(uint64_t packet);
		static uint8_t getHumidity(uint64_t packet);

		bool available();

		uint8_t getErrno();
		String getError();
};

#endif
