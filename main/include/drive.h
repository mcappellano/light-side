#ifndef DRIVE_H
#define DRIVE_H

// Library includes
#include <cmath>

// Pin definitions
#define motor1F 22
#define motor1B 21
#define motor2F 8
#define motor2B 19
#define motor3F 7
#define motor3B 5
#define motor4F 10
#define motor4B 9

// Variables
extern uint32_t freqHz;
extern uint8_t resolution;
extern uint8_t dcQuarter;
extern uint8_t dcHalf;
extern uint8_t dcMax;

// Function declarations
void goNextNode();
void traverseForward();
void traverseBackward();
void crossUp();
void crossDown();
void spinAround();

void motorsForward(uint8_t dutyCycle);
void motorsBackward(uint8_t dutyCycle);
void motorsUpwards(uint8_t dutyCycle);
void motorsDownwards(uint8_t dutyCycle);
void stopMotors();

#endif // DRIVE_H