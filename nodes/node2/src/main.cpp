#include <Arduino.h>
#include <ESP8266WiFi.h> //https://github.com/esp8266/Arduino
#include <PubSubClient.h>

void connectWifi();
void connectMQTT();

// Settings
// ----------------------------------------
char ssid[] = "";
char ssid_pass[] = "";
char mqttHost[] = "";
char mqttTopic[] = "sensors/node2";
uint16_t mqttPort = 1883;

// Clients
// ----------------------------------------
WiFiClient espClient;
PubSubClient client(espClient);

// pms5003
// ----------------------------------------
char checkValue(unsigned char *thebuf, char leng);
int transmitPM01(unsigned char *thebuf);
int transmitPM2_5(unsigned char *thebuf);
int transmitPM10(unsigned char *thebuf);

#define LENG 31 //0x42 + 31 bytes equal to 32 bytes
unsigned char buf[LENG];

int PM01Value = 0;  //define PM1.0 value of the air detector module
int PM2_5Value = 0; //define PM2.5 value of the air detector module
int PM10Value = 0;  //define PM10 value of the air detector module

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
    if (client.connect("node2"))
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
}

void loop()
{
  if (!client.connected())
  {
    connectMQTT();
  }
  client.loop();

  if (Serial.find(0x42))
  { // start to read after detecting 0x42
    Serial.readBytes(buf, LENG);

    if (buf[0] == 0x4d)
    {
      if (checkValue(buf, LENG))
      {
        PM01Value = transmitPM01(buf);
        PM2_5Value = transmitPM2_5(buf);
        PM10Value = transmitPM10(buf);
      }
    }
  }

  if (millis() - lastMillis > 5000)
  {
    lastMillis = millis();
    Serial.print("PM1.0: ");
    Serial.print(PM01Value);
    Serial.println("  ug/m3");

    Serial.print("PM2.5: ");
    Serial.print(PM2_5Value);
    Serial.println("  ug/m3");

    Serial.print("PM10 : ");
    Serial.print(PM10Value);
    Serial.println("  ug/m3");
    Serial.println();

    String payload = "{\"PM1\": " + String(PM01Value) + ", \"PM2_5\": " + String(PM2_5Value) + ", \"PM10\": " + String(PM10Value) + " }";
    client.publish(mqttTopic, String(payload).c_str(), true);
  }
}

char checkValue(unsigned char *thebuf, char leng)
{
  char receiveflag = 0;
  int receiveSum = 0;

  for (int i = 0; i < (leng - 2); i++)
  {
    receiveSum = receiveSum + thebuf[i];
  }
  receiveSum = receiveSum + 0x42;

  if (receiveSum == ((thebuf[leng - 2] << 8) + thebuf[leng - 1])) //check serial data
  {
    receiveSum = 0;
    receiveflag = 1;
  }
  return receiveflag;
}

int transmitPM01(unsigned char *thebuf)
{
  int PM01Val;
  PM01Val = ((thebuf[3] << 8) + thebuf[4]);
  return PM01Val;
}

int transmitPM2_5(unsigned char *thebuf)
{
  int PM2_5Val;
  PM2_5Val = ((thebuf[5] << 8) + thebuf[6]);
  return PM2_5Val;
}

int transmitPM10(unsigned char *thebuf)
{
  int PM10Val;
  PM10Val = ((thebuf[7] << 8) + thebuf[8]);
  return PM10Val;
}
