#include "DHT.h"
#include <LiquidCrystal.h>

#define DHTPIN 7          // Pin which is connected to the DHT sensor.
#define DHTTYPE DHT21     // DHT 21 (AM2301)

DHT dht(DHTPIN, DHTTYPE);

// Initialize the LiquidCrystal library
// VSS (or V-) to Arduino GND
// VDD (or V+) to Arduino 5V
// VO to the wiper (middle pin) of a 10k potentiometer (The other two pins of the potentiometer connect to 5V and GND)
// RS (LCD pin 4) to Arduino pin 12
// E  (LCD pin 6) to Arduino pin 11
// D4 (LCD pin 11) to Arduino pin 5
// D5 (LCD pin 12) to Arduino pin 4
// D6 (LCD pin 13) to Arduino pin 3
// D7 (LCD pin 14) to Arduino pin 2
//A (Anode, or back-light positive) to Arduino 5V
//K (Cathode, or back-light ground) to Arduino GND
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

void setup() {
  Serial.begin(9600);
  dht.begin();
  lcd.begin(16, 2);
  lcd.clear();
}

void loop() {
  // Wait a few seconds between measurements.
  delay(2000);

  // Reading temperature or humidity takes about 250 milliseconds!
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();

  // Check if any reads failed and exit early (to try again).
  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  // Print the values.
  // Serial.print("Humidity: ");
  // Serial.print(humidity);
  // Serial.print(" %\t");
  // Serial.print("Temperature: ");
  // Serial.print(temperature);
  // Serial.println(" *C");

  lcd.setCursor(0, 0); // Set cursor to column 0, row 0
  lcd.print("Hum: ");
  lcd.print(humidity);
  lcd.print(" %   ");

  lcd.setCursor(0, 1);
  lcd.print("Tem: ");
  lcd.print(temperature);
  lcd.print(" C   ");
}
