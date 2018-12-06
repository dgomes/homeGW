#define BMP
//#define REMOTE

#include <ArduinoJson.h>

#include <homeGW.h>
#include <digoo.h>
#include <weather.h>
#ifdef REMOTE
#include <generic.h>
#endif

#ifdef BMP
#include <Adafruit_BMP085.h>
Adafruit_BMP085 bmp;
#endif

#ifdef REMOTE
HomeGW gw(3);
#else
HomeGW gw(2);
#endif
weather station1;
digoo station2;
#ifdef REMOTE
generic remote;
#endif

uint64_t prev_p = 0;

#ifndef BMP
#define LM35sensorPin 0  //A0
#endif

#define RF_RECEIVER_PIN 3 // 3 -> RF_GW, 2 -> RFv0

void setup() {
  Serial.begin(115200);
  // pull-down RF pin (less noise)
  pinMode(RF_RECEIVER_PIN, OUTPUT);
  digitalWrite(RF_RECEIVER_PIN, LOW);
  pinMode(RF_RECEIVER_PIN, INPUT);
  digitalWrite(RF_RECEIVER_PIN, LOW);
  gw.setup(RF_RECEIVER_PIN);

  gw.registerPlugin(&station1); 
  gw.registerPlugin(&station2); 
#ifdef REMOTE
  gw.registerPlugin(&remote); 
#endif

  #ifdef BMP
  if(bmp.begin()) {
    Serial.println(F("{\"dev\":\"arduino\", \"name\": \"hass_serial\", \"device_class\": \"sensor\", \"bmp\": 1}"));
  } else
    Serial.println(F("{\"dev\":\"arduino\", \"name\": \"hass_serial\", \"device_class\": \"sensor\", \"error\":\"bmp\"}"));
  #else
    Serial.println(F("{\"dev\":\"arduino\", \"name\": \"hass_serial\", \"device_class\": \"sensor\", \"bmp\": 0}"));
  #endif
}

void loop() { 
  uint64_t p = 0;
  StaticJsonBuffer<160> jsonBuffer;
  JsonObject& root = jsonBuffer.createObject();

  if(station1.available()) 
    if((p = station1.getPacket())) {
      if(p == prev_p) {

        root[F("dev")] = F("weather");
        root[F("id")] = station1.getId(p);
        root[F("ch")] = station1.getChannel(p);
        root[F("batt")] = station1.getBattery(p);
        root[F("temp")] = station1.getTemperature(p);
        root[F("hum")] = station1.getHumidity(p);
        root[F("raw")] = station1.getString(p);
  
        #ifdef BMP
        root["in_temp"] = bmp.readTemperature();
        root["pressure"] = bmp.readPressure();
        #else
        //http://playground.arduino.cc/Main/LM35HigherResolution 
        root["in_temp"] = analogRead(LM35sensorPin)*0.45;
        #endif
  
        root.printTo(Serial);
        Serial.println();
        p = 0;
      }
      prev_p = p;
    }
  

  if(station2.available()) 
    if((p = station2.getPacket())) {
      if(p == prev_p) {

        root[F("dev")] = F("digoo");
        root[F("id")] = station2.getId(p);
        root[F("ch")] = station2.getChannel(p);
        root[F("batt")] = station2.getBattery(p);
        root[F("temp")] = station2.getTemperature(p);
        root[F("hum")] = station2.getHumidity(p);
        root[F("raw")] = station2.getString(p);
  
        root.printTo(Serial);
        Serial.println();
        p = 0;
      }
      prev_p = p;
    }

#ifdef REMOTE
  if(remote.available()) 
    if((p = remote.getPacket())) {
      if(p != prev_p) {

        root[F("dev")] = F("generic");
        root[F("raw")] = remote.getString(p);
  
        root.printTo(Serial);
        Serial.println();
      }
      prev_p = p;
    }
#endif
  
}

