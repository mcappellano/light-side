#ifndef ELEVATOR_H
#define ELEVATOR_H

// Library includes

// Pin definitions
#define ELEV_SWITCH 88  // change this value
#define ELEV_ENCODER 99 // change this value

// Variables

// Function declarations
void raiseToTop();
void lowerPlatform(int height); // corresponding to the height of the food item we just picked up
void dropBurger();
void returnPlatform(); // bring the platform back from the swung out position

#endif // ELEVATOR_H