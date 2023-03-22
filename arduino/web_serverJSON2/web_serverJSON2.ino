#include <WiFi.h>
#include <ESPAsyncWebSrv.h>
#include <ArduinoJson.h>
#include <DHTesp.h>

#define DHTpin 15

const char* ssid= "jirofg";
const char* password = "Flokiymou125";
AsyncWebServer server(80);
DHTesp dht;
String aux;

void setup() {
  Serial.begin(115200);
  dht.setup(DHTpin, DHTesp::DHT11);
  conectarse();
  
  server.on("/", HTTP_GET,[](AsyncWebServerRequest *request){
    AsyncResponseStream *response = request->beginResponseStream("application/json");
    DynamicJsonDocument json(1024);
    json["status"] = dht.getStatusString();
    json["humedad"] = dht.getHumidity();
    json["temperatura"] = dht.getTemperature();
    json["fahrenheit"] = dht.toFahrenheit(dht.getTemperature());
    json["centigrados"] = dht.computeHeatIndex(dht.getTemperature(), dht.getHumidity(), false);
    json["centigrados2"] = (dht.computeHeatIndex(dht.toFahrenheit(dht.getTemperature()), dht.getHumidity(), true));
    serializeJson(json, *response);
    request->send(response);
  });
  DefaultHeaders::Instance().addHeader("Access-Control-Allow-Origin", "*");
  server.begin();
}

void loop(){
}

void conectarse(){
  Serial.print("Conectando");Serial.println(ssid);
  WiFi.begin(ssid,password);
  while(WiFi.status()!=WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }
  Serial.println(WiFi.localIP());
}
