#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>
#include <Preferences.h> // Include Preferences library for persistence

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

// OLED display object
Adafruit_SH1107 display(SCREEN_HEIGHT, SCREEN_WIDTH, &Wire, -1);

// GPIO pins
#define BLUE_LED_PIN 2
#define BUTTON_A_PIN 14 // Replace with the actual GPIO pin connected to Button A

// Timing thresholds for Morse code
#define DOT_THRESHOLD 300   // Press duration < 300 ms is a dot
#define DASH_THRESHOLD 300  // Press duration >= 300 ms is a dash
#define LETTER_PAUSE 1000   // Time between letters (1 second)

// Preferences object for persistent storage
Preferences preferences;

String morseInput = "";     // Store Morse code
String translatedText = ""; // Store translated text

void setup() {
  Serial.begin(115200);
  Serial.println("Starting program...");
  Wire.begin(21, 22);

  // Initialize the OLED
  if (!display.begin(0x3C, true)) {
    Serial.println("Failed to initialize OLED");
    while (1);
  }

  // Set up the OLED display
  display.setRotation(1);
  display.clearDisplay();
  display.setTextColor(SH110X_WHITE);
  display.setTextSize(1);

  // Initialize GPIO pins
  pinMode(BLUE_LED_PIN, OUTPUT);
  digitalWrite(BLUE_LED_PIN, LOW);
  pinMode(BUTTON_A_PIN, INPUT_PULLUP);

  // Load persisted translated text
  preferences.begin("morse", false); // Namespace "morse", read/write mode
  translatedText = preferences.getString("text", ""); // Default to empty
  Serial.println("Loaded Translated Text: " + translatedText);

  // Display the loaded text
  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("Loaded Text:");
  display.println(translatedText);
  display.display();
}

void loop() {
  static unsigned long pressStartTime = 0;
  static bool isPressed = false;

  // Read the button state
  int buttonState = digitalRead(BUTTON_A_PIN);

  if (buttonState == LOW) { // Button is pressed
    if (!isPressed) { // Detect new press
      pressStartTime = millis();
      isPressed = true;
    }
    digitalWrite(BLUE_LED_PIN, HIGH); // Turn on the blue LED
  } else if (isPressed) { // Button was released
    unsigned long pressDuration = millis() - pressStartTime;

    // Determine dot or dash
    if (pressDuration < DOT_THRESHOLD) {
      morseInput += ".";
    } else {
      morseInput += "-";
    }

    isPressed = false;
    digitalWrite(BLUE_LED_PIN, LOW); // Turn off the blue LED

    // Display the current Morse code on the OLED
    display.clearDisplay();
    display.setCursor(0, 0);
    display.println("Morse Input:");
    display.println(morseInput);
    display.display();
  }

  // Detect letter pause
  static unsigned long lastPressTime = 0;
  if (isPressed) {
    lastPressTime = millis();
  } else if (millis() - lastPressTime > LETTER_PAUSE && !morseInput.isEmpty()) {
    // Translate Morse code to text
    translatedText += translateMorse(morseInput);
    morseInput = ""; // Clear Morse input

    // Save the translated text persistently
    preferences.putString("text", translatedText);

    // Display translated text on the OLED
    display.clearDisplay();
    display.setCursor(0, 0);
    display.println("Translated Text:");
    display.println(translatedText);
    display.display();
  }
}

// Translate Morse code to English letters
char translateMorse(String morse) {
  if (morse == ".-") return 'A';
  if (morse == "-...") return 'B';
  if (morse == "-.-.") return 'C';
  if (morse == "-..") return 'D';
  if (morse == ".") return 'E';
  if (morse == "..-.") return 'F';
  if (morse == "--.") return 'G';
  if (morse == "....") return 'H';
  if (morse == "..") return 'I';
  if (morse == ".---") return 'J';
  if (morse == "-.-") return 'K';
  if (morse == ".-..") return 'L';
  if (morse == "--") return 'M';
  if (morse == "-.") return 'N';
  if (morse == "---") return 'O';
  if (morse == ".--.") return 'P';
  if (morse == "--.-") return 'Q';
  if (morse == ".-.") return 'R';
  if (morse == "...") return 'S';
  if (morse == "-") return 'T';
  if (morse == "..-") return 'U';
  if (morse == "...-") return 'V';
  if (morse == ".--") return 'W';
  if (morse == "-..-") return 'X';
  if (morse == "-.--") return 'Y';
  if (morse == "--..") return 'Z';
  return '?'; // Unknown Morse code
}
