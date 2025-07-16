#include <WiFi.h>
#include <HTTPClient.h>

const char* ssid = "Assembly Teachers 01";
const char* password = "teachers@assembly";

// IP of your PC running the Flask server (change this!)
const char* server = "http://192.168.1.195:5000/groq"; 

void setup() {
  Serial.begin(115200);
  Serial.print("hello ....");
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected!");

  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;

    http.begin(server);
    http.addHeader("Content-Type", "application/json");

    String json = "{\"message\":\"Who is Tru Kait?\"}";

    int httpResponseCode = http.POST(json);

    if (httpResponseCode > 0) {
      String response = http.getString();
      lcd.print("Response:");
      lcd.print(response);
    } else {
      lcd.print("Error code: ");
      lcd.print(httpResponseCode);
    }

    http.end();
  }
}

void loop() {
  // nothing
}
