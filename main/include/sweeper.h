#ifndef SWEEPER_H
#define SWEEPER_H

// Library includes
#include <cmath>

// Pin definitions - not necessarily finalized
#define SWEEP_SWITCH 36
#define SWEEP_ENCODER_1 25
#define SWEEP_ENCODER_2 26
#define SWEEP_MOTOR_OUT 14  // 12
#define SWEEP_MOTOR_BACK 27 // 13

// Variables
extern const double SWEEP_PULSE_DISTANCE;
extern double sweeperPosition;
extern volatile int sweepCounter;
extern volatile bool extending;
extern volatile bool sweepStopped;
extern volatile bool swept;

// Function declarations
void extendSweeper(uint8_t dutyCycle);
void retractSweeper(uint8_t dutyCycle, bool reset);
void stopSweeper();
void sweepSwitchInterrupt();
void sweepEncoderInterrupt();

#endif // SWEEPER_H