#ifndef SWEEPER_H
#define SWEEPER_H

// Library includes
#include <cmath>

// Pin definitions - not necessarily finalized
#define SWEEP_SWITCH 32
#define SWEEP_ENCODER_1 35 // 35
#define SWEEP_ENCODER_2 34 // 34
#define SWEEP_MOTOR_OUT 14
#define SWEEP_MOTOR_BACK 27

// Variables - distances in millimitres
extern const double SWEEP_PULSE_DIST;
extern const int FULL_RETRACT_DIST;
extern const int HALF_RETRACT_DIST;
extern int distanceToSweep;
extern volatile int sweepCounter;
extern volatile bool extending;
extern volatile bool sweepStopped;
extern volatile bool swept;
extern volatile bool served;

// Function declarations
void extendSweeper(uint8_t dutyCycle);
void retractSweeper(uint8_t dutyCycle, bool reset);
void stopSweeper();
void sweepSwitchInterrupt();
void sweepEncoderInterrupt();

#endif // SWEEPER_H