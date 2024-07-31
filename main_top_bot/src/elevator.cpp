#include "elevator.h"
#include "sweeper.h"
#include "drive.h"
#include "main.h"

const double ELEV_PULSE_DIST = 0.40906; // Previously 1.63625 (4x)
int elevCounter = 0;
volatile bool maxHeight = false;
volatile bool raising = false;
volatile bool elevStopped = true;
volatile bool retract = false;
volatile bool extend = false;
double previousFoodHeight = 35;
int elevPrevious = 0;

void raisePlatform(uint8_t dutyCycle, bool extendB)
{
    if (digitalRead(ELEV_SWITCH))
    {
        elevCounter = 0;
        raising = true;
        elevStopped = false;
        analogWrite(ELEV_MOTOR_UP, dutyCycle);
        analogWrite(ELEV_MOTOR_DOWN, 0);
        extend = extendB;
    }
}

void lowerPlatform(uint8_t dutyCycle, bool retractB)
{
    elevCounter = 0;
    elevStopped = false;
    raising = false;
    analogWrite(ELEV_MOTOR_UP, 0);
    analogWrite(ELEV_MOTOR_DOWN, dutyCycle);
    retract = retractB;
    if (retract)
        distanceToSweep = FULL_RETRACT_DIST;
}

void stopPlatform()
{
    analogWrite(ELEV_MOTOR_UP, 0);
    analogWrite(ELEV_MOTOR_DOWN, 0);
    if (retract)
    {
        retractSweeper(dcQuarter, false);
        retract = false;
    }
    else if (extend)
    {
        extendSweeper(dcQuarter);
        extend = false;
    }
    raising = false;
    elevStopped = true;
}

void elevSwitchInterrupt()
{
    if (raising)
        stopPlatform();
}

// IF EACH TIME WE GO DOWN WE GO 1 MM EXTRA, WE MAY HAVE TO ACCOUNT FOR THIS (by the end it could be 0.5 cm off)
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

    int stoppingTicks = 14;
    if (previousFoodHeight >= 4 && previousFoodHeight <= 5)
        stoppingTicks = 7;

    if (!raising && !elevStopped && (elevCounter <= (-previousFoodHeight / ELEV_PULSE_DIST) + stoppingTicks))
        stopPlatform();
    else if (raising && !elevStopped && (currentStation.equals(exchange) || currentStation.equals(cooktop)) && (elevCounter >= (previousFoodHeight / ELEV_PULSE_DIST)))
        stopPlatform();
}