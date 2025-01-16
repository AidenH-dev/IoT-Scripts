// Desc:
// This program uses the ESP32 connected to a STEMMA QT BUS through I2C 
// that has both the SH1107 Feather OLED & the SHT41 Temp/Humidity sensor
// 
// Program Use:
// Display the info from SHT41 & ESP32 MAC address (can be toggled on and off with button A on the OLED Board)

#include <Wire.h>
#include <Adafruit_SH110X.h>
#include <Adafruit_SHT4x.h>
#include <WiFi.h>
#include <esp_wifi.h>

// Define I2C addresses and pins
#define OLED_RESET -1  // No reset pin
Adafruit_SH1107 display = Adafruit_SH1107(64, 128, &Wire, OLED_RESET);
Adafruit_SHT4x sht4 = Adafruit_SHT4x();

#define BUTTON_A_PIN 14 // GPIO pin connected to Button A

String macAddress;      // MAC address
bool showMac = false;   // Toggle flag for MAC address display
bool isPressed = false; // State of button press

// Function to read MAC address
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
    return "Failed to read MAC";
  }
}

void setup() {
  Serial.begin(115200);
  Wire.begin(21, 22);

  // Initialize Wi-Fi and get MAC address
  WiFi.mode(WIFI_STA);
  macAddress = getMacAddress();
  Serial.println("MAC Address: " + macAddress);

  // Initialize the OLED
  if (!display.begin(0x3C, true)) { // Default I2C address is 0x3C
    Serial.println("Failed to initialize OLED");
    while (1);
  }

  // Set up the OLED display
  display.setRotation(1);
  display.clearDisplay();
  display.setTextColor(SH110X_WHITE);
  display.setTextSize(1);

  // Initialize Button A
  pinMode(BUTTON_A_PIN, INPUT_PULLUP);

  // Initialize SHT41 sensor
  if (!sht4.begin()) {
    Serial.println("Failed to initialize SHT41 sensor");
    while (1);
  }
  Serial.println("SHT41 Sensor Ready!");
}

void loop() {
  // Check if Button A is pressed
  int buttonState = digitalRead(BUTTON_A_PIN);
  if (buttonState == LOW) { // Button is pressed
    if (!isPressed) { // Detect new press
      isPressed = true;
      showMac = !showMac; // Toggle MAC display
      Serial.println(showMac ? "Showing MAC Address" : "Hiding MAC Address");
    }
  } else {
    isPressed = false; // Reset press state when button is released
  }

  // Read temperature and humidity from SHT41
  sensors_event_t temp, humidity;
  if (sht4.getEvent(&humidity, &temp)) {
    Serial.print("Temp: ");
    Serial.print(temp.temperature);
    Serial.print(" C, Humidity: ");
    Serial.print(humidity.relative_humidity);
    Serial.println(" %");

    // Display readings on OLED
    display.clearDisplay();
    display.setCursor(0, 0);
    display.print("Temp: ");
    display.print(temp.temperature);
    display.println(" C");
    display.print("Humidity: ");
    display.print(humidity.relative_humidity);
    display.println(" %");

    // Conditionally display MAC address
    if (showMac) {
      display.setCursor(0, 55);
      display.print("MAC:");
      display.print(macAddress);
    }

    display.display();
  } else {
    Serial.println("Failed to read SHT41 data");
  }

  delay(2000); // Wait 2 seconds before next reading
}