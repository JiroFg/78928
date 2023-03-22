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
  /*String status = dht.getStatusString();
  float hum = dht.getHumidity();
  float tem = dht.getTemperature();
  float far = dht.toFahrenheit(tem);
  float cent = dht.computeHeatIndex(tem, hum, false);
  float cent2 = (dht.computeHeatIndex(dht.toFahrenheit(tem), hum, true));
  aux = postHigrometro(status, hum, tem, far, cent, cent2);*/
  server.on("/", HTTP_GET,[](AsyncWebServerRequest *request){
    //request->send(200,"text/html",aux);
    String status = dht.getStatusString();
    float hum = dht.getHumidity();
    float tem = dht.getTemperature();
    float far = dht.toFahrenheit(tem);
    float cent = dht.computeHeatIndex(tem, hum, false);
    float cent2 = (dht.computeHeatIndex(dht.toFahrenheit(tem), hum, true));
    aux = postHigrometro(status, hum, tem, far, cent, cent2);
    request->send(200,"text/html","<h1>Hola " + aux + "</h1>");
  });
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

String postHigrometro(String status,float hum, float tem, float far, float cent, float cent2){
  StaticJsonBuffer<300> jsonBuffer;
  JsonObject& JSONencoder = jsonBuffer.createObject();
  JSONencoder["temperatura"] = tem;
  JSONencoder["humedad"] = hum;

  char JSONmessageBuffer[256];
  JSONencoder.prettyPrintTo(JSONmessageBuffer, sizeof(JSONmessageBuffer));
  Serial.println("Estructura JSON:");
  Serial.println(String(JSONmessageBuffer));
  return String(JSONmessageBuffer);
}
