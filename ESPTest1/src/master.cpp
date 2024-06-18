//First test code for esp communication between this master reading pot value and transmitting to slave to display

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <esp_now.h>
#include <WiFi.h>

// Potentiometer is connected to GPIO 36
const int potPin = 36;

//MAC Address of your slave //mac address of this one {0x64, 0xb7, 0x08, 0x9c, 0x61, 0x08}
uint8_t broadcastAddress[] = {0x64, 0xb7, 0x08, 0x9d, 0x44, 0xf8};

// Structure example to send data
typedef struct struct_message {
    int potValueR;
} struct_message;

// Create a struct_message to hold pot readings
struct_message potValueToSend;

// Peer information
esp_now_peer_info_t peerInfo;

// Callback when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  //Serial.print("\r\nLast Packet Send Status:\t");
  //Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

void setup() {
  // Init Serial Monitor
  Serial.begin(115200);

  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Register for send callback to get the status of transmitted packet
  esp_now_register_send_cb(OnDataSent);
  
  // Register peer
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;
  
  // Add peer
  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
    return;
  }
}

void loop() {
  // Reading potentiometer value
  int potValueR = analogRead(potPin);
  Serial.println(potValueR);
  potValueToSend.potValueR = potValueR;

  // Send message via ESP-NOW
  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &potValueToSend, sizeof(potValueToSend));
   
  if (result == ESP_OK) {
    //Serial.println("Sent with success");
  } else {
    Serial.println("Error sending the data");
  }
  
  delay(100);
}
