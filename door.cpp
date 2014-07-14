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
#include <door.h>

uint64_t door::packet = 0;
uint8_t door::error = OK;
String door::error_str = "";

uint64_t door::getPacket() {
  uint64_t p = packet;
  packet = 0;
  return p;
}

door::door(uint64_t id) {
	door::id = id;
}

bool door::change() {
	uint64_t p = packet;
	packet = 0;
	return p == id;
}

bool door::change(uint64_t p) {
	return p == id;
}

void door::detectPacket(int bitsRead, unsigned int *timings) {
    packet = 0;
    for(int i=1; i< bitsRead-1; i+=2) {
        int duration = timings[i];
        int next_duration = timings[i+1];

        if(duration > AVERAGE && next_duration < AVERAGE) {
          packet = packet << 1;
          bitSet(packet, 0);
        } else if(duration < AVERAGE && next_duration > AVERAGE) {
          packet = packet << 1;
          bitClear(packet, 0);
        }
  }
#ifdef DEBUG
      Serial.print("|0x");
      Serial.println((long) packet, HEX);
#endif

}

bool door::available() {
  if(door::packet != 0) {
    if(error == OK)
      return true;
  }
  return false;
}

uint8_t door::getErrno() {
  return error;
}

String door::getError() {
  switch(error) {
    default:
      return "unknown error: " + error_str;
  }
}
