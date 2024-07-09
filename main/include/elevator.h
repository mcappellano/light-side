#ifndef ELEVATOR_H
#define ELEVATOR_H

// Library includes

// Pin definitions
#define ELEV_SWITCH 88  // change this value
#define ELEV_ENCODER 99 // change this value
#define ELEV_MOTOR 111  // change this value

// Elevator Constants - ALL distances in centimetres, measuring how far the platform is below the max height in which the switch is pressed
#define MAX_HEIGHT 50           // VALUE NOT FINALIZED - The "reset" position, other values are meaured from this point
#define MIN_HEIGHT 0            // As a safety, this will prevent the platform from trying to lower further than possible
#define ELEV_PULSE_DISTANCE 0.1 // VALUE NOT FINALIZED - The distance that the platform moves for every pulse sent by the rotary encoder
#define PATTY_HEIGHT
#define TOMATO_HEIGHT
#define CHEESE_HEIGHT
#define LETTUCE_HEIGHT
#define BOTTOM_BUN_HEIGHT // No need for top bun height, as it is the last burger item

// Variables - Measured the same way as the constants above (except for previousFoodHeight)
extern int platformHeight;
extern int previousHeight;
extern int previousFoodHeight; // For the plate, this height is the distance between the two platforms, not the height of the plate

// Function declarations
void raisePlatform();
void lowerPlatform();
void stopPlatform(); // stop moving the platform up or down
void swingOut();
void swingIn(); // bring the platform back from the swung out position
void elevSwitchInterrupt();

#endif // ELEVATOR_H