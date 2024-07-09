#ifndef SWEEPER_H
#define SWEEPER_H

// Library includes

// Pin definitions
#define SWEEP_SWITCH 66  // change this value
#define SWEEP_ENCODER 77 // change this value

// Variables

// Function declarations
void fullyExtend();
void sweepIn();
void fullyRetract();
void pushPlate();

#endif // SWEEPER_H