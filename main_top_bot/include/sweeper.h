#ifndef SWEEPER_H
#define SWEEPER_H

// Library includes
#include <cmath>

// Pin definitions - not necessarily finalized
#define SWEEP_SWITCH 32
#define SWEEP_ENCODER_1 25
#define SWEEP_ENCODER_2 26
#define SWEEP_MOTOR_OUT 13
#define SWEEP_MOTOR_BACK 12

// Sweeper Constants - ALL distances in MILLIMITRES, measured with 0 being fully retracted, and calibrated using the switch at the fully extended position
#define EXTEND_POS 660      // VALUE NOT FINALIZED - The "reset" position
#define SWEEP_IN_POS 330    // VALUE NOT FINALIZED - The distance when the sweeper is in the position after sweeping in or pushing the plate out
#define FULLY_RETRACT_POS 0 // The distance when the sweeper is in the fully retracted position

// Variables
extern const double SWEEP_PULSE_DISTANCE;
extern double sweeperPosition;
extern volatile int sweepCounter;
extern volatile bool readyToLeave;
extern volatile bool extending;
extern volatile bool sweepStopped;

// Function declarations
void extendSweeper(uint8_t dutyCycle);
void retractSweeper(uint8_t dutyCycle, bool reset);
void stopSweeper();
void sweepSwitchInterrupt();
void sweepEncoderInterrupt();

#endif // SWEEPER_H