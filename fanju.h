/*
  Written by Danil Korotkov, spballiance@gmail.com
  based on digoo.h by Diogo Gomes, diogogomes@gmail.com
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
