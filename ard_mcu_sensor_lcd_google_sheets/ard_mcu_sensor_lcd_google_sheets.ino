#include <Arduino.h>
#include <ESP8266WiFi.h>
#include "HTTPSRedirect.h"
#include "DHT.h"
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <SoftwareSerial.h>
#include <ArduinoJson.h>
#include <EEPROM.h>

// WiFi credentials
const char *ssid = "";
const char *password = "";
// Google Script Deployment ID
const char *GScriptId = ""; // need to replace this every time you redeploy google sheets app script
// Google Sheets setup
const char *host = "script.google.com";
const int httpsPort = 443;
String url = String("/macros/s/") + GScriptId + "/exec";
HTTPSRedirect *client = nullptr;
// DHT sensor setup
#define DHTPIN D3     // Pin which is connected to the DHT sensor
#define DHTTYPE DHT21 // DHT 21 (AM2301)
DHT dht(DHTPIN, DHTTYPE);
// Setup timing
unsigned long lastUpdateLCDTime = 0;
unsigned long lastUploadTime = 0;
unsigned long lcdUpdateRateInMinutes = 1;
unsigned long lcdUpdateInterval = lcdUpdateRateInMinutes * 60 * 1000;
unsigned long dataUploadRateInMinutes = 2;
unsigned long uploadInterval = dataUploadRateInMinutes * 60 * 1000;
float temperatureOpenThreshold = 28;
float temperatureCloseThreshold = 22;
float humidityOpenThreshold = -1;
float humidityCloseThreshold = -1;
String currentTime; // updated at upload interval
int selectedGreenhouseNumber = 1;
int confirmedGreenhouseNumber = 0;

int backlightOn = 1; // 0 never, 1 sometimes, 2 always, 3 sometimes and at night
// Setup the display and I2C Address 0X27
LiquidCrystal_I2C lcd(0X27, 16, 2);
// setup arduino communication
SoftwareSerial mySerial(D7, D8); // RX, TX

void printAndSendDataToArduino(const float temperature, const float humidity)
{
  // print to serial monitor
  //  Serial.print("Humidity: ");
  //  Serial.print(humidity);
  //  Serial.print(" %\t");
  //  Serial.print("Temperature: ");
  //  Serial.print(temperature);
  //  Serial.println(" *C");
  // print to LCD
  lcd.setCursor(0, 0);
  lcd.print("Hum: ");
  lcd.print(humidity);
  lcd.print(" %   ");
  lcd.setCursor(0, 1);
  lcd.print("Tem: ");
  lcd.print(temperature);
  lcd.print(" C   ");
  // send to arduino over serial pins 7,8 on NodeMCU and 10, 11 on arduino
  // pin connection is 7->10, 8->11
  mySerial.print("Tem: ");
  mySerial.println(temperature);
  mySerial.print("Hum: ");
  mySerial.println(humidity);
}

String prevWord1;
String prevWord2;
void printToLCD(String word1, String word2 = "")
{
  if (word1 == prevWord1 && word2 == prevWord2)
  {
    return;
  }
  prevWord1 = word1;
  prevWord2 = word2;
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(word1);
  if (word2[0] != '\0')
  { // Check if word2 is not an empty string
    lcd.setCursor(0, 1);
    lcd.print(word2);
  }
  // Serial.print(word1);
  // Serial.print(" ");
  // Serial.println(word2);
}

bool isNightTime()
{
  // Extract hour from the currentTime string
  // Assuming the format is "yyyy/MM/dd HH:mm:ss"
  int hourPosition = 11; // Position where hour starts in the string
  int hour = currentTime.substring(hourPosition, hourPosition + 2).toInt();
  // Define night time as between 8 PM (20) and 6 AM (6)
  if (hour >= 20 || hour < 6)
  {
    return true; // It's night time
  }
  else
  {
    return false; // It's not night time
  }
}

void getControlsFromSheet()
{
  if (client != nullptr)
  {
    if (!client->connected())
    {
      client->connect(host, httpsPort);
    }
  }
  String payload = "{\"command\": \"get_controls\", \"sheet_name\": \"Controls" + String(confirmedGreenhouseNumber) + "\"}";
  if (client->POST(url, host, payload))
  {
    // parse the response
    String response = client->getResponseBody();
    // Debug print the response
    // Serial.println(response);
    // Using ArduinoJson library to parse the response
    const size_t capacity = JSON_OBJECT_SIZE(20);
    DynamicJsonDocument doc(capacity);
    DeserializationError error = deserializeJson(doc, response);
    if (error)
    {
      printToLCD("Cant parse res", "from sheets");
      // Print error details
      // Serial.println(error);
      return;
    }
    temperatureOpenThreshold = doc["temperatureOpenThreshold"];
    temperatureCloseThreshold = doc["temperatureCloseThreshold"];
    humidityOpenThreshold = doc["humidityOpenThreshold"];
    humidityCloseThreshold = doc["humidityCloseThreshold"];
    lcdUpdateRateInMinutes = doc["lcdUpdateRateInMinutes"];
    dataUploadRateInMinutes = doc["dataUploadRateInMinutes"];
    backlightOn = doc["backlightOn"];
    String currentTime = doc["currentTime"].as<String>();
    lcdUpdateInterval = lcdUpdateRateInMinutes * 60 * 1000;
    uploadInterval = dataUploadRateInMinutes * 60 * 1000;
    // Display the variables on the LCD
    printToLCD("TempOpenThres", String(temperatureOpenThreshold));
    delay(2000);
    printToLCD("TempCloseThres", String(temperatureCloseThreshold));
    delay(2000);
    printToLCD("HumOpenThres", String(humidityOpenThreshold));
    delay(2000);
    printToLCD("HumCloseThres", String(humidityCloseThreshold));
    delay(2000);
    printToLCD("LCD Upd (min)", String(lcdUpdateRateInMinutes));
    delay(2000);
    printToLCD("Data Upd (min)", String(dataUploadRateInMinutes));
    delay(2000);
    printToLCD("Backlight", String(backlightOn));
    delay(2000);
    printToLCD("Current Time", currentTime);
    delay(2000);
  }
  else
  {
    printToLCD("Failed", "to get data");
  }
}

void turnBacklightOn()
{
  switch (backlightOn)
  {
  case 0:
    // Never turn on
    break;
  case 1:
    lcd.backlight();
    break;
  case 2:
    // Always turn on
    lcd.backlight();
    break;
  case 3:
    // Turn on under some conditions or at night
    if (isNightTime())
    {
      lcd.backlight();
    }
    break;
  default:
    // handle invalid values
    break;
  }
}

void turnBacklightOff()
{
  if (backlightOn == 2)
  {
    return; // Keep the backlight always on
  }
  if (backlightOn == 3 && isNightTime())
  {
    return; // Keep the backlight on at night
  }
  // Turn off the backlight in other cases
  lcd.noBacklight();
}

void connectToWiFi()
{
  // will find any wifi network it can without a password
  int connectionAttempts = 0;
  const int maxConnectionAttempts = 10;
  int networkCount = WiFi.scanNetworks();
  printToLCD("Connecting", "to WiFi");
  for (int i = 0; i < networkCount; ++i)
  {
    if (WiFi.encryptionType(i) == ENC_TYPE_NONE)
    {
      printToLCD("Attempting WiFi", WiFi.SSID(i));
      WiFi.begin(WiFi.SSID(i));
      while (WiFi.status() != WL_CONNECTED)
      {
        delay(1000);
        connectionAttempts++;
        if (connectionAttempts >= maxConnectionAttempts)
        {
          printToLCD("Connection", "failed");
          return;
        }
      }
      printToLCD("Connected to", WiFi.SSID(i));
      return;
    }
  }

  printToLCD("No open", "networks found");
}

bool firstLoop = true;

void setup()
{
  EEPROM.begin(4);
  Serial.begin(9600);
  delay(10);
  lcd.init();
  turnBacklightOn();
  // Initialize DHT sensor
  dht.begin();
  // preparing google sheets client
  client = new HTTPSRedirect(httpsPort);
  client->setInsecure();
  client->setPrintResponseBody(true);
  client->setContentTypeHeader("application/json");
  // bool flag = false;
  // for (int i = 0; i < 5; i++) {
  //   int retval = client->connect(host, httpsPort);
  //   if (retval == 1) {
  //     flag = true;
  //     break;
  //   }
  // }
  // if (!flag) {
  //   printToLCD("Cant connect", "to sheets");
  //   return;
  // }
  lastUpdateLCDTime = millis();
  lastUploadTime = millis();
  checkForGreenhouseNumberInLocalStorage();
  delay(2000);
  mySerial.begin(9600);
}

bool numberIncremented = false;

void checkForGreenhouseNumberInLocalStorage()
{
  EEPROM.get(0, confirmedGreenhouseNumber);
  // Check if the value is 0xFFFFFFFF (uninitialized EEPROM)
  if (confirmedGreenhouseNumber == 0xFFFFFFFF || confirmedGreenhouseNumber < 1 || confirmedGreenhouseNumber > 8)
  {
    // The EEPROM is uninitialized, set default value
    confirmedGreenhouseNumber = 0;
    EEPROM.put(0, confirmedGreenhouseNumber);
    EEPROM.commit();
  }
}

void listenForReset()
{
  Serial.println("listening for reset");
  if (mySerial.available())
  {
    String message = mySerial.readStringUntil('\n');
    message.trim(); // for some reason i needed to trim it but idk why. it wasn't working without trim()
    Serial.println(message);
    if (message == "reset")
    {
      turnBacklightOn();
      Serial.println("message == reset");
      confirmedGreenhouseNumber = 0;
      printToLCD("Greenhouse", "Reset!");
      EEPROM.put(0, confirmedGreenhouseNumber);
      EEPROM.commit();
      delay(2000);
    }
  }
}

void chooseGreenhouse()
{
  if (mySerial.available())
  {
    String message = mySerial.readStringUntil('\n');
    // Extract joystick data from message coming from arduino
    int delimiterIndex = message.indexOf(',');
    selectedGreenhouseNumber = message.substring(0, delimiterIndex).toInt();
    int buttonState = message.substring(delimiterIndex + 1).toInt();
    printToLCD("Select & click", ("Greenhouse " + String(selectedGreenhouseNumber)));
    if (buttonState == LOW)
    {
      confirmedGreenhouseNumber = selectedGreenhouseNumber;
      if (confirmedGreenhouseNumber != 0)
      {
        printToLCD(("Greenhouse " + String(confirmedGreenhouseNumber)), "Confirmed");
      }
      EEPROM.put(0, confirmedGreenhouseNumber);
      EEPROM.commit();
      delay(2000);
    }
  }
  else
  {
    printToLCD("Select & click ~", ("Greenhouse " + String(selectedGreenhouseNumber)));
  }
}

void loop()
{
  if (confirmedGreenhouseNumber == 0)
  {
    chooseGreenhouse();
    delay(100);
    return;
  }
  else
  {
    listenForReset();
  }

  unsigned long currentTime = millis();
  unsigned long newLcdTime = currentTime - lastUpdateLCDTime;
  // Update LCD every 1 minute
  if (firstLoop || newLcdTime >= lcdUpdateInterval)
  {
    lcd.clear();
    turnBacklightOff();
    // Read data from DHT sensor
    float humidity = dht.readHumidity();
    float temperature = dht.readTemperature();
    // Check if any reads failed
    if (isnan(humidity) || isnan(temperature))
    {
      printToLCD("Sensor read", "failed");
      return;
    }
    printAndSendDataToArduino(temperature, humidity);
    lastUpdateLCDTime = currentTime;
  }

  unsigned long newUploadTime = currentTime - lastUploadTime;
  // Upload to Google Sheets
  if (firstLoop || newUploadTime >= uploadInterval)
  {
    firstLoop = false;
    turnBacklightOn(); // turn on backlight to alert user of uploading, in case they need to check the message
    connectToWiFi();
    // Connect to Google Sheets
    if (client != nullptr)
    {
      if (!client->connected())
      {
        client->connect(host, httpsPort);
      }
    }
    float humidity = dht.readHumidity();
    float temperature = dht.readTemperature();
    if (isnan(humidity) || isnan(temperature))
    {
      printToLCD("Sensor read", "failed");
      return;
    }
    String payload = "{\"command\": \"insert_row\", \"sheet_name\": \"GH" + String(confirmedGreenhouseNumber) + "\", \"values\": \"";
    payload += String(temperature) + "," + String(humidity) + "\"}";
    // Publish data to Google Sheets
    printToLCD("Uploading", "Data");
    if (client->POST(url, host, payload))
    {
      printToLCD("Upload success");
    }
    else
    {
      printToLCD("Upload error");
    }
    getControlsFromSheet();
    lastUploadTime = currentTime;
  }
  delay(100);
}
