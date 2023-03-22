#include <WiFi.h>
#include <ESPAsyncWebSrv.h>
#include <DHTesp.h>

#define DHTpin 15

const char* ssid= "jirofg";
const char* password = "Flokiymou125";
AsyncWebServer server(80);
DHTesp dht;

void setup(){
  Serial.begin(115200);
  Serial.println();
  Serial.println("Estado\tHumedad (%)\tTemperatura (C)\t(F)\tIndiceCalor (C)\t(F)");
  dht.setup(DHTpin, DHTesp::DHT11);
  conectarse();
  server.on("/", HTTP_GET,[](AsyncWebServerRequest *request){
    int numParametros =request->params();
    float h = dht.getHumidity();
    float t = dht.getTemperature();
    String status = dht.getStatusString();
    float f = dht.toFahrenheit(t);
    float c = dht.computeHeatIndex(t, h, false);
    float c2 = (dht.computeHeatIndex(dht.toFahrenheit(t), h, true));
    if(numParametros == 0)
      if (isnan(h) || isnan(t)){
        request->send(200,"text/html","<h1> No se pudo leer sensor DHT </h1>");
      }else{
        request->send(200,"text/html","<table class='default'><tr><th>Estado</th><th>Humedad (%)</th><th>Temperatura(C)</th><th>(F)</th><th>IndiceCalor (C)</th><th>(F)</th></tr><tr><td>"+status+"</td><td>"+String(h)+"</td><td>"+String(t)+"</td><td>"+String(f)+"</td><td>"+String(c)+"</td><td>"+String(c2)+"</td></tr></table>");
      }
    else{
      AsyncWebParameter *p=request->getParam(0);
      String html="<H1> hola " + p->value() + "desde ESP32</H1>";
      request->send(200,"text/html",html);
    }
  });
  server.begin();

}

void loop(){
  delay(dht.getMinimumSamplingPeriod());
  float humedad =  dht.getHumidity();
  float temperatura = dht.getTemperature();
  if (isnan(humedad) || isnan(temperatura)){
    Serial.println("No se pudo leer sensor DHT!");
    return;
  }

  Serial.print( dht.getStatusString() );
  Serial.print("\t");
  Serial.print(humedad, 1);
  Serial.print("\t\t");
  Serial.print(temperatura, 1);
  Serial.print("\t\t");
  Serial.print( dht.toFahrenheit(temperatura), 1);
  Serial.print("\t\t");
  Serial.print( dht.computeHeatIndex(temperatura, humedad, false), 1);
  Serial.print("\t\t");
  Serial.println(dht.computeHeatIndex(dht.toFahrenheit(temperatura), humedad, true), 1);
  delay(2000);
}

void conectarse(){
  Serial.print("Conectando");Serial.println(ssid);
  WiFi.begin(ssid,password);
  while(WiFi.status()!=WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }
  Serial.print(WiFi.localIP());
}