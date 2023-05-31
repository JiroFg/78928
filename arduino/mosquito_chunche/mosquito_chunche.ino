#include <WiFi.h>
#include <PubSubClient.h>

#include <ESPAsyncWebSrv.h>
#include <DHTesp.h>
#include <ArduinoJson.h>

#define DHTpin 15
DHTesp dht;

// Update these with values suitable for your network.

const char* ssid = "jirofg";
const char* password = "Flokiymou125";
const char* mqtt_server = "http://ip172-18-0-104-chahsdg1k7jg009utb40-9001.direct.labs.play-with-docker.com";

WiFiClient espClient;
PubSubClient client(espClient);
unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE	(50)
char msg[MSG_BUFFER_SIZE];
int value = 0;

void setup_wifi() {

  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  // Switch on the LED if an 1 was received as first character
  if ((char)payload[0] == '1') {
    digitalWrite(2, LOW);   // Turn the LED on (Note that LOW is the voltage level
    // but actually the LED is on; this is because
    // it is active low on the ESP-01)
  } else {
    digitalWrite(2, HIGH);  // Turn the LED off by making the voltage HIGH
  }

}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "Chunche-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish("outTopic", "Hola");
      // ... and resubscribe
      client.subscribe("inTopic");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup() {
  dht.setup(DHTpin, DHTesp::DHT11);
  
  pinMode(2, OUTPUT);     // Initialize the BUILTIN_LED pin as an output
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

  //  Recurso al que se accede
    char Json_string[256];
    DynamicJsonDocument json(1024);
    json["status"] = dht.getStatusString();
    json["humedad"] = dht.getHumidity();
    json["temperatura"] = dht.getTemperature();
    json["farenheit"] = dht.toFahrenheit(dht.getTemperature());
    json["sensacion"] = dht.computeHeatIndex(dht.getTemperature(), dht.getHumidity(), false);
    json["sensacion2"] = (dht.computeHeatIndex(dht.toFahrenheit(dht.getTemperature()), dht.getHumidity(), true));
    serializeJson(json, Json_string);
    // request->send(response);



  unsigned long now = millis();
  if (now - lastMsg > 2000) {
    lastMsg = now;
    ++value;
    snprintf (Json_string, MSG_BUFFER_SIZE, Json_string, value);
    Serial.print("Publish message: ");
    Serial.println(Json_string);
    client.publish("Mela chupas", Json_string);
  }
}
