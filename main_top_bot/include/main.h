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
    Station(int num, double height, int sweepLength, int item);

    int num;
    double height;
    int sweepLength;
    int item;

    bool equals(const Station &other) const;
};

// Defines
#define BOTTOM_BUN 0
#define TOP_BUN 1
#define PATTY 2
#define FRY 3
#define EMPTY 4
#define NA 5

// Variables
extern std::map<int, Station> numsToStation;
extern Station start;
extern Station patties;
extern Station buns;
extern Station potatoes;
extern Station tomatoes;
extern Station exchange;
extern Station cooktop;
extern Station plates;
extern Station currentStation;
extern Station nextStation;
extern int node;

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