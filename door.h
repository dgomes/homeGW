#ifndef door_h
#define door_h

#if ARDUINO < 100
  #include <WProgram.h>
#else
  #include <Arduino.h>
#endif

#define OK	0

#define END_PACKET	5000
#define AVERAGE		800 // 400 - 1200

class door {

	private:
		static uint64_t packet;

		static uint8_t error;
		static String error_str;

		uint64_t id;
	public:
		door(uint64_t id);

		static void detectPacket(int readBits, unsigned int *timings);
		uint64_t getPacket();
		bool change();
		bool change(uint64_t packet);

		bool available();

		uint8_t getErrno();
		String getError();
};

#endif
