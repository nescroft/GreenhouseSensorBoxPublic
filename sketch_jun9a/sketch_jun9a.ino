const int potPin = A1;        // Analog input pin for potentiometer
const int motorPinA = 3;      // IA pin connected to Arduino pin 3
const int motorPinB = 5;      // IB pin connected to Arduino pin 5
const int button2Pin = 2;
const int button4Pin = 4;
const int button7Pin = 7;

void setup() {
  Serial.begin(9600);
  pinMode(motorPinA, OUTPUT);
  pinMode(motorPinB, OUTPUT);
  digitalWrite(motorPinB, LOW); // Keep this pin LOW for forward direction
}

void loop() {
  int button2Val = digitalRead(button2Pin);
  int button4Val = digitalRead(button4Pin);
  int button7Val = digitalRead(button7Pin);
  // Serial.println("b2");
  // Serial.println(button2Val);
  // Serial.println("b4");
  // Serial.println(button4Val);
  // Serial.println("b7");
  // Serial.println(button7Val);


  int potValue = analogRead(potPin);        // Read the potentiometer value (0 to 1023)
  Serial.println(potValue);
  // Map the potentiometer value to a PWM value (0 to 255)
  int pwmValue = map(potValue, 0, 1023, 0, 255);

  // Write the PWM value to the motorPinA for forward direction
  analogWrite(motorPinA, pwmValue);
  // analogWrite(motorPinB, -pwmValue);

  // Optional delay, you can adjust or remove it based on your requirements
  delay(10);
}
