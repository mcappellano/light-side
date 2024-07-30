#ifndef NAV_H
#define NAV_H

// Library includes
#include <cmath>
#include <Arduino.h>

// Pin definitions

// Variables
extern hw_timer_t *slowDownTimer;
extern volatile int tapeCounter;
extern volatile bool arrived;
extern int tapeToSee;
extern bool adjusted;

// Function declarations
void goNextStation();
void traverseCounter(bool forward, uint8_t driveSpeed, uint8_t reverseSpeed);
void exchangeItem();
void slowDownTimerInterrupt();
// void handleEdgeCases();

#endif // NAV_H