#ifndef SECRETS_H
#define SECRETS_H

// Configuration WiFi
const char* WIFI_SSID = "Wokwi-GUEST";
const char* WIFI_PASSWORD = "";

// Configuration MQTT
const char* MQTT_SERVER = "test.mosquitto.org";
const int MQTT_PORT = 1883;
const char* MQTT_CLIENT_ID = "ESP32_DHT_Sensor";

// Configuration du capteur DHT
#define DHTPIN 4  
#define DHTTYPE DHT22 

#endif 
