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
#include <homeGW.h>

unsigned int homeGW::timings[MAX_CHANGES];
PacketParser homeGW::plugin[MAX_PLUGINS] = {NULL, NULL};
unsigned int homeGW::plugin_trigger[MAX_PLUGINS] = {MAX_CHANGES, MAX_CHANGES};

void homeGW::registerPlugin(int bits, PacketParser f) {
	for(int i=0; i<MAX_PLUGINS; i++) {
		if(homeGW::plugin[i] == NULL) {
			homeGW::plugin[i] = f;
			homeGW::plugin_trigger[i] = bits;
			return;
		}
	}

}

bool homeGW::setup(uint8_t pin, unsigned int sync) {
  if(!(pin == 3 || pin == 2)) {
    return false;
  }

  homeGW::pin = pin;

  pinMode(pin, INPUT);
  digitalWrite(pin, LOW);

  attachInterrupt(pin-2, homeGW::handleInterrupt, CHANGE); // 1 = PIN3

  return true;
}

void homeGW::handleInterrupt() {
  static unsigned long lastTime;
  static unsigned int bitsRead;

  long time = micros();
  unsigned int duration = time - lastTime;

  if(duration > END_PACKET) {

	for(int i=0; i<MAX_PLUGINS; i++) {
		if(homeGW::plugin_trigger[i] != NULL) {
			if(bitsRead > homeGW::plugin_trigger[i]-homeGW::plugin_trigger[i]*0.1 && bitsRead < homeGW::plugin_trigger[i]+homeGW::plugin_trigger[i]*0.1) {  //check if we are in the range +- 10%
				#ifdef DEBUG
				Serial.println(bitsRead);
				#endif
				homeGW::plugin[i](bitsRead, homeGW::timings);
			}
		}
	}
    bitsRead = 0;

  }

  if(duration > MIN_PACKET) {
  	homeGW::timings[bitsRead] = duration;
  	bitsRead++;
  }

  if(bitsRead == MAX_CHANGES) {
	bitsRead = 0;
  }
  lastTime = time;
}
