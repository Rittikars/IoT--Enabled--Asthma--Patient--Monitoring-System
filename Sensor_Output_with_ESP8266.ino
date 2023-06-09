#include <SoftwareSerial.h> 
#include <Wire.h> 
#include <ESP8266WiFi.h> 
#include <WiFiClientSecure.h> 
#include <WiFiClient.h>; 
#include "Wire.h"
#include <OneWire.h> 
#include <DallasTemperature.h> 
#include "DHTesp.h" 
#include <GP2Y1010_DustSensor.h> 
#include <LiquidCrystal_I2C.h> 

/*wifi name & password*/
char ssid[] = "Your WiFi Name";
char pass[] = "Your WiFi Password ";



OneWire oneWire(ONE_WIRE_BUS); 
DallasTemperature sensors(&oneWire); 
DHTesp dht; 
GP2Y1010_DustSensor dustsensor; 

WiFiClient Client; 
LiquidCrystal_I2C lcd(0x27, 16, 2); 
WiFiClientSecure client;



float temp1; 
const int led_pin = 16; 
const int analog_pin = A0; 
int mq135 = D6;
const unsigned char Active_buzzer = 0; 

void setup() {
  
  
 pinMode(Active_buzzer, OUTPUT); 
 WiFi.begin(ssid, pass); 
 sensors.begin(); 
 dustsensor.begin(led_pin, analog_pin);
 dht.setup(14, DHTesp::DHT11);

  lcd.init(); 
  lcd.backlight();
  lcd.setCursor(0, 0); 
  lcd.print(" ROOM "); 
  lcd.setCursor(0, 1); 
  lcd.print(" ENVIRONMENT "); 

void sendTemps() {
  sensors.requestTemperatures(); 
  float temp1 = sensors.getTempCByIndex(0) * 1.8 + 32;
  Serial.println(temp1); 
}

void loop() {

  float temp1; 
  float bpm; 
  int spo2; 
  float dust = dustsensor.getDustDensity();  

  Serial.print("Dust Density: ");
  Serial.print(dust); 
  Serial.println(" ug/m3");

  if (dust < 35) {
    Serial.println("No Dust");
    digitalWrite(Active_buzzer, LOW); 
  } else {
    Serial.println("Dust Detected");
    digitalWrite(Active_buzzer, HIGH); 
  }

  temp1 = sensors.getTempCByIndex(0) * 1.8 + 32; 


  String msg = NodeMCU.readStringUntil('\r');  
   Serial.println(msg);

  /*1st part of the string (bpm)*/

  String bp_m = msg.substring(0, 5); 
  int bp__m = bp_m.toInt(); 
  Serial.println(bp__m); 

  /*2nd part of the string (spo2)*/
  String sp_o2 = msg.substring(6); 
  int sp__o2 = sp_o2.toInt(); 
  Serial.println(sp__o2); 

  float temperature = dht.getTemperature(); 
  float humidity = dht.getHumidity(); 

  Serial.print("Room Temperature (C): ");
  Serial.println(temperature); 

  Serial.print("Humidity (%): ");
  Serial.println(humidity); 


  int data = digitalRead(mq135); 

  if (data == 0) 
  {
    String dt = "polluted";
    digitalWrite(Active_buzzer, HIGH); 
    Serial.println(dt); 
  }

  else {
    String dt = "Good Environment";
    digitalWrite(Active_buzzer, LOW); 
    Serial.println(dt); 
  }

  lcd.clear(); 
  lcd.setCursor(0, 0); 

  if (dust < 35) {
    lcd.print("No Dust");
  } else {
    lcd.print("Dust Detected"); 
  lcd.setCursor(0, 1); 
 
  if (data == 1) 
  {
    lcd.print("No Harmful Gas");
  } else {
    lcd.print("Polluted by Gas"); 
  }

  byte error, address; 
  int I2CDevices; 
  Serial.println("Scanning for I2C Devicesâ€¦"); 

  I2CDevices = 0; //variable initialization
  
  for (address = 1; address < 127; address++) {
    Wire.beginTransmission(address);

    if (error == 0) {
      Serial.print("I2C device found at address 0x");
      if (address < 16) {
        Serial.print("0");
      }
      Serial.print(address, HEX);
      Serial.println(" !");

      I2CDevices++;
    } else if (error == 4) {
      Serial.print("Unknown error at address 0x");
      if (address < 16) {
        Serial.print("0");
      }
      Serial.println(address, HEX);
    }
  }
  if (I2CDevices == 0) {
    Serial.println("No I2C devices found\n");
  } else {
    Serial.println("****\n");
  }
  
}