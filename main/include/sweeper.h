#ifndef SWEEPER_H
#define SWEEPER_H

// Library includes

// Pin definitions
#define SWEEP_SWITCH 66  // change this value
#define SWEEP_ENCODER 77 // change this value
#define SWEEP_MOTOR 100  // change this value

// Sweeper Constants - ALL distances in centimetres, measured with 0 being fully retracted, and calibrated using the switch at the fully extended position
#define EXTEND_POS 66             // VALUE NOT FINALIZED - The "reset" position
#define SWEEP_IN_POS 33           // VALUE NOT FINALIZED - The distance when the sweeper is in the position after sweeping in or pushing the plate out
#define FULLY_RETRACT_POS 0       // The distance when the sweeper is in the fully retracted position
#define SWITCH_PULSE_DISTANCE 0.1 // VALUE NOT FINALIZED - The distance that the sweeper moves for every pulse sent by the rotary encoder

// Variables
extern int sweeperPosition; // Using the rotary encoder, determine

// Function declarations
void extendSweeper();
void retractSweeper();
void stopSweeper();
void sweepSwitchInterrupt();

#endif // SWEEPER_H