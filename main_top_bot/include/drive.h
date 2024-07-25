#ifndef DRIVE_H
#define DRIVE_H

// Library includes
#include <cmath>
#include <Arduino.h>
#include <array>

// Pin definitions
#define motor1F 22
#define motor1B 21
#define motor2F 19
#define motor2B 8
#define motor3F 5
#define motor3B 7
#define motor4F 9
#define motor4B 10

// Variables
extern hw_timer_t *accelTimer;
extern uint32_t freqHz;
// extern uint8_t dcSixteenth;
// extern uint8_t dcMin;
extern uint8_t dcEighth;
extern uint8_t dc316;
extern uint8_t dcQuarter;
// extern uint8_t dcHalf;
extern uint8_t dcThreeQs;
// extern uint8_t dcMax;
extern uint8_t speeds[4];

extern volatile double loopNum;

// Function declarations
void crossCounters();
void crossCountersCool();
void changingPWMs(uint8_t motor1Start, uint8_t motor1End, bool forw1, uint8_t motor2Start, uint8_t motor2End, bool forw2, uint8_t motor3Start, uint8_t motor3End, bool forw3, uint8_t motor4Start, uint8_t motor4End, bool forw4, int loops);
void calibrateDutyCycle(uint8_t dutyCycle);
void spinAround(uint8_t dutyCycle);

void driveForward(uint8_t dutyCycle);
void driveBackward(uint8_t dutyCycle);
void driveForward2(uint8_t dutyCycle);
void driveBackward2(uint8_t dutyCycle);
void IRAM_ATTR accelTimerInterrupt();
void driveUpward(uint8_t dutyCycle);
void driveDownward(uint8_t dutyCycle);
void stopDriving();

#endif // DRIVE_H