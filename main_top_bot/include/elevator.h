#ifndef ELEVATOR_H
#define ELEVATOR_H

// Library includes
#include <cmath>

// Pin definitions
#define ELEV_SWITCH 20
#define ELEV_ENCODER_1 38
#define ELEV_ENCODER_2 37
#define ELEV_MOTOR_UP 12   // 27
#define ELEV_MOTOR_DOWN 13 // 14
#define SERVO_PLATFORM 1

// Variables
extern const double ELEV_PULSE_DIST; // The distance in mm that the platform moves for every pulse sent by the rotary encoder
extern const int DROP_DIST;
extern const int FULL_DROP_DIST;
extern int elevCounter;
extern volatile bool raising;
extern volatile bool maxHeight;
extern int var1;
extern double previousFoodHeight;

// Function declarations
void raisePlatform(uint8_t dutyCycle, bool extendB);
void lowerPlatform(uint8_t dutyCycle, bool retractB);
void stopPlatform(); // Stop moving the platform up or down
void elevSwitchInterrupt();
void elevEncoderInterrupt();

#endif // ELEVATOR_H