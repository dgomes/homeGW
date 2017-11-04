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
#include <Plugin.h>

Plugin::Plugin() {
	packet = 0;
	error = OK;
	String error_str = "";
}

Plugin::~Plugin() {

}

void Plugin::detectPacket(unsigned int duration, Plugin *self ) {

	if(duration > END_PACKET) {
    	if(bitsRead > packet_size-packet_size*0.1 && bitsRead < packet_size+packet_size*0.1) {  //check if we are in the range +- 10%
			#ifdef DEBUG
			Serial.println(bitsRead);
			#endif
			self->processPacket();
        }
		bitsRead = 0;	
    }

	if(duration > MIN_PACKET) {
       timings[bitsRead] = duration;
       bitsRead++;
	}

	if(bitsRead == MAX_CHANGES) {
    	bitsRead = 0;
	}

}

String Plugin::getString(uint64_t packet) {
	String s = "0x";
	return s + String((long) packet, HEX);
}

uint64_t Plugin::getPacket() {
  uint64_t p = packet;
  packet = 0;
  return p;
}

bool Plugin::available() {
  if(Plugin::packet != 0) {
    if(error == OK)
      return true;
  }
  return false;
}

uint8_t Plugin::getErrno() {
  return error;
}

