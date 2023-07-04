#include <Arduino.h>
#include <ESP8266WiFi.h>
#include "HTTPSRedirect.h"
#include "DHT.h"

// WiFi credentials
const char *ssid = "";
const char *password = "";

// Google Script Deployment ID
const char *GScriptId = "";

// Google Sheets setup
const char *host = "script.google.com";
const int httpsPort = 443;
String url = String("/macros/s/") + GScriptId + "/exec";
HTTPSRedirect *client = nullptr;

// DHT sensor setup
#define DHTPIN D2     // Pin which is connected to the DHT sensor
#define DHTTYPE DHT21 // DHT 21 (AM2301)
DHT dht(DHTPIN, DHTTYPE);

void setup()
{
  Serial.begin(9600);
  delay(10);
  Serial.println('\n');

  // Connect to WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("Connected to WiFi");

  // Initialize DHT sensor
  dht.begin();

  // Connect to Google Sheets
  client = new HTTPSRedirect(httpsPort);
  client->setInsecure();
  client->setPrintResponseBody(true);
  client->setContentTypeHeader("application/json");
  bool flag = false;
  for (int i = 0; i < 5; i++)
  {
    int retval = client->connect(host, httpsPort);
    if (retval == 1)
    {
      flag = true;
      break;
    }
  }
  if (!flag)
  {
    Serial.println("Could not connect to Google Sheets");
    return;
  }
}

void loop()
{
  // Read data from DHT sensor
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();

  // Check if any reads failed
  if (isnan(humidity) || isnan(temperature))
  {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  // Connect to Google Sheets
  if (client != nullptr)
  {
    if (!client->connected())
    {
      client->connect(host, httpsPort);
    }
  }

  // Create JSON object string
  String payload = "{\"command\": \"insert_row\", \"sheet_name\": \"Sheet1\", \"values\": \"";
  payload += String(temperature) + "," + String(humidity) + "," + "0" + "\"}";

  // Publish data to Google Sheets
  Serial.println("Publishing data...");
  Serial.println(payload);
  if (client->POST(url, host, payload))
  {
    // Success
  }
  else
  {
    Serial.println("Error while connecting");
  }

  // Wait for 5 seconds
  delay(1000);
}
