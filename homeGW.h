#ifndef homeGW_h
#define homeGW_h

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

class homeGW {

	private:
		static unsigned int sync;
		uint8_t pin;

		static uint64_t weatherPacket;
		static uint64_t doorPacket;
		#define MAX_CHANGES 128
		static unsigned int timings[MAX_CHANGES];

		void parsePacket(uint64_t packet);
		static void handleInterrupt();

		static uint8_t error;
		static String error_str;

		static void detectWeatherPacket(int readBits);
		static void detectDoorPacket(int readBits);

	public:
		bool setup(uint8_t pin, unsigned int sync);
		uint64_t getWeatherPacket();
		uint64_t getDoorPacket();

		static uint8_t isValidWeather(uint64_t packet);
		static uint8_t getChannel(uint64_t packet);
		static float getTemperature(uint64_t packet);
		static uint8_t getHumidity(uint64_t packet);

		bool available();

		uint8_t getErrno();
		String getError();

};

#endif
