/*
  Written by Danil Korotkov, spballiance@gmail.com
  based on digoo.cpp by Diogo Gomes, diogogomes@gmail.com
  and PiLight project https://github.com/pilight/pilight
*/
#include <fanju.h>
extern "C"{
  #include "binary.h"
}

fanju::fanju() {
	packet_size = 47;
  END_PACKET = 10000;
	MIN_PACKET = 650;
  binarray[MAX_CHANGES] = {0};
  SYNC = 1000;
  HEAD = 7900;
}


uint8_t fanju::getId(uint64_t packet) {
  uint8_t id = binToDecRev(fanju::binarray, 0, 7);
  return id;
}

uint8_t fanju::getBattery(uint64_t packet) {
  uint8_t batt = fanju::binarray[13];
  return batt ? 0 : 1;
}

uint8_t fanju::getChannel(uint64_t packet) {
  uint8_t channel = binToDecRev(fanju::binarray, 38, 39);
  return channel;
}

float fanju::getTemperature(uint64_t packet) {
	double t = (double) binToDecRevUl(fanju::binarray, 16, 27);
	float temperature = ((float(t) - 0x4C4) * 5) / 90;
  return temperature;
}

uint8_t fanju::getHumidity(uint64_t packet) {
  uint8_t humidity = binToDecRev(fanju::binarray, 32, 35) + 10 * binToDecRev(fanju::binarray, 28, 31);
  return humidity;
}

uint8_t fanju::isValidWeather(uint64_t ppacket) {
  uint8_t humidity = getHumidity(ppacket);
  if (humidity > 100) { //sanity check according to specs
    return INVALID_HUMIDITY;
  }
  float temperature = getTemperature(ppacket);
  if (temperature < -40.0 || temperature > 90.0) { //sanity check according to specs
    return INVALID_TEMPERATURE;
  }
  return OK;
}

void fanju::processPacket() {
	packet = 0;
  memset(fanju::binarray, 0, sizeof(fanju::binarray));
  uint8_t sync=0;
  uint8_t offset=0;
  for(unsigned i=1; i< bitsRead; i++) {
    unsigned duration = timings[i];
    if(duration > fanju::SYNC*0.9 && duration < fanju::SYNC*1.1) {
      sync++;
      if (sync == 8) {
        //Serial.println(" SYNC detected ");
        offset = i + 1;
        break;
      }
    } else {
      sync = 0;
      return;
    }
  }
 

  if (timings[offset]     > fanju::HEAD*0.9 && 
      timings[offset]     < fanju::HEAD*1.1) {
    offset++;
  } else {
    offset = 0;
    sync = 0;
    return;
  }

	for(unsigned i=offset; i< bitsRead; i++) {
		unsigned duration = timings[i];
		if(duration > fanju::ONE) {
	      packet = packet << 1;
          bitSet(packet, 0);
          fanju::binarray[i-offset] = 1;
        } else if(duration > fanju::ZERO) {
          packet = packet << 1;
          bitClear(packet, 0);
          fanju::binarray[i-offset] = 0;
        }
	}
	
  if (!ChkSum()){
    memset(fanju::binarray, 0, sizeof(fanju::binarray));
    packet = 0;
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

bool  fanju::ChkSum(){
  uint8_t mask = 0xc;
  uint8_t x, bit, csum = 0;

  uint8_t chkSum = binToDecRev(fanju::binarray, 8, 11);

  int binary_cpy[fanju::packet_size];

  // move channel to the checksum position
  x = 0;
  for (unsigned i=0; i < 8; i++) {
    binary_cpy[x++] = fanju::binarray[i];
  }
  for (unsigned i=36; i < 40; i++) {
    binary_cpy[x++] = fanju::binarray[i];
  }
  for (unsigned i= 12; i < 36; i++) {
    binary_cpy[x++] = fanju::binarray[i];
  }

// verify checksum
for (unsigned i=0; i < 36; i++) {
  bit = mask & 0x1;
  mask >>= 1;
  if (bit == 0x1) {mask ^= 0x9;}
  if (binary_cpy[i] == 1) {csum ^= mask;}
  }

  return csum == chkSum;
}
