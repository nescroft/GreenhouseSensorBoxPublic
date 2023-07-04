const int analogInputPin = A0; // Analog input pin that the output voltage is connected to
const float referenceVoltage = 5.0; // Reference voltage of Arduino (5V for most Arduinos)

void setup() {
  Serial.begin(9600); // Initialize serial communication at 9600 bits per second
}

void loop() {
  int sensorValue = analogRead(analogInputPin); // Read the input on analog pin 0
  float voltage = sensorValue * (referenceVoltage / 1023.0);  // Convert the analog reading to voltage
  
  Serial.print("A0 Voltage: "); // Print the voltage value to the serial monitor
  Serial.print(voltage);
  Serial.println(" V");

  // Using a voltage divider, multiply by the division ratio
  float R1 = 10000.0; // Replace with the value of your R1 resistor in ohms
  float R2 = 2000.0; // Replace with the value of your R2 resistor in ohms
  float divisionRatio = (R1 + R2) / R2;
  voltage = voltage*divisionRatio;

  Serial.print("Real Voltage: "); // Print the voltage value to the serial monitor
  Serial.print(voltage);
  Serial.println(" V");

  delay(1000); // Delay a second between readings
}
