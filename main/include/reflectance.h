#ifndef REFLECTANCE_H
#define REFLECTANCE_H

// Includes
#include <Arduino.h>

// Pin definitions
#define REFLEC1 4
#define REFLEC2 15

// Variables
extern hw_timer_t *tapeTimer;
extern volatile bool alreadySeen;
const unsigned long tapedelay_ms = 250;

// Function declarations
void tapeInterrupt();
void tapeTimerInterrupt();

#endif // REFLECTANCE_H