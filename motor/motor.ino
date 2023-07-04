// Joystick Pins
const int xAxis = A4;  // Connect VRX pin here
const int yAxis = A5;  // Connect VRY pin here
const int buttonPin = 2;  // Connect SW pin here

#define ENABLE_MOTOR_1 A0
#define ENABLE_MOTOR_2 A1

#define MOTOR_1_CW  7
#define MOTOR_1_CCW 8
#define MOTOR_1_PWM 5
#define MOTOR_1_CURRENT_SENSOR A2

#define MOTOR_2_CW  4
#define MOTOR_2_CCW 9
#define MOTOR_2_PWM 6
#define MOTOR_2_CURRENT_SENSOR A3

void setup() {
  // Set the control pins as outputs
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

   // Joystick Setup
  pinMode(buttonPin, INPUT_PULLUP);  
  
  // Start serial communication at 9600 bps
  Serial.begin(9600);
}

void loop() {
  // Motor 1 rotates clockwise at half speed
  // digitalWrite(MOTOR_1_CW, HIGH);
  // digitalWrite(MOTOR_1_CCW, LOW);
  // analogWrite(MOTOR_1_PWM, 128); // Half speed

  // Motor 2 rotates counterclockwise at full speed
  // digitalWrite(MOTOR_2_CW, LOW);
  // digitalWrite(MOTOR_2_CCW, HIGH);
  // analogWrite(MOTOR_2_PWM, 255); // Full speed

  // Read the current sensor values
  int currentMotor1 = analogRead(MOTOR_1_CURRENT_SENSOR);
  int currentMotor2 = analogRead(MOTOR_2_CURRENT_SENSOR);

  // Print the current sensor values to the serial monitor
  Serial.print("Current Motor 1: ");
  Serial.print(currentMotor1);
  Serial.print("\tCurrent Motor 2: ");
  Serial.println(currentMotor2);

  delay(2000); // Run for 2 seconds
  
  // Stop motors
  analogWrite(MOTOR_1_PWM, 0);
  analogWrite(MOTOR_2_PWM, 0);

  delay(2000); // Pause for 2 seconds
}
