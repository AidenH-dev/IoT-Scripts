#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <WiFi.h>
#include <esp_wifi.h>

// Define the I2C address and LCD dimensions
#define I2C_ADDR 0x27 // Replace with your LCD's I2C address
#define LCD_COLUMNS 16
#define LCD_ROWS 2

// Initialize the LCD
LiquidCrystal_I2C lcd(I2C_ADDR, LCD_COLUMNS, LCD_ROWS);

// Function to get MAC address
String getMacAddress() {
  uint8_t baseMac[6];
  esp_err_t ret = esp_wifi_get_mac(WIFI_IF_STA, baseMac);
  if (ret == ESP_OK) {
    char macStr[18]; // String to hold formatted MAC address
    snprintf(macStr, sizeof(macStr), "%02X:%02X:%02X:%02X:%02X:%02X",
             baseMac[0], baseMac[1], baseMac[2],
             baseMac[3], baseMac[4], baseMac[5]);
    return String(macStr);
  } else {
    return "MAC ERR";
  }
}

void setup() {
  Serial.begin(115200);

  // Initialize Wi-Fi to retrieve MAC address
  WiFi.mode(WIFI_STA);

  // Initialize the LCD with correct parameters
  lcd.begin(LCD_COLUMNS, LCD_ROWS); // Specify columns and rows
  lcd.backlight();                  // Turn on the LCD backlight
  lcd.clear();                      // Clear the LCD display

  // Display MAC address on LCD
  String macAddress = getMacAddress();
  lcd.setCursor(0, 0); // Set cursor to top-left
  lcd.print("MAC Address:");
  lcd.setCursor(0, 1); // Set cursor to second row
  lcd.print(macAddress);

  // Debug output
  Serial.println("MAC Address: " + macAddress);
}

void loop() {
  // Nothing to do here
}