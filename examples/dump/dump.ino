
#include <homeGW.h>
#include <digoo.h>
#include <weather.h>
#include <generic.h>


HomeGW gw(3);
weather station1;
digoo station2;
generic remote;

#define RF_RECEIVER_PIN 2

void setup() {
	Serial.begin(115200);

	pinMode(RF_RECEIVER_PIN, OUTPUT);
	digitalWrite(RF_RECEIVER_PIN, LOW);
	pinMode(RF_RECEIVER_PIN, INPUT);
	digitalWrite(RF_RECEIVER_PIN, LOW);
	gw.setup(RF_RECEIVER_PIN);

	gw.registerPlugin(&station1); 
	gw.registerPlugin(&station2); 
	gw.registerPlugin(&remote); 
}

void loop() { 
	uint64_t p = 0;
	String output = "";

	if(station1.available()) {
		if((p = station1.getPacket())) {
			Serial.print("Weather: ");
			Serial.println(station1.getString(p));
		}
	}

	if(station2.available()) {
		if((p = station2.getPacket())) {
			Serial.print("Digoo: ");
			Serial.println(station2.getString(p));
		}
	}

	if(remote.available()) {
		if((p = remote.getPacket())) {
			Serial.print("Generic: ");
			Serial.println(remote.getString(p));
		}
	}

}
