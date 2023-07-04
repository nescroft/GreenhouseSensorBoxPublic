#include "DHT.h"
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define DHTPIN D7      // Pin which is connected to the DHT sensor.
#define DHTTYPE DHT21 // DHT 21 (AM2301)

DHT dht(DHTPIN, DHTTYPE);

LiquidCrystal_I2C lcd(0X27, 16, 2); // Set the I2C address (0x27) and the LCD dimensions (16x2)

void setup()
{
    Serial.begin(9600);
    dht.begin();

    lcd.init();      // Initialize the LCD
    // lcd.backlight(); // Turn on the backlight (if supported)

    lcd.clear();
}

void loop()
{
    // Wait a few seconds between measurements.
    delay(2000);

    // Reading temperature or humidity takes about 250 milliseconds!
    float humidity = dht.readHumidity();
    float temperature = dht.readTemperature();

    // Check if any reads failed and exit early (to try again).
    if (isnan(humidity) || isnan(temperature))
    {
        Serial.println("Failed to read from DHT sensor!");
        return;
    }

    Serial.print("Humidity: ");
    Serial.print(humidity);
    Serial.print(" %\t");
    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.println(" *C");

    lcd.setCursor(0, 0); // Set cursor to column 0, row 0
    lcd.print("Hum: ");
    lcd.print(humidity);
    lcd.print(" %   ");

    lcd.setCursor(0, 1);
    lcd.print("Tem: ");
    lcd.print(temperature);
    lcd.print(" C   ");
}
