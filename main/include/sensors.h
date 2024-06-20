#ifndef SENSORS_H
#define SENSORS_H

#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <Arduino.h>

// Pin definitions
#define SWITCH 4
#define DIGITAL_REFLECTANCE 1

// Function declarations
void handle_interrupt();
void checkSwitch();
void getReflectance();

// Variables
extern int counter;

#endif // SENSORS_H
