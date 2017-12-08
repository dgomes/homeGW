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
#include <generic.h>

generic::generic() {
	packet_size = 48;
    END_PACKET = 5000;
	MIN_PACKET = 280; 
}

void generic::processPacket() {
	packet = 0;
	for(unsigned i=1; i< bitsRead-1; i+=2) {
		unsigned duration = timings[i];
        unsigned next_duration = timings[i+1];

        if(duration > AVERAGE && next_duration < AVERAGE) {
          packet = packet << 1;
          bitSet(packet, 0);
        } else if(duration < AVERAGE && next_duration > AVERAGE) {
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


