#include <WiFi.h>
#include <PubSubClient.h>
#include "DHT.h"
//RELLENAR DATOS AQUI
// --- DATOS WIFI Y MQTT ---
const char* ssid = "";
const char* password = "";
const char* mqtt_server = ""; // Ej: "192.168.11.19"

// --- CONFIGURACIÓN HARDWARE ---
#define DHTPIN 4
#define DHTTYPE DHT22
#define MQ_PIN 34
const float DIVISOR_FACTOR = 1.5;

DHT dht(DHTPIN, DHTTYPE);
WiFiClient espClient;
PubSubClient client(espClient);

// Nombre único para cada nodo (Cámbialo a "Nodo2" en el otro ESP32)
const char* clientID = "Nodo2"; 

void setup_wifi() {
  delay(10);
  Serial.print("\nConectando a "); Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500); Serial.print(".");
  }
  Serial.println("\nWiFi conectado. IP: "); Serial.println(WiFi.localIP());
}

// Reconexión en caso de caída de red
void reconnect() {
  while (!client.connected()) {
    Serial.print("Intentando conexión MQTT...");
    if (client.connect(clientID)) {
      Serial.println("conectado");
    } else {
      Serial.print("falló, rc="); Serial.print(client.state());
      Serial.println(" reintentando en 5 segundos");
      delay(5000);
    }
  }
}

float getCorrectionFactor(float t, float h) {
  return 1.4327 - 0.003 * t - 0.0006 * h;
}

void setup() {
  Serial.begin(115200);
  analogSetAttenuation(ADC_11db);
  dht.begin();
  setup_wifi();
  client.setServer(mqtt_server, 1883);
}

void loop() {
  if (!client.connected()) reconnect();
  client.loop();

  float h = dht.readHumidity();
  float t = dht.readTemperature();

  if (!isnan(h) && !isnan(t)) {
    // Lectura y compensación
    int mq_raw = analogRead(MQ_PIN);
    float voltaje_sensor = (mq_raw * (3.3 / 4095.0)) * DIVISOR_FACTOR;
    float voltaje_final = voltaje_sensor / getCorrectionFactor(t, h);
    float ppm = pow(10, ((voltaje_final - 1.1) / 0.6) + 2.6); // Usando tu ajuste
    if(ppm < 350) ppm = 350;

    // --- ENVÍO DE DATOS ---
    // Creamos strings para los datos
    String payload = "{\"temp\":" + String(t) + ",\"hum\":" + String(h) + ",\"ppm\":" + String(ppm) + "}";
    
    // Publicamos en un tópico específico para este nodo
    char topic[50];
    sprintf(topic, "aula/%s/datos", clientID); 
    
    if (client.publish(topic, payload.c_str())) {
      Serial.print("Enviado a MQTT: "); Serial.println(payload);
    } else {
      Serial.println("Error al enviar por MQTT");
    }
  }

  delay(5000); // Envía datos cada 5 segundos
}