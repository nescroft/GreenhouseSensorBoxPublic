#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

ESP8266WebServer server(80);

const int ledPin = D0; // The pin where the LED is connected

void setup() {
  Serial.begin(9600);

  // Set the NodeMCU as an access point
  WiFi.mode(WIFI_AP);

  // Configure the access point (replace "MyAP" with your desired network name, and "password" with your desired password)
  WiFi.softAP("MyAP", "password");

  Serial.println("Access point is set up! Connect to it with the credentials above.");

  // Configure the LED pin as an OUTPUT
  pinMode(ledPin, OUTPUT);

  // Handle HTTP requests
  server.on("/message", []() {
    digitalWrite(ledPin, HIGH); // Turn the LED on
    
    String temp = server.arg("temp");
    String hum = server.arg("hum");
    Serial.print("Received random value1: ");
    Serial.print(temp);
    Serial.print(" and random value2: ");
    Serial.println(hum);
    
    delay(500); // Delay for 500 milliseconds
    digitalWrite(ledPin, LOW); // Turn the LED off
    
    server.send(200, "text/plain", "Message received");
  });

  // Start the server
  server.begin();
}

void loop() {
  server.handleClient();
}
