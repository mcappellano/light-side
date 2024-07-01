#ifndef SENSORS_H
#define SENSORS_H

#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <Arduino.h>

// Variables
extern int counter;

// Function declarations
void handle_interrupt();
void checkSwitch();
void getReflectance();

#endif // SENSORS_H
