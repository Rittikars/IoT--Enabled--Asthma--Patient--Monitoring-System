#include <Wire.h>                    
#include "MAX30100_PulseOximeter.h"  

#define REPORTING_PERIOD_MS 1000  

PulseOximeter pox;          
uint32_t tsLastReport = 0;  

void setup() {

  Serial.print("Initializing pulse oximeter..");
  if (!pox.begin()) {
    Serial.println("FAILED");
  } else {
    Serial.println("SUCCESS");
  }
  }
void loop() {
  int bpm = pox.getHeartRate(); 
  int spo2 = pox.getSpO2(); 
  pox.update();
  
  if (millis() - tsLastReport > REPORTING_PERIOD_MS) {
    Serial.print(pox.getHeartRate()); 
    Serial.println(pox.getSpO2());   
    tsLastReport = millis();  
  }
}