#include "sweeper.h"
#include "main.h"
#include "elevator.h"

const double SWEEP_PULSE_DISTANCE = 0.6545; // PREVIOUSLY 0.6545 - previously 2.618 (4x) - The distance that the sweeper moves for every pulse sent by the rotary encoder
volatile int sweepCounter = 0;
int distanceToSweep = 0;
volatile bool extending = false;
volatile bool sweepStopped = true;
volatile bool slowed = false;
volatile bool swept = false;
bool resetSweepCount = true;
bool raiseA = false;
bool lower = false;
int sweepPrevious = 0;

void extendSweeper(uint8_t dutyCycle)
{
    if (digitalRead(SWEEP_SWITCH))
    {
        if (resetSweepCount)
            sweepCounter = 0;
        resetSweepCount = true;
        extending = true;
        slowed = false;
        sweepStopped = false;
        analogWrite(SWEEP_MOTOR_OUT, dutyCycle);
        analogWrite(SWEEP_MOTOR_BACK, 0);
    }
}

void retractSweeper(uint8_t dutyCycle, bool reset, bool raiseB, bool lowerB)
{
    swept = false;
    raiseA = raiseB;
    lower = lowerB;
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
    sweepStopped = true;
    extending = false;
    if (raiseA)
    {
        raisePlatform(63, false);
        raiseA = false;
    }
    if (lower)
    {
        lower = false;
        previousFoodHeight = 60; // VALUE NOT FINALIZED - Move entire burger from rim of plate to top out of the way
        lowerPlatform(63, true);
    }
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
        if (sweepCounter <= (-distanceToSweep / SWEEP_PULSE_DISTANCE) + 15)
        {
            stopSweeper();
            swept = true;
        }
        // else if (sweepCounter >= (currentStation.sweepLength / SWEEP_PULSE_DISTANCE) - 100)
        //     stopSweeper();
    }
    else if (extending && !slowed && !currentStation.equals(servingArea) && sweepCounter >= 160 / SWEEP_PULSE_DISTANCE)
    {
        extendSweeper(40);
        slowed = true;
    }
}