#include <Arduino.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <HTTPClient.h>
#include <stdio.h>
#include <EEPROM.h>


const char* ssid = "HUAWEI P20 lite";                                                                 //WFi AP                                                 
const char* password = "jure1234";                                                                    //Password za Wifi AP
const char* serverName = "http://purs.a2hosted.com/esp-post-data.php";                                //HTTP POST
const char* serverName2 = "http://purs.a2hosted.com/esp-post-data.php?action=get_last_relay_value";   //HTTP GET


String apiKeyValue = "tPmAT5Ab3j7F9";           //API ključ koji mora biti jednak API ključu na serveru
String deviceLocation = "Radnicka";             //Lokacija - promjeniti ovisno o korisniku
String clientID = "Aspira";                     //Zakupac - promjeniti ovisno o korisniku
String deviceID = "A05901";                     //SN uređaja - promjeniti ovisno o korisniku

//GPIO postavke 
#define DHTPIN 27             //pin DHT11 senzora
#define DHTTYPE    DHT11      //tip senzora - DHT 11
#define MAIN_RELAY 26         //pin releja 
#define COFFEE_BTN 14         //pin tipke za kavu
#define CUP_BTN 13            //pin tipke za praznu šalicu
#define REFILL 12             //pin tipke za reset stanja nakon popune
        
#define EEPROM_SIZE 4
DHT dht(DHTPIN, DHTTYPE);

int coffeLvl = 5;            
int cupLvl = 5;              

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
//*****************KAVA*****************//
String coffeeStatus(){
  int x = 0;
  x = EEPROM.read(0);
  if (x<=15){
    return String("FULL");
  } else if (x>15 && x<20){
    return String("REFILL NEEDED");
  } else {
    return String("EMPTY");
  }
}
//*****************KAVA*****************//
//*****************ČAŠE*****************//
String cupStatus(){
  int y = 0;
  y = EEPROM.read(1);
  if (y<=20){
    return String("FULL");
  } else if (y>25 && y<30){
    return String("REFILL NEEDED");
  } else {
    return String("EMPTY");
  }
}
//*****************ČAŠE*****************//
//*****************GET*****************//
String httpGETLastRelayValue(const char* serverName2){
  HTTPClient http2;
  http2.begin(serverName2);
  int httpResponseCode1 = http2.GET();
  String payload = "{}"; 
  if(httpResponseCode1 > 0) {
    Serial.print("HTTP Response code GET: ");
    Serial.println(httpResponseCode1);
    payload = http2.getString();
  } else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode1);
  }
  http2.end();
  return payload;
}
//*****************GET*****************//
//*****************POST****************//
void sendPOST(){
  int a = 0;
  int b = 0;
    HTTPClient http;
  http.begin(serverName);
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");
  String httpRequestData = "api_key=" + apiKeyValue + "&sens=" + deviceID
                          + "&loc=" + deviceLocation + "&user=" + clientID + "&temp=" + readDHTTemperature()
                          + "&humid=" + readDHTHumidity() + "&coffee=" + coffeeStatus() + "&cup=" + cupStatus();

  Serial.print("httpRequestData: ");    //za potrebe debugginga i demonstracije
  Serial.println(httpRequestData);      //za potrebe debugginga i demonstracije
  
  a = EEPROM.read(0);
  b = EEPROM.read(1);
  Serial.println(a);
  Serial.println(b);
  
  int httpResponseCode = http.POST(httpRequestData); // POST zahtjev prema serveru. Odgovor servera se sprema u httpResponseCode 

  if (httpResponseCode>0) {
  Serial.print("HTTP Response code POST: "); // ispisuje status odgovora servera (200 je ok, 500 je problem)
  Serial.println(httpResponseCode);
  } else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode); // ispisuje grešku ako dođe do nje
  }
  http.end();
}
//*****************POST****************//



TaskHandle_t Task1;
TaskHandle_t Task2;


void Task_1( void * pvParameters) {
  int coffeeStat = 0;
  int cupStat = 0;
  int refil = 0;

  for(;;) {
    refil = digitalRead(REFILL);
    if(refil==HIGH){
          coffeLvl = 0;
          cupLvl = 0;
          EEPROM.write(0, coffeLvl);    //kave
          EEPROM.commit();
          EEPROM.write(1, cupLvl);      //čaše
          EEPROM.commit();
          Serial.println("REFILL - spremljeno");
          delay(100);
    } 
    
    coffeeStat = digitalRead(COFFEE_BTN);
    if(coffeeStat==HIGH){
          delay(200);
          coffeLvl++;
          cupLvl++;
          EEPROM.write(0, coffeLvl);
          EEPROM.commit();
          Serial.println("COFFEE - spremljeno");
          delay(200);
          if(coffeLvl>20){
            coffeLvl=20;}
    }    
   
    
    cupStat = digitalRead(CUP_BTN);
    if(cupStat==HIGH){
          delay(400);
          cupLvl++;
          EEPROM.write(1, cupLvl);
          EEPROM.commit();
          Serial.println("CUP - spremljeno");
          delay(200);
          if(cupLvl>30){
            cupLvl=30;}
    }
    }
}

void Task_2( void * pvParameters) {
  for(;;){
    if(WiFi.status()== WL_CONNECTED){
      sendPOST();
      String relayOutput = httpGETLastRelayValue(serverName2);
        EEPROM.write(2, relayOutput.toInt());
        EEPROM.commit();
        if(relayOutput.toInt()==0){
          digitalWrite(MAIN_RELAY, LOW);
        } else {
          digitalWrite(MAIN_RELAY, HIGH);
      }} else {
    Serial.println("WiFi Disconnected");
    }
    vTaskDelay(2000);
  }
}


void setup() {
  int relay_state = 0;
  Serial.begin(115200);
  dht.begin(); 
  EEPROM.begin(EEPROM_SIZE);
  WiFi.begin(ssid, password);

  pinMode(MAIN_RELAY, OUTPUT);          //postavlja pin releja
  pinMode(COFFEE_BTN, INPUT);           //postavlja pin za kavu
  pinMode(CUP_BTN, INPUT);              //postavlja pin za čaše
  pinMode(REFILL, INPUT);
  coffeLvl = EEPROM.read(0);
  cupLvl = EEPROM.read(1);
  relay_state = EEPROM.read(2);

  if(relay_state!=0){
    digitalWrite(MAIN_RELAY, HIGH);   // uključuje relej tj. uređaj kod uključenja
  } else {
    digitalWrite(MAIN_RELAY, LOW);
  }

  Serial.println("Spajam na Wifi...");
  while(WiFi.status() != WL_CONNECTED) { 
    delay(500);
    Serial.print("...");
  }
  Serial.println("");
  Serial.print("Spojen na WiFi sa IP adresom: ");
  Serial.println(WiFi.localIP());

  xTaskCreatePinnedToCore(Task_1, "Task1", 10000, NULL, 0, &Task1, 0);
  xTaskCreatePinnedToCore(Task_2, "Task2", 10000, NULL, 1, &Task2, 0);
  delay(500); 
}

void loop(){
  vTaskDelay(10000);
}









