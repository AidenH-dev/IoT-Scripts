#include <WiFi.h>
#include <esp_now.h>

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

void onSent(const uint8_t *macAddr, esp_now_send_status_t status) {
  Serial.print("Last Packet Send Status: ");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Success" : "Fail");
}

void setup() {
  Serial.begin(115200);

  // Initialize WiFi
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Register send callback
  esp_now_register_send_cb(onSent);

  // Add peer
  esp_now_peer_info_t peerInfo;
  memcpy(peerInfo.peer_addr, receiverMAC, 6);
  peerInfo.channel = 0; // Default channel
  peerInfo.encrypt = false;

  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
    return;
  }
}

void loop() {
  // Simulate data
  myData.id = 1;
  myData.temperature = random(20, 30);
  myData.humidity = random(50, 70);

  // Send message
  esp_err_t result = esp_now_send(receiverMAC, (uint8_t *)&myData, sizeof(myData));

  Serial.print("Sending data: ");
  Serial.print(myData.temperature);
  Serial.print(", ");
  Serial.println(myData.humidity);

  if (result == ESP_OK) {
    Serial.println("Sent successfully");
  } else {
    Serial.println("Send failed");
  }

  delay(2000); // Wait before sending again
}
