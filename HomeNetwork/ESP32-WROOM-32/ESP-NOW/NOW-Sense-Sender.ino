#include <WiFi.h>
#include <esp_now.h>
#include <Wire.h>
#include <Adafruit_SH110X.h>
#include <Adafruit_SHT4x.h>
#include <esp_wifi.h>

// Define I2C addresses and pins
#define OLED_RESET -1  // No reset pin
Adafruit_SH1107 display = Adafruit_SH1107(64, 128, &Wire, OLED_RESET);
Adafruit_SHT4x sht4 = Adafruit_SHT4x();

#define BUTTON_A_PIN 14 // GPIO pin connected to Button A

// Receiver MAC address
uint8_t receiverMAC[] = {0x3C, 0x8A, 0x1F, 0xA7, 0x3F, 0x0C};

// Structure to hold data
typedef struct struct_message {
  int id;
  float temperature;
  float humidity;
} struct_message;

// Create a message instance
struct_message myData;

// MAC address and screen state
String macAddress;
bool screenOn = true;
bool isPressed = false;

// Function to get MAC address
String getMacAddress() {
  uint8_t baseMac[6];
  esp_err_t ret = esp_wifi_get_mac(WIFI_IF_STA, baseMac);
  if (ret == ESP_OK) {
    char macStr[18];
    snprintf(macStr, sizeof(macStr), "%02X:%02X:%02X:%02X:%02X:%02X",
             baseMac[0], baseMac[1], baseMac[2],
             baseMac[3], baseMac[4], baseMac[5]);
    return String(macStr);
  } else {
    return "Failed to read MAC";
  }
}

// Callback for ESP-NOW send
void onSent(const uint8_t *macAddr, esp_now_send_status_t status) {
  Serial.print("Last Packet Send Status: ");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Success" : "Fail");
}

void setup() {
  Serial.begin(115200);
  Wire.begin(21, 22);

  // Initialize Wi-Fi and get MAC address
  WiFi.mode(WIFI_STA);
  macAddress = getMacAddress();
  Serial.println("MAC Address: " + macAddress);

  // Initialize ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  esp_now_register_send_cb(onSent);
  esp_now_peer_info_t peerInfo;
  memcpy(peerInfo.peer_addr, receiverMAC, 6);
  peerInfo.channel = 0; // Default channel
  peerInfo.encrypt = false;

  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
    return;
  }

  // Initialize OLED
  if (!display.begin(0x3C, true)) {
    Serial.println("Failed to initialize OLED");
    while (1);
  }

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
  if (buttonState == LOW) {
    if (!isPressed) {
      isPressed = true;
      screenOn = !screenOn; // Toggle screen state
      Serial.println(screenOn ? "Screen ON" : "Screen OFF");
    }
  } else {
    isPressed = false;
  }

  // Read temperature and humidity
  sensors_event_t temp, humidity;
  if (sht4.getEvent(&humidity, &temp)) {
    myData.id = 1;
    myData.temperature = temp.temperature;
    myData.humidity = humidity.relative_humidity;

    // Send data via ESP-NOW
    esp_err_t result = esp_now_send(receiverMAC, (uint8_t *)&myData, sizeof(myData));
    if (result == ESP_OK) {
      Serial.println("Data sent successfully");
    } else {
      Serial.println("Failed to send data");
    }

    if (screenOn) {
      // Display data on OLED
      display.clearDisplay();
      display.setCursor(0, 0);
      display.print("Temp: ");
      display.print(temp.temperature);
      display.println(" C");
      display.print("Humidity: ");
      display.print(humidity.relative_humidity);
      display.println(" %");
      display.setCursor(0, 55);
      display.print("MAC:");
      display.print(macAddress);
      display.display();
    } else {
      // Turn off the screen
      display.clearDisplay();
      display.display();
    }

    Serial.print("Temp: ");
    Serial.print(temp.temperature);
    Serial.print(" C, Humidity: ");
    Serial.print(humidity.relative_humidity);
    Serial.println(" %");
  } else {
    Serial.println("Failed to read SHT41 data");
  }

  delay(2000);
}
