#include <Arduino.h>
#include "USB.h"
#include "USBHIDKeyboard.h"

USBHIDKeyboard Keyboard;

void setup() {
    USB.begin();      // Initialize USB
    Keyboard.begin(); // Start HID Keyboard
    delay(3000);      // Wait for USB enumeration

    // Open Spotlight (Cmd + Space)
    Keyboard.press(KEY_LEFT_GUI);
    Keyboard.press(' ');
    Keyboard.releaseAll();
    delay(500);

    // Type "Terminal" and open it
    Keyboard.println("Google Chrome");
    delay(500);
    Keyboard.press(KEY_RETURN);
    Keyboard.releaseAll();
    delay(1500); // Wait for Terminal to open
    
    // 🔹 Open a new tab (Cmd + T)
    Keyboard.press(KEY_LEFT_GUI);
    Keyboard.press('t');
    Keyboard.releaseAll();
    delay(500);

    // 🔹 Type the URL
    Keyboard.println("https://static.wikia.nocookie.net/5a2286a5-ce45-4bbf-b4d7-bd28563441bb/scale-to-width/755");
    delay(500);

    // 🔹 Press Enter to navigate
    Keyboard.press(KEY_RETURN);
    Keyboard.releaseAll();
}

void loop() {
    // HID executes once, no need for a loop
}


