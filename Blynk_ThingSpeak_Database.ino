/*blynk credentials*/
#define BLYNK_TEMPLATE_ID "Template Id"
#define BLYNK_DEVICE_NAME "Project Name"
#define BLYNK_AUTH_TOKEN "Your Authentication Token"
#define BLYNK_PRINT Serial

char auth[] = "your blynk authentication token "; 

#include <ESP8266WiFi.h> 
#include <WiFiClientSecure.h>
#include <Blynk.h> 
#include <BlynkSimpleEsp8266.h> 
#include <ThingSpeak.h>; 
# include <WiFiClient.h>;

/*wifi name & password*/
char ssid[] = "Your WiFi Name";
char pass[] = "Your WiFi Password ";


/*Thingspeak and Google Spreadsheet credentials*/
unsigned long myChannelNumber “Your Thingspeak Channel Number”;
const char* myWriteAPIKey = " Thingspeak Write API Key”;
const char* host = " host website for spreadsheet"; 
const int httpsPort = “default port for https protocol”
String GAS_ID = "Your spreadsheet script ID”;




void setup() {
  
  WiFi.begin(ssid, pass);
  sensors.begin(); 
  dustsensor.begin(led_pin, analog_pin); 
  Blynk.begin(auth, ssid, pass); 
  ThingSpeak.begin(Client);

void sendTemps() {
  sensors.requestTemperatures();
  Blynk.virtualWrite(V1, temp1); 

}

void loop() {

  Blynk.run(); 
  timer.run(); 

  Blynk.virtualWrite(V7, dirt); //Update dust sensor value to blynk
  ThingSpeak.writeField(myChannelNumber, 6, dust, myWriteAPIKey); 

 
  ThingSpeak.writeField(myChannelNumber, 1, temp1, myWriteAPIKey); 

  String msg = NodeMCU.readStringUntil('\r');
  Serial.println(msg); 


  Blynk.virtualWrite(V2, bp__m); 
  ThingSpeak.writeField(myChannelNumber, 2, bp__m, myWriteAPIKey); 



  Blynk.virtualWrite(V3, sp__o2); 
  ThingSpeak.writeField(myChannelNumber, 3, sp__o2, myWriteAPIKey); 



  Blynk.virtualWrite(V4, temperature); 
  ThingSpeak.writeField(myChannelNumber, 4, temperature, myWriteAPIKey); 

  Blynk.virtualWrite(V5, humidity); 
  ThingSpeak.writeField(myChannelNumber, 5, humidity, myWriteAPIKey); 

  Blynk.virtualWrite(V6, data);

  sendData(temp1, bp__m, sp__o2, temperature, humidity, dust); 
}

/*description of sendData function*/
void sendData(float temp, float bpm, int spo2, float roomt, int humi, float dus) 

{
  Serial.println("==========");
  Serial.print("connecting to ");
  Serial.println(host);
  if (!client.connect(host, httpsPort)) //check if connection can be made
  {
    Serial.println("connection failed");
    return;
  }
  String string_temperature = String(temp);
  String string_BPM = String(bpm, DEC);
  String string_SPO2 = String(spo2, DEC);
  String string_RT = String(roomt, 2); 
  String string_HM = String(humi, DEC);
  String string_DD = String(dus, 2); 

  String url = "/macros/s/" + GAS_ID + "/exec?temperature=" + string_temperature + "&bpm=" + string_BPM + "&spo2=" + string_SPO2 + "&roomtemp=" + string_RT + "&humidity=" + string_HM + "&airquality=" + string_DD;
  Serial.print("requesting URL: ");
  Serial.println(url);
  client.print(String("GET ") + url + " HTTP/1.1\r\n" + "Host: " + host + "\r\n" + "User-Agent: BuildFailureDetectorESP8266\r\n" + "Connection: close\r\n\r\n");
  
  while (client.connected()) {
    String line = client.readStringUntil('\n');
    if (line == "\r") {
      Serial.println("headers received"); 
      break;
    }
  }
  String line = client.readStringUntil('\n');
  if (line.startsWith("{\"state\":\"success\"")) {
    Serial.println("esp8266/Arduino CI successfull!");
  } else {
    Serial.println("esp8266/Arduino CI has failed");
  }
  Serial.print("reply was : ");
  Serial.println(line);
  Serial.println("closing connection");
  Serial.println("==========");
  Serial.println();
  
}