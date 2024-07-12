#ifndef ELEVATOR_H
#define ELEVATOR_H

// Library includes
#include <cmath>

// Pin definitions - not necessarily finalized - also need to add the servos!
#define ELEV_SWITCH 39
#define ELEV_ENCODER_1 25
#define ELEV_ENCODER_2 26
#define ELEV_MOTOR_UP 35
#define ELEV_MOTOR_DOWN 34

// Elevator Constants - ALL heights/distances in centimetres, measuring how far the platform is below the max height in which the switch is pressed
#define MAX_HEIGHT 50 // VALUE NOT FINALIZED - The "reset" position, other values are meaured from this point
#define MIN_HEIGHT 0  // As a safety, this will prevent the platform from trying to lower further than possible
#define PATTY_HEIGHT 1
#define TOMATO_HEIGHT 0.45
#define CHEESE_HEIGHT 0.43
#define LETTUCE_HEIGHT 0.43
#define BOTTOM_BUN_HEIGHT 1.5 // No need for top bun height, as it is the last burger item

// Variables - Measured the same way as the constants above (except for previousFoodHeight)
extern const float ELEV_PULSE_DISTANCE; // VALUE NOT FINALIZED - The distance that the platform moves for every pulse sent by the rotary encoder
extern int elevCounter;
extern double platformHeight;
extern double previousHeight;
extern double previousFoodHeight; // For the plate, this height is the distance between the two platforms, not the height of the plate

// Function declarations
void raisePlatform(uint8_t dutyCycle);
void lowerPlatform(uint8_t dutyCycle);
void stopPlatform(); // stop moving the platform up or down
void swingOut();
void swingIn(); // bring the platform back from the swung out position
void elevSwitchInterrupt();
void elevEncoderInterrupt();

void testElevator();

#endif // ELEVATOR_H