#define DEBUG

#include <ArduinoJson.h>

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
			StaticJsonBuffer<160> jsonBuffer;

			JsonObject& root = jsonBuffer.createObject();
			root["device"] = "weather";
			root["id"] = station1.getId(p);
			root["ch"] = station1.getChannel(p);
			root["batt"] = station1.getBattery(p);
			root["temp"] = station1.getTemperature(p);
			root["hum"] = station1.getHumidity(p);
			root["raw"] = station1.getString(p);

			root.printTo(output);
		}
	}

	if(station2.available()) {
		if((p = station2.getPacket())) {
			StaticJsonBuffer<160> jsonBuffer;

			JsonObject& root = jsonBuffer.createObject();
			root["device"] = "digoo";
			root["id"] = station2.getId(p);
			root["ch"] = station2.getChannel(p);
			root["batt"] = station2.getBattery(p);
			root["temp"] = station2.getTemperature(p);
			root["hum"] = station2.getHumidity(p);
			root["raw"] = station2.getString(p);

			root.printTo(output);
		}
	}

	if(remote.available()) {
		if((p = remote.getPacket())) {
			StaticJsonBuffer<160> jsonBuffer;

			JsonObject& root = jsonBuffer.createObject();
			root["device"] = "generic";
			root["raw"] = remote.getString(p);

			root.printTo(output);
		}
	}

	if(output.length()) {
		Serial.println(output);
	}
}
