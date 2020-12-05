#include <Arduino.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <Arduino_JSON.h>
#include "Arduino_JSON.h"
#include <HTTPClient.h>

//WiFi login i lokalni server
const char* ssid = "xxxxx";
const char* password = "xxxxx";
AsyncWebServer server(80);
const char* serverName = "xxxxx";
String apiKeyValue = "tPmAT5Ab3j7F9";
String statusNone = "N/A";

//GPIO postavke, postavke senzora i releja
#define DHTPIN 27 //pin DHT11 senzora
#define DHTTYPE    DHT11     // odabir vrste senzora - DHT 11
#define RELAY_NO    true
#define NUM_RELAYS  1
DHT dht(DHTPIN, DHTTYPE);
String deviceLocation = "Radnička 111";
String clientID = "Aspira d.o.o";
String deviceID = "A0012G56KLM";

const int switch_pin = 14; //pin prekidača
bool sw_stat = false; //status prekidača

int relay_pin[NUM_RELAYS] = {26}; //pinovi releja
const char* PARAM_INPUT_1 = "relay";
const char* PARAM_INPUT_2 = "state";

const long interval = 5000;
unsigned long previousMillis = 0;
String outputsState;

//***************WEB***************//

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <link rel="stylesheet" href="https://use.fontawesome.com/releases/v5.7.2/css/all.css" integrity="sha384-fnmOCqbTlWIlj8LyTjo7mOUStjsKC4pOpQbqyi7RrhN7udi9RwhKkMHpvLbHG9Sr" crossorigin="anonymous">
  <meta charset="UTF-8">
  <style>
    html {
     font-family: Arial;
     display: inline-block;
     margin: 0px auto;
     text-align: center;
    }
    h2 { font-size: 3.0rem; }
    p { font-size: 2.0rem; }
    .units { font-size: 1rem; }
    .dht-labels{
      font-size: 1rem;
      vertical-align:middle;
      padding-bottom: 15px;
    }
    body {max-width: 600px; margin:0px auto; padding-bottom: 25px;}
    .switch {position: relative; display: inline-block; width: 120px; height: 68px} 
    .switch input {display: none}
    .slider {position: absolute; top: 0; left: 0; right: 0; bottom: 0; background-color: #ccc; border-radius: 34px}
    .slider:before {position: absolute; content: ""; height: 52px; width: 52px; left: 8px; bottom: 8px; background-color: #fff; -webkit-transition: .4s; transition: .4s; border-radius: 68px}
    input:checked+.slider {background-color: #e6160b}
    input:checked+.slider:before {-webkit-transform: translateX(52px); -ms-transform: translateX(52px); transform: translateX(52px)}
  </style>
</head>
<body>
  <h2>Vending Machine Node</h2>
  <p>
    <i class="fas fa-temperature-low" style="color:#f2aa00;"></i> 
    <span class="dht-labels">Temperatura: </span> 
    <span id="temperature">%TEMPERATURE%</span>
    <sup class="units">&deg;C</sup>
  </p>
  <p>
    <i class="fas fa-tint" style="color:#4287f5;"></i> 
    <span class="dht-labels">Vlaga: </span>
    <span id="humidity">%HUMIDITY%</span>
    <sup class="units">&percnt;</sup>
  </p>
  <p>
    <i class="fas fa-layer-group" style="color:#914103;"></i> 
    <span class="dht-labels">Twix Candy bar: </span>
    <span id="stock">%STOCK%</span>
  </p>
    %BUTTONPLACEHOLDER%
<script>function toggleCheckbox(element) {
  var xhr = new XMLHttpRequest();
  if(element.checked){ xhr.open("GET", "/update?relay="+element.id+"&state=1", true); }
  else { xhr.open("GET", "/update?relay="+element.id+"&state=0", true); }
  xhr.send();
}</script>
</body>
<script>
setInterval(function ( ) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("temperature").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "/temperature", true);
  xhttp.send();
}, 1000 ) ;

setInterval(function ( ) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("humidity").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "/humidity", true);
  xhttp.send();
}, 1000 ) ;

setInterval(function ( ) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("stock").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "/stock", true);
  xhttp.send();
}, 1000 ) ;

</script>
</html>)rawliteral";



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
    return "EMPTY";
  }
  else {
    return "FULL";
  }
}
//*****************STOCK*****************//
//*****************RELEJ*****************//
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
//*****************RELEJ*****************//



String processor(const String& var){
  //Serial.println(var);
  if(var == "TEMPERATURE"){
    return readDHTTemperature();
  }
  else if(var == "HUMIDITY"){
    return readDHTHumidity();
  }
  else if(var == "STOCK"){
    return stockStatus();
  }
  else if(var == "BUTTONPLACEHOLDER"){
    String buttons ="";
    for(int i=1; i<=NUM_RELAYS; i++){
      String relayStateValue = relayState(i);
      buttons+= "<h4>Uključivanje i isključivanje uređaja</h4><label class=\"switch\"><input type=\"checkbox\" onchange=\"toggleCheckbox(this)\" id=\"" + String(i) + "\" "+ relayStateValue +"><span class=\"slider\"></span></label>";
    }
    return buttons;
  }
  return String();
}


void setup() {
  Serial.begin(115200);
  dht.begin();
  WiFi.begin(ssid, password);
  for(int i=1; i<=NUM_RELAYS; i++){
    pinMode(relay_pin[i-1], OUTPUT);
    if(RELAY_NO){
      digitalWrite(relay_pin[i-1], HIGH);
    }
    else{
      digitalWrite(relay_pin[i-1], LOW);
    }
  }
  Serial.println("Spajam na Wifi...");
  while(WiFi.status() != WL_CONNECTED) { 
    delay(500);
    Serial.print("...");
  }
  Serial.println("");
  Serial.print("Spojen na WiFi sa IP adresom: ");
  Serial.println(WiFi.localIP());



  // Lokalni web
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html, processor);
  });
  server.on("/temperature", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", readDHTTemperature().c_str());
  });
  server.on("/humidity", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", readDHTHumidity().c_str());
  });
  server.on("/update", HTTP_GET, [] (AsyncWebServerRequest *request) {
    String inputMessage;
    String inputParam;
    String inputMessage2;
    String inputParam2;
    // GET input1 value on <ESP_IP>/update?relay=<inputMessage>
    if (request->hasParam(PARAM_INPUT_1) & request->hasParam(PARAM_INPUT_2)) {
      inputMessage = request->getParam(PARAM_INPUT_1)->value();
      inputParam = PARAM_INPUT_1;
      inputMessage2 = request->getParam(PARAM_INPUT_2)->value();
      inputParam2 = PARAM_INPUT_2;
      if(RELAY_NO){
        Serial.print("NO ");
        digitalWrite(relay_pin[inputMessage.toInt()-1], !inputMessage2.toInt());
      }
      else{
        Serial.print("NC ");
        digitalWrite(relay_pin[inputMessage.toInt()-1], inputMessage2.toInt());
      }
    }
    else {
      inputMessage = "No message sent";
      inputParam = "none";
    }
    Serial.println(inputMessage + inputMessage2);
    request->send(200, "text/plain", "OK");
  });

  // Start server
  server.begin();
  }

void loop(){

//Check WiFi connection status
  if(WiFi.status()== WL_CONNECTED){
    HTTPClient http;
    
    // Your Domain name with URL path or IP address with path
    http.begin(serverName);
    
    // Specify content-type header
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");
    
    // Prepare your HTTP POST request data
    String httpRequestData = "api_key=" + apiKeyValue + "&devId=" + deviceID
                          + "&locationId=" + deviceLocation + "&userId=" + clientID + "&temp=" + readDHTTemperature()
                          + "&humidity=" + readDHTHumidity() + "&supp1" + String(stockStatus()) + "&supp2" + statusNone 
                          + "&supp3" + statusNone + "&supp4" + statusNone;
    Serial.print("httpRequestData: ");
    Serial.println(httpRequestData);
    
    // You can comment the httpRequestData variable above
    // then, use the httpRequestData variable below (for testing purposes without the BME280 sensor)
    //String httpRequestData = "api_key=tPmAT5Ab3j7F9&sensor=BME280&location=Office&value1=24.75&value2=49.54&value3=1005.14";

    // Send HTTP POST request
    int httpResponseCode = http.POST(httpRequestData);
     
    // If you need an HTTP request with a content type: text/plain
    //http.addHeader("Content-Type", "text/plain");
    //int httpResponseCode = http.POST("Hello, World!");
    
    // If you need an HTTP request with a content type: application/json, use the following:
    //http.addHeader("Content-Type", "application/json");
    //int httpResponseCode = http.POST("{\"value1\":\"19\",\"value2\":\"67\",\"value3\":\"78\"}");
        
    if (httpResponseCode>0) {
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
    }
    else {
      Serial.print("Error code: ");
      Serial.println(httpResponseCode);
    }
    http.end();
  }
  else {
    Serial.println("WiFi Disconnected");
  }
  //Podaci se šalju svakih 15 sekundi
  delay(15000);  
}



