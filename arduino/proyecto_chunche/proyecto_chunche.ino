#include <WiFi.h>
#include <PubSubClient.h>
#include "DHTesp.h"

#define DHTpin 15
DHTesp dht;

//comando para ejecutar mosquitto
//docker run -it --rm -p 1883:1883 -p 9001:9001 -v C:\dirección de tu archivo mosquitto\mosquitto:/mosquitto/config/mosquitto.conf eclipse-mosquitto

const char* ssid= "jirofg";
const char* password = "12345678";
const char* mqtt_server = "192.168.50.242";

WiFiClient espClient;
PubSubClient client(espClient);
unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE	(50)
char msg[MSG_BUFFER_SIZE];
int value = 0;
char classroom = '2';

void setup_wifi() {

  delay(10);
  Serial.print("Connecting");
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  randomSeed(micros());
  Serial.println("WiFi connected");
  
  Serial.println("IP address: ");
  Serial.print(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
  //se comprueba si los valores del payload son los que indican que este ESP32 abra la puerta
  //si el valor de la posición cero es igual al salon y el valor de la posición dos es igual a uno entonces la abre
  if ((char)payload[0] == classroom && (char)payload[2] == '1') {
    //ABRIR PUERTA AQUI
    digitalWrite(2, HIGH); 
  //por el contrario si es el salon pero el valor es cero entonces cierra la puerta
  } else if ((char)payload[0] == classroom && (char)payload[2] == '0'){
    //CERRAR PUERTA AQUI
    digitalWrite(2, LOW);
  } else {
    Serial.println("Valor invalido");
  }
}

void reconnect() {
  while (!client.connected()) {
    Serial.println("Attempting MQTT connection...");
    String clientId = "ESP8266Client-eaha-";
    clientId += String(random(0xffff), HEX);
    
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      client.subscribe("Classrooms");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void setup() {
  dht.setup(DHTpin, DHTesp::DHT11);
  pinMode(2, OUTPUT);
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}