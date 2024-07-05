// //This was the first test for slave recieving potentiometer reading from master and displaying pot reading on display

// #include <Arduino.h>
// #include <Wire.h>
// #include <Adafruit_SSD1306.h>
// #include <esp_now.h>
// #include <WiFi.h>

// #define SCREEN_WIDTH 128 // OLED display width, in pixels
// #define SCREEN_HEIGHT 64 // OLED display height, in pixels
// #define OLED_RESET 	-1 // This display does not have a reset pin accessible

// Adafruit_SSD1306 display_handler(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// void display (int toDisplay ){
//   display_handler.clearDisplay();
//   display_handler.setTextSize(1);
//   display_handler.setTextColor(SSD1306_WHITE);
//   display_handler.setCursor(0,0);
//   display_handler.println(toDisplay);
//   display_handler.display();
// }

// // Structure example to receive data
// typedef struct struct_message {
//     int potValue;
// } struct_message;

// // Create a struct_message to hold incoming pot readings
// struct_message incomingReadings;

// // Callback when data is received
// void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
//   memcpy(&incomingReadings, incomingData, sizeof(incomingReadings));
//   Serial.print("Bytes received: ");
//   Serial.println(len);
//   Serial.print("Potentiometer Value: ");
//   Serial.println(incomingReadings.potValue);
//   display(incomingReadings.potValue);

// }

// void setup() {
//   // Init Serial Monitor
//   Serial.begin(115200);

//   display_handler.begin(SSD1306_SWITCHCAPVCC, 0x3C);

//     //display setup test
//   display_handler.clearDisplay();
//   display_handler.setTextSize(1);
//   display_handler.setTextColor(SSD1306_WHITE);
//   display_handler.setCursor(0,0);
//   display_handler.println("Jordane not lit");
//   display_handler.display();
//   Serial.printf("tried to display");
//   delay (100);

//   // Set device as a Wi-Fi Station
//   WiFi.mode(WIFI_STA);

//   // Init ESP-NOW
//   if (esp_now_init() != ESP_OK) {
//     Serial.println("Error initializing ESP-NOW");
//     return;
//   }

//   // Register for a callback function that will be called when data is received
//   esp_now_register_recv_cb(OnDataRecv);
// }

// void loop() {

// }
