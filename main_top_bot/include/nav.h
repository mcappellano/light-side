#ifndef NAV_H
#define NAV_H

// Library includes
#include <cmath>
#include <Arduino.h>

// Pin definitions

// Variables
extern volatile int tapeCounter;
extern volatile bool arrived;
extern int tapeToSee;
extern hw_timer_t *slowDownTimer;
extern bool adjusted;

// Function declarations
void goNextStation();
void traverseCounter(bool forward, uint8_t driveSpeed, uint8_t reverseSpeed); // Enter true to go forwards, false to go backwards?
void handleEdgeCases();
void goServe();
void slowDownTimerInterrupt();

#endif // NAV_H