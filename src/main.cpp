#define BLYNK_PRINT Serial

/* Fill-in your Template ID (only if using Blynk.Cloud) */
#define BLYNK_TEMPLATE_ID   "TMPLdWsd4Yrq"
#define BLYNK_DEVICE_NAME "FAN"


#include <ESP8266WiFi.h>
#include <BlynkSimpleStream.h>

#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

#define DHTPIN 13
#define DHTTYPE DHT11

#define FAN D1


DHT_Unified dht(DHTPIN, DHTTYPE);
// Your WiFi credentials.
// Set password to "" for open networks.
const char* ssid = "P.TRIS";
const char* pass = "mahmuditri";

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "_stff9EcYPLXKhjTZxylkGDKsymwQKN-";

WiFiClient wifiClient;

// This function tries to connect to the cloud using TCP
bool connectBlynk()
{
  wifiClient.stop();
  return wifiClient.connect(BLYNK_DEFAULT_DOMAIN, BLYNK_DEFAULT_PORT);
}

// This function tries to connect to your WiFi network
void connectWiFi()
{
  Serial.print("Connecting to ");
  Serial.println(ssid);

  if (pass && strlen(pass)) {
    WiFi.begin((char*)ssid, (char*)pass);
  } else {
    WiFi.begin((char*)ssid);
  }

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
}


void setup()
{
  // Debug console
  Serial.begin(9600);

  connectWiFi();

  connectBlynk();

  Blynk.begin(wifiClient, auth);

  pinMode(FAN, INPUT);
  digitalWrite(FAN, HIGH);

  pinMode(D4, OUTPUT);
  digitalWrite(D4, LOW);

  dht.begin();
}

BLYNK_WRITE(V0)
{
  if( param.asInt() ) {
    digitalWrite(FAN, LOW);
    pinMode(FAN, OUTPUT);
  } else {
    digitalWrite(FAN, HIGH);
    pinMode(FAN, INPUT);
  }

}

void loop()
{
  // Reconnect WiFi
  if (WiFi.status() != WL_CONNECTED) {
    connectWiFi();
    return;
  }

  // Reconnect to Blynk Cloud
  if (!wifiClient.connected()) {
    connectBlynk();
    return;
  }

  delay(2000);

  Blynk.run();

  sensors_event_t event;

  dht.temperature().getEvent(&event);
  if (isnan(event.temperature)) {
    Serial.println(F("Error reading temperature!"));
  }
  else {
      if (event.temperature > 30.00) {
        digitalWrite(FAN, LOW);
        pinMode(FAN, OUTPUT);
        Blynk.virtualWrite(V0, 1);
      }
      else {
        digitalWrite(FAN, HIGH);
        pinMode(FAN, INPUT);
      }
    Blynk.virtualWrite(V1, event.temperature);
    Serial.print("Temperature: ");
    Serial.print(event.temperature);
    Serial.println(" *C");
  }

  dht.humidity().getEvent(&event);
  if (isnan(event.relative_humidity)) {
    Serial.println(F("Error reading humidity!"));
  }
  else {
    Blynk.virtualWrite(V2, event.relative_humidity);
  }

}