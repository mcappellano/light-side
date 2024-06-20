#ifndef SENSORS_H
#define SENSORS_H

#include <Wire.h>
#include <Adafruit_SSD1306.h>

// Pin definitions
#define SWITCH PA4
#define DIGITAL_REFLECTANCE PA1

// OLED display
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
inline Adafruit_SSD1306 display_handler(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Function declarations
void handle_interrupt();
void checkSwitch();
void getReflectance();

// Variables
extern int counter;

#endif // SENSORS_H
