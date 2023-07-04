#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

void setup() {
  Serial.begin(9600);

  // Connect to the WiFi network created by the other NodeMCU (replace "MyAP" with the network name, and "password" with the password)
  WiFi.begin("MyAP", "password");

  Serial.print("Connecting to WiFi ");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("Connected!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;

    // Generate fake temperature and humidity
    float temp = random(200, 350) / 10.0;
    float hum = random(300, 600) / 10.0;

    // Specify the URL and the payload (data to be sent)
    String url = "http://192.168.4.1/message?temp=" + String(temp) + "&hum=" + String(hum);
    WiFiClient client;
    http.begin(client, url);
    int httpCode = http.GET();

    // Check the returning code                                                                  
    if (httpCode > 0) {
      String payload = http.getString();   //Get the response to the request
      Serial.println(httpCode);   // Print HTTP return code
      Serial.println(payload);    // Print request answer
    } else {
      Serial.println("Error on HTTP request");
    }
    http.end();
  }

  // Wait 10 seconds before sending the next request
  delay(500);
}
