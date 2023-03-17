#include "homeGW.h"
#include "digoo.h"
#include "weather.h"
#include "fanju.h"

#include <esp_task_wdt.h>

#define WDT_TIMEOUT 100 

HomeGW gw(3); //  is the number of plugins to be registered 
digoo   DigooStation;
weather WeatherStation;
fanju   FanjuStation;

Plugin    *WeatherPlugin[3]    = {&DigooStation, &WeatherStation, &FanjuStation};
String    PluginName[3]        = {"Digoo:    ", "Weather:   ", "Fanju:    "};

#define RF_RECEIVER_PIN 2

uint64_t prev_p = 0;
uint8_t current_ch = 0;

void setup() {
  Serial.begin(115200);

  gw.setup(RF_RECEIVER_PIN);
  gw.registerPlugin(&DigooStation); 
  gw.registerPlugin(&WeatherStation);
  gw.registerPlugin(&FanjuStation);  
    
  Serial.print("Configuring WDT...\n");

  esp_task_wdt_init(WDT_TIMEOUT, false);
  esp_task_wdt_add(NULL);
  
  esp_task_wdt_reset();
}

void loop() { 
  esp_task_wdt_reset();
  uint64_t p = 0;

  for (uint8_t plugin = 0; plugin < 3; plugin++){
    if(WeatherPlugin[plugin]->available()){
      if((p = WeatherPlugin[plugin]->getPacket())){
        if(p == prev_p){
          String sens = PluginName[plugin];
                             
          if (!WeatherPlugin[plugin]->isValidWeather(p)) {          
            Serial.print(sens);               Serial.print(WeatherPlugin[plugin]->getString(p));      Serial.print(" ");
            Serial.print("ID: ");             Serial.print(WeatherPlugin[plugin]->getId(p));          Serial.print(" ");
            Serial.print("Channel: ");        Serial.print(WeatherPlugin[plugin]->getChannel(p));     Serial.print(" ");
            Serial.print("Battery: ");        Serial.print(WeatherPlugin[plugin]->getBattery(p));     Serial.print(" ");        
            Serial.print("Temperature: ");    Serial.print(WeatherPlugin[plugin]->getTemperature(p)); Serial.print(" ");
            Serial.print("Humidity: ");       Serial.print(WeatherPlugin[plugin]->getHumidity(p));    Serial.print("\n");
          }   
          p = 0;
        }
        prev_p = p;
      }
    }
  }

}
