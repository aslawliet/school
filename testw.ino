#include <WiFi.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <HTTPClient.h>

// Set the LCD address to 0x27 or 0x3F depending on your I2C module
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Use String instead of char[] for SSID and Password
String ssid = "Assembly Teachers 01";
String password = "teachers@assembly";

const char* server = "http://192.168.1.195:5000/groq"; 

void setup() {
  Serial.begin(9600);
  delay(1000);  // Small delay to let Serial settle
  lcd.begin(4, 5);         // Initialize the LCD
  lcd.backlight();  

  lcd.setCursor(0, 0); // Column 0, Row 0
  lcd.print("ESP32 Ready!");

  // Connect to WiFi using String variables
  WiFi.begin(ssid.c_str(), password.c_str());  // Convert String to const char* using .c_str()

  lcd.setCursor(0, 1); // Column 0, Row 1
  lcd.print("Connecting to WiFi");
  // Serial.print("Connecting to WiFi");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    lcd.setCursor(0, 0); // Column 0, Row 0
    lcd.print(".");
  }

  lcd.setCursor(0, 1); // Column 0, Row 1
  lcd.print("Connected!");
  // Serial.println("\nConnected!");
  lcd.setCursor(0, 0);
  lcd.print("IP Address: ");
  lcd.setCursor(0, 1);
  lcd.print(WiFi.localIP());  // Show ESP32's local IP

  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;

    http.begin(server);
    http.addHeader("Content-Type", "application/json");

    String json = "{\"message\":\"Who is Tru Kait?\"}";

    int httpResponseCode = http.POST(json);

    if (httpResponseCode > 0) {
      String response = http.getString();
      lcd.setCursor(0, 0);
      lcd.print("Response:");
      lcd.setCursor(0, 1);
      lcd.print(response);
    } else {
      lcd.setCursor(0, 0);
      lcd.print("Error code: ");
      lcd.setCursor(0, 1);
      lcd.print(httpResponseCode);
    }

    http.end();
  }
}

void loop() {
  // Nothing for now
}
