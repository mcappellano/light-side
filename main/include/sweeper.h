#ifndef SWEEPER_H
#define SWEEPER_H

// Library includes
#include <cmath>

// Pin definitions - not necessarily finalized
#define SWEEP_SWITCH 36
#define SWEEP_ENCODER_1 32
#define SWEEP_ENCODER_2 33
#define SWEEP_MOTOR_OUT 12
#define SWEEP_MOTOR_BACK 13

// Sweeper Constants - ALL distances in MILLIMITRES, measured with 0 being fully retracted, and calibrated using the switch at the fully extended position
#define EXTEND_POS 660            // VALUE NOT FINALIZED - The "reset" position
#define SWEEP_IN_POS 330          // VALUE NOT FINALIZED - The distance when the sweeper is in the position after sweeping in or pushing the plate out
#define FULLY_RETRACT_POS 0       // The distance when the sweeper is in the fully retracted position
#define SWITCH_PULSE_DISTANCE 0.1 // VALUE NOT FINALIZED - The distance that the sweeper moves for every pulse sent by the rotary encoder

// Variables
extern double sweeperPosition;
extern int sweepCounter;

// Function declarations
void extendSweeper(uint8_t dutyCycle);
void retractSweeper(uint8_t dutyCycle);
void stopSweeper();
void sweepSwitchInterrupt();
void sweepEncoderInterrupt();

#endif // SWEEPER_H