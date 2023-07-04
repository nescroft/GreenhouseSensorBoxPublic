#include <SoftwareSerial.h>

SoftwareSerial mySerial(11,10); // RX, TX

// Joystick Pins
const int xAxis = A5;    // Connect VRX pin here
const int yAxis = A4;    // Connect VRY pin here
const int buttonPin = 12; // Connect SW pin here

// Motor Pins
#define ENABLE_MOTOR_1 A0
#define ENABLE_MOTOR_2 A1

#define MOTOR_1_CW 7
#define MOTOR_1_CCW 8
#define MOTOR_1_PWM 5
#define MOTOR_1_CURRENT_SENSOR A2

#define MOTOR_2_CW 4
#define MOTOR_2_CCW 9
#define MOTOR_2_PWM 6
#define MOTOR_2_CURRENT_SENSOR A3

unsigned long lastCheckTempTime = 0;

const unsigned long checkTempInterval = 10000; //60 * 1000; 

void setup()
{
  // Initialize serial communication at 9600 bits per second
  Serial.begin(9600);
  
  // Joystick Setup
  pinMode(buttonPin, INPUT_PULLUP); // Set the button pin as input with internal pull-up resistor

  // Motor Setup
  pinMode(ENABLE_MOTOR_1, OUTPUT);
  pinMode(ENABLE_MOTOR_2, OUTPUT);

  pinMode(MOTOR_1_CW, OUTPUT);
  pinMode(MOTOR_1_CCW, OUTPUT);
  pinMode(MOTOR_1_PWM, OUTPUT);

  pinMode(MOTOR_2_CW, OUTPUT);
  pinMode(MOTOR_2_CCW, OUTPUT);
  pinMode(MOTOR_2_PWM, OUTPUT);

  // Enable the motors
  digitalWrite(ENABLE_MOTOR_1, HIGH);
  digitalWrite(ENABLE_MOTOR_2, HIGH);

  delay(5000);
  mySerial.begin(9600);
}

int selectedGreenhouseNumber = 1;

unsigned long buttonDownTime;

void sendGreenhouseSelection(int xValue, int buttonState){
  if(xValue > 924){
        selectedGreenhouseNumber++;
      } else if (xValue < 100){
        selectedGreenhouseNumber--;
      }
      selectedGreenhouseNumber = constrain(selectedGreenhouseNumber, 1, 8);
      mySerial.println(String(selectedGreenhouseNumber)+ "," + String(buttonState));
}

void loop()
{
  unsigned long currentTime = millis();
  int xValue = analogRead(xAxis);           // Read the X-axis value (0 to 1023)
  int yValue = analogRead(yAxis);           // Read the Y-axis value (0 to 1023)
  int buttonState = digitalRead(buttonPin); // Read the button state
  xValue = 1024 - xValue; //reverse xValue... cause joystick is mounted weird
  // Serial.println(xValue);
  // Serial.println(buttonState);
  // String message = String(xValue) + "," + String(buttonState);
  // mySerial.println(message);

  if(buttonState == HIGH){
    buttonDownTime = millis();
    sendGreenhouseSelection(xValue, buttonState);
  } else {
    if(millis() - buttonDownTime > 5000){
      Serial.println("reset");
      mySerial.println("reset");
    } else {
      sendGreenhouseSelection(xValue, buttonState);
    }
  }
  delay(500);
  // Control Motor 2 with Y-axis
  // if (yValue > 520)
  // {
  //   digitalWrite(MOTOR_1_CW, HIGH); // Clockwise rotation
  //   digitalWrite(MOTOR_1_CCW, LOW);
  //   analogWrite(MOTOR_1_PWM, (yValue - 512) / 2); // Full speed
  // }
  // else if (yValue < 504)
  // {
  //   digitalWrite(MOTOR_1_CW, LOW); // Counter-clockwise rotation
  //   digitalWrite(MOTOR_1_CCW, HIGH);
  //   analogWrite(MOTOR_1_PWM, 255 - (yValue / 2)); // Full speed
  // }
  // else
  // {
  //   analogWrite(MOTOR_1_PWM, 0); // Stop motor when Y-axis is centered
  // }

  // // Read the current sensor values
  // int currentMotor1 = analogRead(MOTOR_1_CURRENT_SENSOR);
  // int currentMotor2 = analogRead(MOTOR_2_CURRENT_SENSOR);

  // Print the current sensor values to the serial monitor
  // Serial.print("Current Motor 1: ");
  // Serial.print(currentMotor1);
  // Serial.print("\tCurrent Motor 2: ");
  // Serial.println(currentMotor2);

  // if (mySerial.available() && (currentTime - lastCheckTempTime >= checkTempInterval)) {
  //   String message = mySerial.readString(); // Read the message from NodeMCU
  //   Serial.println(message); // Print the message to the Serial Monitor
  // }

  delay(10); // Delay for a short period before reading again
}
