#ifndef DRIVE_H
#define DRIVE_H

// Library includes
#include <cmath>
#include <Arduino.h>

// Pin definitions
#define motor1F 22
#define motor1B 21
#define motor2F 8
#define motor2B 19
#define motor3F 5
#define motor3B 7
#define motor4F 10
#define motor4B 9

// Variables
extern uint32_t freqHz;
extern uint8_t dcQuarter;
extern uint8_t dcHalf;
extern uint8_t dcMax;
extern hw_timer_t *arrivalCheckTimer;
extern volatile bool arrived;

// Function declarations
void arrivalCheckInterrupt();

void goNextNode();
void traverseForward();
void traverseBackward();
void crossCounters();
void spinAround();

void driveForward(uint8_t dutyCycle);
void driveBackward(uint8_t dutyCycle);
void driveUpward(uint8_t dutyCycle);
void driveDownward(uint8_t dutyCycle);
void stopDriving();

#endif // DRIVE_H