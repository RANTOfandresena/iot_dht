#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <DHT.h>
#include "../config.h" 

DHT dht(DHTPIN, DHTTYPE);


WiFiClient espClient;
PubSubClient client(espClient);
unsigned long lastMsgTime = 0;
const unsigned long msgInterval = 500; 
bool mqttConnected = false;


void setupWiFi() {
  delay(10);
  Serial.println();
  Serial.print("Connexion au WiFi: ");
  Serial.println(WIFI_SSID);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi connecté");
  Serial.print("Adresse IP: ");
  Serial.println(WiFi.localIP());
}


void reconnectMQTT() {
  while (!client.connected()) {
    Serial.print("Tentative de connexion MQTT...");

    if (client.connect(MQTT_CLIENT_ID)) {
      Serial.println("connecté");
      mqttConnected = true;
    } else {
      Serial.print("échec, rc=");
      Serial.print(client.state());
      Serial.println(". Nouvelle tentative dans 5 secondes");
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  
  dht.begin();
  
  setupWiFi();
  
  client.setServer(MQTT_SERVER, MQTT_PORT);
  reconnectMQTT();
}

void loop() {
  if (!client.connected()) {
    mqttConnected = false;
    reconnectMQTT();
  }
  client.loop();

  unsigned long now = millis();
  if (now - lastMsgTime > msgInterval) {
    lastMsgTime = now;
    
    float humidity = dht.readHumidity();
    float temperature = dht.readTemperature();

    if (isnan(humidity) || isnan(temperature)) {
      Serial.println("Échec de lecture du capteur DHT!");
      return;
    }

    Serial.printf("Température: %.2f °C | Humidité: %.2f %%\n", temperature, humidity);

    char tempStr[10], humStr[10];
    dtostrf(temperature, 6, 2, tempStr);
    dtostrf(humidity, 6, 2, humStr);

    if (mqttConnected) {
      client.publish("dht_data/temperature", tempStr) ? Serial.println("Température publiée") : Serial.println("Erreur");
      client.publish("dht_data/humidite", humStr) ? Serial.println("Humidité publiée") : Serial.println("Erreur");
    }
  }
}
