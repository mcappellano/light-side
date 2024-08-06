#include "elevator.h"
#include "sweeper.h"
#include "main.h"

const double ELEV_PULSE_DISTANCE = 0.40906; // Millimitres - previously 1.63625 (4x)
int elevCounter = 0;
volatile bool raising = false;
volatile bool elevStopped = true;
double previousFoodHeight = 11.5;
int elevPrevious = 0;
bool retract = false;
bool retractD = false;
volatile bool raisePartial = false;

void raisePlatform(uint8_t dutyCycle, bool retractC)
{
    if (digitalRead(ELEV_SWITCH))
    {
        elevCounter = 0;
        raising = true;
        elevStopped = false;
        retractD = retractC;
        analogWrite(ELEV_MOTOR_UP, dutyCycle);
        analogWrite(ELEV_MOTOR_DOWN, 0);
    }
}

void lowerPlatform(uint8_t dutyCycle, bool retractB)
{
    elevCounter = 0;
    elevStopped = false;
    analogWrite(ELEV_MOTOR_UP, 0);
    analogWrite(ELEV_MOTOR_DOWN, dutyCycle);
    retract = retractB;
}

void stopPlatform()
{
    analogWrite(ELEV_MOTOR_UP, 0);
    analogWrite(ELEV_MOTOR_DOWN, 0);
    raising = false;
    elevStopped = true;
    if (retract)
    {
        retract = false;
        distanceToSweep = servingArea.sweepLength;
        retractSweeper(63, false, true, false);
    }
    if (retractD)
    {
        retractD = false;
        distanceToSweep = 225;
        retractSweeper(63, false, false, true);
    }
}

void elevSwitchInterrupt()
{
    if (raising)
        stopPlatform();
}

void elevEncoderInterrupt()
{
    int MSB = digitalRead(ELEV_ENCODER_1);
    int LSB = digitalRead(ELEV_ENCODER_2);

    int updatedEncoder = (MSB << 1) | LSB;
    int full = (updatedEncoder << 2) | elevPrevious;

    if ((full == 0b0001) || (full == 0b1000) || (full == 0b0111) || (full == 0b1110))
        elevCounter++;
    else if ((full == 0b0010) || (full == 0b0100) || (full == 0b1011) || (full == 0b1101))
        elevCounter--;

    elevPrevious = updatedEncoder;

    int stoppingTicks = 7;
    if (previousFoodHeight > 5)
        stoppingTicks = 10;
    if (previousFoodHeight > 12)
        stoppingTicks = 14;

    if (!raising && !elevStopped && (elevCounter <= (-previousFoodHeight / ELEV_PULSE_DISTANCE) + stoppingTicks))
        stopPlatform();
    // else if (raising && currentStation.equals(servingArea) && elevCounter >= 84 / ELEV_PULSE_DISTANCE - 7)
    //     stopPlatform();
    else if (raisePartial && !elevStopped && (elevCounter >= (41 / ELEV_PULSE_DISTANCE) - 7))
    {
        stopPlatform();
        raisePartial = false;
    }
}
