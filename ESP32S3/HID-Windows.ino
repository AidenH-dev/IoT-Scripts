#include <Arduino.h>
#include "USB.h"
#include "USBHIDKeyboard.h"

USBHIDKeyboard Keyboard;

void setup() {
    USB.begin();      // Initialize USB
    Keyboard.begin(); // Start HID Keyboard
    delay(3000);      // Wait for USB enumeration

    // Open Run dialog (Win + R)
    Keyboard.press(KEY_LEFT_GUI);
    Keyboard.press('r');
    Keyboard.releaseAll();
    delay(500);

    // Type "microsoft-edge:https://your-url.com" to open Edge directly to a URL
    Keyboard.println("microsoft-edge:https://static.wikia.nocookie.net/5a2286a5-ce45-4bbf-b4d7-bd28563441bb/scale-to-width/755");
    delay(500);

    // Press Enter to execute
    Keyboard.press(KEY_RETURN);
    Keyboard.releaseAll();
}

void loop() {
    // HID executes once, no need for a loop
}
