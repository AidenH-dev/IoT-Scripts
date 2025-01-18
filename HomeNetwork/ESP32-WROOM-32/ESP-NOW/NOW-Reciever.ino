#include <WiFi.h>
#include <esp_now.h>

// Structure to hold received data
typedef struct struct_message {
  int id;
  float temperature;
  float humidity;
} struct_message;

// Create a message instance
struct_message myData;

// Callback function for receiving data
void onReceive(const esp_now_recv_info_t *recvInfo, const uint8_t *data, int dataLen) {
  Serial.print("Received data from: ");
  char macStr[18];
  snprintf(macStr, sizeof(macStr), "%02X:%02X:%02X:%02X:%02X:%02X",
           recvInfo->src_addr[0], recvInfo->src_addr[1], recvInfo->src_addr[2],
           recvInfo->src_addr[3], recvInfo->src_addr[4], recvInfo->src_addr[5]);
  Serial.println(macStr);

  memcpy(&myData, data, sizeof(myData));

  // Print received data
  Serial.print("ID: ");
  Serial.println(myData.id);
  Serial.print("Temperature: ");
  Serial.println(myData.temperature);
  Serial.print("Humidity: ");
  Serial.println(myData.humidity);
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

  // Register receive callback
  esp_now_register_recv_cb(onReceive);
}

void loop() {
  // Do nothing, waiting for data
}
