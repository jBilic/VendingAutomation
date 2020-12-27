#include <Arduino.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <Arduino_JSON.h>
#include "Arduino_JSON.h"
#include <HTTPClient.h>

//WiFi login i lokalni server
const char* ssid = "HUAWEI P20 lite";
const char* password = "jure1234";
AsyncWebServer server(80);
const char* serverName = "http://bilateks-fidus.hr/esp-post-data.php";

//Podaci o uređaju
String apiKeyValue = "tPmAT5Ab3j7F9";
String deviceLocation = "Radnička 282";
String clientID = "Aspira d.o.o";
String deviceID = "A0012G56KLM";

//GPIO postavke, postavke senzora i releja
#define DHTPIN 27 //pin DHT11 senzora
#define DHTTYPE    DHT11     // odabir vrste senzora - DHT 11
DHT dht(DHTPIN, DHTTYPE);

#define RELAY_ON 1
#define RELAY_OFF 0
#define MAIN_RELAY 26 //pin releja 
int relay_state = 1;

const int switch_pin = 14; //pin prekidača

const long interval = 5000;
unsigned long previousMillis = 0;
String outputsState;

//**************TEMPERATURA**************//
String readDHTTemperature() {
  float t = dht.readTemperature();
  if (isnan(t)) { 
    Serial.println("Neuspješno čitanje podataka sa DHT senzora!");
    return "--";
  }
  else {
    Serial.println(t);
    return String(t);
  }
}
//**************TEMPERATURA**************//
//*****************VLAGA*****************//
String readDHTHumidity() {
  float h = dht.readHumidity();
  if (isnan(h)) {
    Serial.println("Neuspješno čitanje podataka sa DHT senzora!");
    return "--";
  }
  else {
    Serial.println(h);
    return String(h);
  }
}
//*****************VLAGA*****************//
//*****************STOCK*****************//
String stockStatus(){
  if(digitalRead(switch_pin) == 0){
    return String("EMPTY");
  }
  else {
    return String("FULL");
  }
}

//*****************STOCK*****************//
/* RELEJ
String relayState(int numRelay){
  if(RELAY_NO){
    if(digitalRead(relay_pin[numRelay-1])){
      return "";
    }
    else {
      return "checked";
    }
  }
  else {
    if(digitalRead(relay_pin[numRelay-1])){
      return "checked";
    }
    else {
      return "";
    }
  }
  return "";
}
*/

String httpGETRequest(const char* serverName) {
  WiFiClient client;
  HTTPClient http;
    
  // Your IP address with path or Domain name with URL path 
  http.begin(client, serverName);
  
  // Send HTTP POST request
  int httpResponseCode = http.GET();
  
  String payload = "{}"; 
  
  if (httpResponseCode>0) {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    payload = http.getString();
  }
  else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }
  // Free resources
  http.end();

  return payload;
}




void setup() {
  Serial.begin(115200);
  dht.begin();
  WiFi.begin(ssid, password);
  pinMode(MAIN_RELAY, OUTPUT);  
  digitalWrite(MAIN_RELAY, RELAY_ON);
  
  Serial.println("Spajam na Wifi...");
  while(WiFi.status() != WL_CONNECTED) { 
    delay(500);
    Serial.print("...");
  }
  Serial.println("");
  Serial.print("Spojen na WiFi sa IP adresom: ");
  Serial.println(WiFi.localIP());
}

void loop(){
  //Check WiFi connection status
  if(WiFi.status()== WL_CONNECTED){

    HTTPClient http;
    http.begin(serverName); // Your Domain name with URL path or IP address with path
    http.addHeader("Content-Type", "application/x-www-form-urlencoded"); // Specify content-type header
    
    // Prepare your HTTP POST request data
    String httpRequestData = "api_key=" + apiKeyValue + "&devId=" + deviceID
                          + "&locationId=" + deviceLocation + "&userId=" + clientID + "&temp=" + readDHTTemperature()
                          + "&humidity=" + readDHTHumidity() + "&supp1" + String(stockStatus());
  
    int httpResponseCode = http.POST(httpRequestData); // Send HTTP POST request
    
    outputsState = httpGETRequest(serverName);
    Serial.println(outputsState);
    JSONVar myObject = JSON.parse(outputsState);

    if (JSON.typeof(myObject) == "undefined") {
      Serial.println("Parsing input failed!");
      return;
    }
    JSONVar keys = myObject.keys();
    
    for (int i = 0; i < keys.length(); i++) {
      JSONVar value = myObject[keys[i]];
      digitalWrite(MAIN_RELAY, atoi(value));
    }
    
    http.end();
  }
  else {
    Serial.println("WiFi Disconnected");
  }
  //Podaci se šalju svakih 15 sekundi
  delay(15000);  
}


