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
extern double previousFoodHeight; // For the plate, this height is the distance between the two platforms, not the height of the plate

// Function declarations
void raisePlatform(uint8_t dutyCycle);
void lowerPlatform(uint8_t dutyCycle);
void stopPlatform(); // Stop moving the platform up or down
void elevSwitchInterrupt();
void elevEncoderInterrupt();

#endif // ELEVATOR_H