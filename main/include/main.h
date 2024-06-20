#ifndef MAIN_H
#define MAIN_H

#include <Wire.h>
#include <Adafruit_SSD1306.h>

// Pin definitions
#define SWITCH PA4
#define DIGITAL_REFLECTANCE PA1
#define REFLEC_POT PA2
#define ANALOG_REFLECTANCE PA7
#define HOMEMADE_REFLECTANCE PA5

// OLED display
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET -1    // This display does not have a reset pin accessible
inline Adafruit_SSD1306 display_handler(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Function declarations

// Variables

#endif // MAIN_H