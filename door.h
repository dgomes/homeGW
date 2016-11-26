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
#ifndef door_h
#define door_h

#if ARDUINO < 100
  #include <WProgram.h>
#else
  #include <Arduino.h>
#endif

#define OK	0

#define END_PACKET	5000
#define AVERAGE		800 // 400 - 1200

class door {

	private:
		static uint64_t packet;

		static uint8_t error;
		static String error_str;

		uint64_t id;
	public:
		door(uint64_t id);

		static void detectPacket(int readBits, unsigned int *timings);
		uint64_t getPacket();
		bool change();
		bool change(uint64_t packet);

		bool available();

		uint8_t getErrno();
		String getError();
};

#endif
