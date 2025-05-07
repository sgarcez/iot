#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

void connectWifi();
void connectMQTT();

// Settings
// ----------------------------------------
char ssid[] = "";
char ssid_pass[] = "";
char mqttHost[] = "";
char mqttTopic[] = "sensors/node4";
uint16_t mqttPort = 1883;

// Clients
// ----------------------------------------
WiFiClient espClient;
PubSubClient client(espClient);

// Sensor
// ----------------------------------------
Adafruit_BME280 bme;
float temperature, humidity, pressure, altitude;

// Interval
// ----------------------------------------
unsigned long lastMillis = 0;
unsigned long intervalMillis = 5000;

void connectWifi()
{
  digitalWrite(LED_BUILTIN, LOW);

  WiFi.begin(ssid, ssid_pass);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected..!");
  Serial.print("Got IP: ");
  Serial.println(WiFi.localIP());

  digitalWrite(LED_BUILTIN, HIGH);
}

void connectMQTT()
{
  digitalWrite(LED_BUILTIN, LOW);
  while (!client.connected())
  {
    Serial.print("Attempting MQTT connection...");
    if (client.connect("node4"))
    {
      Serial.println("connected");
    }
    else
    {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
  digitalWrite(LED_BUILTIN, HIGH);
}

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(9600);
  connectWifi();
  client.setServer(mqttHost, mqttPort);
  bme.begin(0x76);
}

void loop()
{
  if (!client.connected())
  {
    connectMQTT();
  }
  client.loop();

  if (millis() - lastMillis > intervalMillis)
  {
    lastMillis = millis();

    temperature = bme.readTemperature();
    humidity = bme.readHumidity();
    pressure = bme.readPressure() / 100.0F;
    String payload = "{\"temperature\": " + String(temperature) + ", \"humidity\": " + String(humidity) + ", \"pressure\": " + String(pressure) + " }";
    client.publish(mqttTopic, String(payload).c_str(), true);
  }
}
