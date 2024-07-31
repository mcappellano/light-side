#include "sweeper.h"
#include "main.h"
#include "drive.h"

const double SWEEP_PULSE_DIST = 0.6545; // Previously 2.618 (4x) - The distance that the sweeper moves for every pulse sent by the rotary encoder
const int FULL_RETRACT_DIST = 327;
const int HALF_RETRACT_DIST = 210;
int distanceToSweep = HALF_RETRACT_DIST;
volatile int sweepCounter = 0;
volatile bool extending = false;
volatile bool sweepStopped = true;
volatile bool swept = false;
volatile bool served = false;
volatile bool slowed = false;
int sweepPrevious = 0;

void extendSweeper(uint8_t dutyCycle)
{
    if (digitalRead(SWEEP_SWITCH))
    {
        sweepCounter = 0;
        extending = true;
        sweepStopped = false;
        slowed = false;
        analogWrite(SWEEP_MOTOR_OUT, dutyCycle);
        analogWrite(SWEEP_MOTOR_BACK, 0);
    }
}

void retractSweeper(uint8_t dutyCycle, bool reset)
{
    swept = false;
    if (reset)
        sweepCounter = 0;

    sweepStopped = false;
    analogWrite(SWEEP_MOTOR_OUT, 0);
    analogWrite(SWEEP_MOTOR_BACK, dutyCycle);
}

void stopSweeper()
{
    analogWrite(SWEEP_MOTOR_OUT, 0);
    analogWrite(SWEEP_MOTOR_BACK, 0);
    extending = false;
    sweepStopped = true;
}

void sweepSwitchInterrupt()
{
    if (extending)
        stopSweeper();
}

void sweepEncoderInterrupt()
{
    int MSB = digitalRead(SWEEP_ENCODER_1);
    int LSB = digitalRead(SWEEP_ENCODER_2);

    int updatedEncoder = (MSB << 1) | LSB;
    int full = (updatedEncoder << 2) | sweepPrevious;

    if ((full == 0b0001) || (full == 0b1000) || (full == 0b0111) || (full == 0b1110))
        sweepCounter++;
    else if ((full == 0b0010) || (full == 0b0100) || (full == 0b1011) || (full == 0b1101))
        sweepCounter--;

    sweepPrevious = updatedEncoder;

    if (!extending && !sweepStopped)
    {
        if (sweepCounter <= (-distanceToSweep / SWEEP_PULSE_DIST) + 15)
        {
            stopSweeper();
            swept = true;
        }
    }
    else if (extending && !slowed && sweepCounter >= 270 / SWEEP_PULSE_DIST)
    {
        extendSweeper(dcEighth);
        slowed = true;
    }
}