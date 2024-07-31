#ifndef REFLECTANCE_H
#define REFLECTANCE_H

// Includes
#include <Arduino.h>

// Pin definitions
#define REFLEC1 4
#define REFLEC2 15
#define REFLEC3 25
#define REFLEC4 26

// Variables
extern hw_timer_t *tapeTimer;
extern volatile bool alreadySeen;
extern const unsigned long tapedelay_ms;

// Function declarations
void tapeInterrupt();
void tapeTimerInterrupt();

#endif // REFLECTANCE_H