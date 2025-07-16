#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Set the LCD address to 0x27 or 0x3F depending on your module
LiquidCrystal_I2C lcd(0x27, 16, 2); // 16 columns, 2 rows

void setup() {
  lcd.begin(16, 2);       // Initialize the LCD
  lcd.backlight();        // Turn on the backlight

  lcd.setCursor(0, 0);    // First column, first row
  lcd.print("Hello, world!");

  lcd.setCursor(0, 1);    // First column, second row
  lcd.print("I2C LCD Display");
}

void loop() {
  // Nothing needed here
}
