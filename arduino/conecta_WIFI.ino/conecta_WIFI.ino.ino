#include <WiFi.h>

const char* ssid = "dr-rojano";
const char* password = "78928-tw";

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  delay(10);
  //se inicia la conexión
  Serial.println();
  Serial.println();
  Serial.println("Connecting to");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while(WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address:");
  Serial.println(WiFi.localIP());
}

void loop() {
  // put your main code here, to run repeatedly:

}
