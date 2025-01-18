#include <WiFi.h>
#include <esp_wifi.h>

void readMacAddress() {
  uint8_t baseMac[6];
  esp_err_t ret = esp_wifi_get_mac(WIFI_IF_STA, baseMac);
  if (ret == ESP_OK) {
    Serial.printf("%02X:%02X:%02X:%02X:%02X:%02X\n",
                  baseMac[0], baseMac[1], baseMac[2],
                  baseMac[3], baseMac[4], baseMac[5]);
  } else {
    Serial.println("Failed to read MAC address");
  }
}

void setup() {
  Serial.begin(115200);
  delay(1000); // Allow time for Serial initialization

  WiFi.mode(WIFI_STA); // Set Wi-Fi to Station mode

  Serial.println("ESP32 MAC Address Reader");
}

void loop() {
  Serial.print("Current MAC Address: ");
  readMacAddress();
  delay(2000); // Wait for 2 seconds before the next reading
}