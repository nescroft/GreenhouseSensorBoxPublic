#include <LiquidCrystal.h>

const int currentSensorPin = A0;
const float currentSensitivity = 185.0; // mV/A for ACS712 5A
const float ADCResolution = 5.0 / 1023.0; // 5V and 1024 steps
const float currentThreshold = 1.5; // threshold in Amps

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
  // Set up the display's number of columns and rows:
  lcd.begin(16, 2);
  lcd.clear();
  lcd.setCursor(0, 0); // Set cursor to column 0, row 0
  lcd.print("Ellie Ashton!");
  lcd.setCursor(0,1);
  lcd.print("is coooool");

  //setup hum/temp sensor
  // pinMode(currentSensorPin, INPUT);
}

void loop() {
   int sensorValue = analogRead(currentSensorPin);
    float voltage = sensorValue * ADCResolution;
    float current = (voltage - 2.5) / (currentSensitivity / 1000.0); // Subtracting the offset (2.5V)

    Serial.print("Current: ");
    Serial.print(current);
    Serial.println(" A");

    if (abs(current) > currentThreshold) {
        // Current spiked, probably because window is fully opened or closed
        // Insert code here to stop the motor
    }
  delay(1000);
}