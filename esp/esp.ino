#include <WiFi.h>
#include <PubSubClient.h>
#include <SoftwareSerial.h>
#include <esp.h>

const char[] ssid = SSID;
const char[] password = PASS;

const char[] mqtt_broker = MQTT_BROKER;
const char[] mqtt_topic = MQTT_TOPIC;
const char[] mqtt_username = MQTT_USERNAME;
const char[] mqtt_password = MQTT_PASSWORD;
const int mqtt_port = MQTT_PORT;

WiFiClient espClient;
PubSubClient mqtt_client(espClient);

SoftwareSerial mySUART(4, 5);

void connectToWiFi();

void connectToMQTTBroker();

void mqttCallback(char *topic, byte *payload, unsigned int length);

void setup() {
  Serial.begin(9600);
  mySUART.begin(9600);
  connectToWiFi();
  mqtt_client.setServer(mqtt_broker, mqtt_port);
  mqtt_client.setCallback(mqttCallback);
  connectToMQTTBroker();
}

void connectToWiFi() {
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
}

void connectToMQTTBroker() {
  while (!mqtt_client.connected()) {
    String client_id = "esp8266-client-" + String(WiFi.macAddress());
    if (mqtt_client.connect(client_id.c_str(), mqtt_username, mqtt_password)) {
      mqtt_client.subscribe(mqtt_topic);
      mqtt_client.publish(mqtt_topic, "Hi EMQX I'm ESP8266 ^^");
     } else {
      delay(5000);
     }
   }
}

void mqttCallback(char *topic, byte *payload, unsigned int length) {
  for (unsigned int i = 0; i < length; i++) {
    mySUART.write((char) payload[i]);
  }
}

void loop() {
  if (!mqtt_client.connected()) {
    connectToMQTTBroker();
   }
  mqtt_client.loop();
}
