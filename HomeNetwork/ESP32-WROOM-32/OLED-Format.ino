#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

// OLED display object
Adafruit_SH1107 display(SCREEN_HEIGHT, SCREEN_WIDTH, &Wire, -1);

// GPIO for the blue LED
#define BLUE_LED_PIN 2

// GPIO for Button A (connected to SH1107 button pin)
#define BUTTON_A_PIN 14 // Replace with the actual GPIO pin connected to Button A

void setup() {
  Serial.begin(115200);
  Wire.begin(21, 22);

  // Initialize the OLED
  if (!display.begin(0x3C, true)) {
    Serial.println("Failed to initialize OLED");
    while (1);
  }

  display.setRotation(1);
  display.clearDisplay();
  display.setTextColor(SH110X_WHITE);
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println("Testing SH1107");
  display.display();

  // Initialize the blue LED
  pinMode(BLUE_LED_PIN, OUTPUT);
  digitalWrite(BLUE_LED_PIN, LOW); // Ensure LED is off at startup

  // Initialize Button A
  pinMode(BUTTON_A_PIN, INPUT_PULLUP); // Use internal pull-up resistor
}

void loop() {
  // Read the state of Button A
  int buttonState = digitalRead(BUTTON_A_PIN);

  if (buttonState == LOW) { // Button is pressed (active LOW)
    digitalWrite(BLUE_LED_PIN, HIGH); // Turn on the blue LED
  } else {
    digitalWrite(BLUE_LED_PIN, LOW);  // Turn off the blue LED
  }

  delay(10); // Small delay to debounce the button
}
