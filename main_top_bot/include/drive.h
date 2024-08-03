#ifndef DRIVE_H
#define DRIVE_H

// Library includes
#include <cmath>
#include <Arduino.h>
#include <array>

// Pin definitions
#define motor1F 21
#define motor1B 22
#define motor2F 8
#define motor2B 19
#define motor3F 7
#define motor3B 5
#define motor4F 10
#define motor4B 9

// Variables
extern hw_timer_t *crossTimer;
extern uint32_t freqHz;
extern uint8_t dcEighth;
extern uint8_t dc316;
extern uint8_t dcQuarter;
extern uint8_t dcThreeQs;
extern uint8_t speeds[4];

// Function declarations
void crossCounters();
void forceIntoCounter();
void crossCountersTape();
void IRAM_ATTR crossTimerInterrupt();
void setCrossTimer(int ms);
void calibrateDutyCycle(uint8_t dutyCycle);
void spinAround(uint8_t dutyCycle);

void driveForward(uint8_t dutyCycle);
void driveBackward(uint8_t dutyCycle);
void driveUpward(uint8_t dutyCycle);
void driveDownward(uint8_t dutyCycle);
void stopDriving();
void driveDiagonal();

#endif // DRIVE_H