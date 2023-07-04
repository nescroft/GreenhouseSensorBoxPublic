const int xAxis = A0;
const int yAxis = A1;
const int buttonPin = 2;

const int ledUp = 3;
const int ledRight = 4;
const int ledDown = 5;
const int ledLeft = 6;

void setup() {
  pinMode(ledUp, OUTPUT);
  pinMode(ledRight, OUTPUT);
  pinMode(ledDown, OUTPUT);
  pinMode(ledLeft, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);
}

void loop() {
  int xValue = analogRead(xAxis);
  int yValue = analogRead(yAxis);
  int buttonState = digitalRead(buttonPin);

  // Turn off all LEDs
  digitalWrite(ledUp, LOW);
  digitalWrite(ledRight, LOW);
  digitalWrite(ledDown, LOW);
  digitalWrite(ledLeft, LOW);

  if (buttonState == LOW) { // Button is pressed
    // Turn on all LEDs
    digitalWrite(ledUp, HIGH);
    digitalWrite(ledRight, HIGH);
    digitalWrite(ledDown, HIGH);
    digitalWrite(ledLeft, HIGH);
  } else {
    // Determine which direction the joystick is tilted and light up the appropriate LED
    if (yValue < 300) {
      digitalWrite(ledUp, HIGH); // Up
    } else if (yValue > 700) {
      digitalWrite(ledDown, HIGH); // Down
    }

    if (xValue < 300) {
      digitalWrite(ledLeft, HIGH); // Left
    } else if (xValue > 700) {
      digitalWrite(ledRight, HIGH); // Right
    }
  }

  delay(100);
}
