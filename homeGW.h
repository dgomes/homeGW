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
