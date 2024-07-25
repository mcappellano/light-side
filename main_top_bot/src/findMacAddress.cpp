// //for finding mac address of esp

// #include <Arduino.h>
// #include <WiFi.h>
// #include <esp_wifi.h>

// void readMacAddress() {
//   uint8_t baseMac[6];
//   esp_err_t ret = esp_wifi_get_mac(WIFI_IF_STA, baseMac);
//   if (ret == ESP_OK) {
//     Serial.printf("%02x:%02x:%02x:%02x:%02x:%02x\n",
//                   baseMac[0], baseMac[1], baseMac[2],
//                   baseMac[3], baseMac[4], baseMac[5]);
//   } else {
//     Serial.println("Failed to read MAC address");
//   }
// }
// //{0x64, 0xb7, 0x08, 0x9d, 0x44, 0xf8};

// void setup() {
//   Serial.begin(115200);

//   WiFi.mode(WIFI_STA);
//   WiFi.begin(); // No need for STA

//   Serial.print("[DEFAULT] ESP32 Board MAC Address: ");
//   readMacAddress();
// }

// void loop() {
//   // Do nothing here
// }