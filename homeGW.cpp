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

#if defined(ESP8266) || defined(ESP32)
    #define ISR_PREFIX ICACHE_RAM_ATTR
#else
    #define ISR_PREFIX
#endif

Plugin **HomeGW::plugin;
uint8_t HomeGW::MAX_PLUGINS;

HomeGW::HomeGW(uint8_t max_plugins) {
	MAX_PLUGINS = max_plugins;
	plugin = new Plugin*[MAX_PLUGINS];
	for(int i=0; i<MAX_PLUGINS; i++)
		plugin[i] = NULL; 
}

HomeGW::~HomeGW() {
	delete HomeGW::plugin;
}


void HomeGW::registerPlugin(Plugin *p) {
	for(int i=0; i<MAX_PLUGINS; i++) {
		if(plugin[i] == NULL) {
			plugin[i] = p;
			return;
		}
	}

}

bool HomeGW::setup(uint8_t pin) {
  bool isESP = false;

#if defined(ESP8266)
  isESP = true;
#endif

  if (!isESP && (!(pin == 3 || pin == 2)))
  {
    return false;
  }

  HomeGW::pin = pin;

  pinMode(pin, INPUT);
  digitalWrite(pin, LOW);
  uint8_t interuptPin;

  if (!isESP)
  {
    interuptPin = pin - 2;
  }
  else
  {
    interuptPin = pin;
  }

  attachInterrupt(interuptPin, HomeGW::handleInterrupt, CHANGE); // 1 = PIN3

  return true;
}

ISR_PREFIX void HomeGW::handleInterrupt() {
  static unsigned long lastTime;

  long time = micros();
  unsigned int duration = time - lastTime;

  for(int i=0; i<MAX_PLUGINS; i++) {
    if(plugin != NULL) {
		plugin[i]->detectPacket(duration, plugin[i]);
	}
  }

  lastTime = time;
}
