#ifndef MAIN_H
#define MAIN_H

#include <Wire.h>
#include <Arduino.h>
#include <iostream>
#include <map>
#include <string>

// Class
class Station
{
public:
    Station(int num, double height, int sweepLength);

    int num;
    double height;
    int sweepLength;
};

// Variables
extern Station currentStation;
extern Station nextStation;
extern Station nextNextStation;
extern std::map<int, Station> numsToStation;

// Function declarations
void setup();
void loop();

#endif // MAIN_H

/*
Unneeded OLED code:
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET -1    // This display does not have a reset pin accessible
extern Adafruit_SSD1306 display_handler;
*/