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

#define END_PACKET	5000
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
