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

// OLED display dimensions
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET -1    // This display does not have a reset pin accessible

// Display handler object
inline Adafruit_SSD1306 display_handler(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Function declarations
void handle_interrupt();
void checkSwitch();
void checkReflectanceDigital();
void checkReflectanceAnalog();
void checkHomemadeReflectance();

// Variables
extern volatile int counter;
extern volatile int reflectanceThreshold;

#endif // MAIN_H