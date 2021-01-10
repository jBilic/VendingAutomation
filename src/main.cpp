#include <Arduino.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <Arduino_JSON.h>
#include "Arduino_JSON.h"
#include <HTTPClient.h>

//WiFi login i link na server tj. PHP skriptu
const char* ssid = "HUAWEI P20 lite";
const char* password = "jure1234";
const char* serverName = "http://purs.a2hosted.com/esp-post-data.php";
const char* serverName2 = "http://purs.a2hosted.com/esp-post-data.php/?action=get_last_relay_value"; //upitno

//Podaci o uređaju i API ključ koji mora biti jednak API ključu na serveru
String apiKeyValue = "tPmAT5Ab3j7F9";
String deviceLocation = "Radnicka";
String clientID = "Aspira";
String deviceID = "A001";

//GPIO postavke 
#define DHTPIN 27 //pin DHT11 senzora
#define DHTTYPE    DHT11     // odabir vrste senzora - DHT 11
DHT dht(DHTPIN, DHTTYPE);
#define RELAY_ON 1
#define RELAY_OFF 0
#define MAIN_RELAY 26 //pin releja 
#define COFFEE_BTN 14
#define CUP_BTN 12
#define REFILL 16 // nakon popune uređaja, aktivira se tipkalo spojeno na ovaj pin i on resetira brojače (coffeeLvl, cupLvl)

int coffeLvl = 10; // inicijalni status broja kava koje uređaj može napraviti. Reset ide preko gore navedenog pina 16 (REFILL)
int cupLvl = 20; // incijalni broj čaša u uređaju. Reset ide preko gore navedenog pina 16 (REFILL)



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
  if (coffeLvl>=5){
    return String("FULL");
  } else if (coffeLvl>0 && coffeLvl<5){
    return String("REFILL NEEDED");
  } else {
    return String("EMPTY");
  }
}
//*****************KAVA*****************//

//*****************ČAŠE*****************//
String cupStatus(){
  if (cupLvl>=10){
    return String("FULL");
  } else if (cupLvl>5 && cupLvl<10){
    return String("REFILL NEEDED");
  } else {
    return String("EMPTY");
  }
}
//*****************ČAŠE*****************//

// statusCounter je pokušaj neke funkcije koje bi trebala vršiti brojanje artikala u uređaju
void statusCounter(){
  if(digitalRead(CUP_BTN==HIGH)){
    cupLvl--;
  } 
  if (digitalRead(COFFEE_BTN==HIGH)){
    coffeLvl--;
  }
}

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




// setup je standardni dio Arduino sintakse i on postavlja inicijalne parametre programa - vrsta pinova, brzina serijske komunikacije itd.
void setup() {
  Serial.begin(115200);
  dht.begin(); // započinje čitanje podataka sa DHT senzora
  WiFi.begin(ssid, password); // započinje spajanje na WiFI
  pinMode(COFFEE_BTN, INPUT);
  pinMode(CUP_BTN, INPUT);
  pinMode(MAIN_RELAY, OUTPUT);
  digitalWrite(MAIN_RELAY, RELAY_ON); // uključuje relej tj. sami uređaj
  
  // ispisuje status spajanja na monitor
  Serial.println("Spajam na Wifi...");
  while(WiFi.status() != WL_CONNECTED) { 
    delay(500);
    Serial.print("...");
  }
  Serial.println("");
  Serial.print("Spojen na WiFi sa IP adresom: ");
  Serial.println(WiFi.localIP());
}

// loop je standardni dio Arduino sintakse i u njemu se vrti glavni program
void loop() {
    //statusCounter(); // poziv funkcije brojača - funkcija NE RADI tj ne reagira na tipkalo

    // donji dio se izvršava dok je uređaj spojen na mrežu - šalju se svi podaci na server preko POST funkcije
    // potrebno je uvrsitit i GET funkciju koja povlači status releja sa mreže te uključuje/isključuje relej ovisno o statusu
    if(WiFi.status()== WL_CONNECTED){
      HTTPClient http;

      http.begin(serverName);
      http.addHeader("Content-Type", "application/x-www-form-urlencoded");

      String httpRequestData = "api_key=" + apiKeyValue + "&sens=" + deviceID
                          + "&loc=" + deviceLocation + "&usex=" + clientID + "&temp=" + readDHTTemperature()
                          + "&humid=" + readDHTHumidity() + "&coffee=" + coffeeStatus() + "&cup=" + cupStatus();

      Serial.print("httpRequestData: ");
      Serial.println(httpRequestData);
      Serial.println(coffeLvl);
      Serial.println(cupLvl);

      int httpResponseCode = http.POST(httpRequestData); // POST zahtjev prema serveru. Odgovor servera se sprema u httpResponseCode 

      if (httpResponseCode>0) {
        Serial.print("HTTP Response code POST: "); // ispisuje status odgovora servera (200 je ok, 500 je problem)
        Serial.println(httpResponseCode);
      }
      else {
        Serial.print("Error code: ");
        Serial.println(httpResponseCode); // ispisuje grešku ako dođe do nje
      }
      http.end(); // prekida komunikaciju kako bi se oslobodili resursi
      delay(2000); // čeka 2 sekunde prije novog slanja

            

      String relayOutput = httpGETLastRelayValue(serverName2);
      Serial.print("Relay status: ");
      Serial.println(relayOutput);
      /*
      JSONVar relayOutputJson = JSON.parse(relayOutput);
  
      if (JSON.typeof(relayOutputJson) == "undefined") {
      Serial.println("Parsing input failed!");
      }

      Serial.print("JSON object = ");
      Serial.println(relayOutputJson);
      
      int relayValue = relayOutputJson["Relay_Status"];
      */
      if(relayOutput==0){
        digitalWrite(MAIN_RELAY, RELAY_OFF);
      } else {
        digitalWrite(MAIN_RELAY, RELAY_ON);
      }
    }
    else {
      Serial.println("WiFi Disconnected");
    }
  }









