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
#ifndef homeGW_h
#define homeGW_h

#if ARDUINO < 100
  #include <WProgram.h>
#else
  #include <Arduino.h>
#endif

#define END_PACKET	5000
#define MIN_PACKET	250	
#define MAX_PLUGINS	2

typedef void (* PacketParser) (int, unsigned int*);

class homeGW {

	private:
		uint8_t pin;

		#define MAX_CHANGES 128
		static unsigned int timings[MAX_CHANGES];

		static PacketParser plugin[2];
		static unsigned int plugin_trigger[2];

		static void handleInterrupt();

	public:
		bool setup(uint8_t pin, unsigned int sync);

		void registerPlugin(int bits, PacketParser f);
};

#endif
