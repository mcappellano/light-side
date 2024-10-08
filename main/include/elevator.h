#ifndef ELEVATOR_H
#define ELEVATOR_H

// Library includes
#include <cmath>

// Pin definitions - not necessarily finalized - also need to add the servos!
#define ELEV_SWITCH 39
#define ELEV_ENCODER_1 32
#define ELEV_ENCODER_2 33
#define ELEV_MOTOR_UP 12   // 27
#define ELEV_MOTOR_DOWN 13 // 14
#define SERVO_PLATFORM 1

// Variables - Measured the same way as the constants above (except for previousFoodHeight)
extern const double ELEV_PULSE_DISTANCE; // The distance that the platform moves for every pulse sent by the rotary encoder
extern int elevCounter;
extern volatile bool raising;
extern int var1;
extern double previousFoodHeight;
extern volatile bool elevStopped;
extern volatile bool raisePartial;
extern volatile bool serveReady;

// Function declarations
void raisePlatform(uint8_t dutyCycle, bool retractC);
void lowerPlatform(uint8_t dutyCycle, bool retractB);
void stopPlatform(); // Stop moving the platform up or down
void elevSwitchInterrupt();
void elevEncoderInterrupt();

#endif // ELEVATOR_H