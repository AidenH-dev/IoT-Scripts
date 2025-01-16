#include <Wire.h>

#define BLUE_LED_PIN 2 // GPIO pin for the blue LED (adjust if needed)

void setup() {
  // Start Serial communication
  Serial.begin(115200);
  Wire.begin(21, 22); // ESP32 default I2C pins

  // Debug message
  Serial.println("\nStarting I2C Scanner...");

  // Initialize the blue LED
  pinMode(BLUE_LED_PIN, OUTPUT);
  digitalWrite(BLUE_LED_PIN, LOW); // Turn off the LED initially
}

void loop() {
  byte error, address;
  int devices = 0;

  // Indicate scanning start
  Serial.println("Scanning...");
  digitalWrite(BLUE_LED_PIN, HIGH); // Turn on the LED during scanning

  for (address = 1; address < 127; address++) {
    Wire.beginTransmission(address);
    error = Wire.endTransmission();

    if (error == 0) {
      Serial.print("I2C device found at address 0x");
      if (address < 16) Serial.print("0");
      Serial.println(address, HEX);
      devices++;
    }
  }

  // Turn off the LED after scanning
  digitalWrite(BLUE_LED_PIN, LOW);

  // Report results
  if (devices == 0) {
    Serial.println("No I2C devices found");
  } else {
    Serial.println("Done scanning");
  }

  // Wait 5 seconds before scanning again
  delay(5000);
}