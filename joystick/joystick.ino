const int xAxis = A0;  // Connect VRX pin here
const int yAxis = A1;  // Connect VRY pin here
const int buttonPin = 2;  // Connect SW pin here

void setup() {
  Serial.begin(9600);  // Initialize serial communication at 9600 bits per second
  
  pinMode(buttonPin, INPUT_PULLUP);  // Set the button pin as input with internal pull-up resistor
}

void loop() {
  int xValue = analogRead(xAxis);  // Read the X-axis value (0 to 1023)
  int yValue = analogRead(yAxis);  // Read the Y-axis value (0 to 1023)
  int buttonState = digitalRead(buttonPin);  // Read the button state
  
  Serial.print("X-axis: ");
  Serial.print(xValue);
  Serial.print("\tY-axis: ");
  Serial.print(yValue);
  Serial.print("\tButton: ");
  Serial.println(buttonState == HIGH ? "Released" : "Pressed");
  
  delay(1000);  // Delay for a short period before reading again
}
