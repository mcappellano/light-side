#ifndef MAIN_H
#define MAIN_H

#include <Wire.h>
#include <Adafruit_SSD1306.h>

// Pin definitions
#define REFLECTANCE 1

// OLED display
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET -1    // This display does not have a reset pin accessible
extern Adafruit_SSD1306 display_handler(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Food stations - BOTTOM ROBOT SPECIFIC
#define START_POSITION -1
#define TOMATO 0
#define EXCHANGE 1
#define COOKTOP 2
#define PLATES 3
#define CHEESE 10
#define LETTUCE 13

// Food stations - TOP ROBOT SPECIFIC (replace the above defines with these for the top robot code)
// #define START_POSITION -1
// #define PATTIES 1
// #define BUNS 3
// #define POTATOES 5
// #define TOMATO 10
// #define EXCHANGE 12
// #define COOKTOP 14
// #define PLATES 16

// Variables
extern int currentNode;
extern int nextNode;
extern int nextNextNode;
extern int tapeCounter;

// Function declarations
void setup();
void loop();

#endif // MAIN_H