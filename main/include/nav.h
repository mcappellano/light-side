#ifndef NAV_H
#define NAV_H

// Library includes
#include <cmath>

// Pin definitions

// Variables
extern volatile int tapeCounter;
extern volatile bool arrived;
extern int tapeToSee;

// Function declarations
void goNextStation();
void traverseCounter(bool forward, uint8_t driveSpeed, uint8_t reverseSpeed); // Enter true to go forwards, false to go backwards?
void goServe();

#endif // NAV_H