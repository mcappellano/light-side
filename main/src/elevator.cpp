#include "elevator.h"
#include "main.h"

const double ELEV_PULSE_DISTANCE = 0.40906; // millimitres - previously 1.63625 (4x)
int elevCounter = 0;
volatile bool maxHeight = false;
volatile bool raising = false;
volatile bool elevStopped = true;
double previousFoodHeight = 23.175; // For testing, but I don't believe it matters what we initialize this to
int elevPrevious = 0;

void raisePlatform(uint8_t dutyCycle)
{
    elevCounter = 0;
    raising = true;
    elevStopped = false;
    analogWrite(ELEV_MOTOR_UP, dutyCycle);
    analogWrite(ELEV_MOTOR_DOWN, 0);
}

void lowerPlatform(uint8_t dutyCycle)
{
    elevCounter = 0;
    elevStopped = false;
    analogWrite(ELEV_MOTOR_UP, 0);
    analogWrite(ELEV_MOTOR_DOWN, dutyCycle);
}

void stopPlatform()
{
    analogWrite(ELEV_MOTOR_UP, 0);
    analogWrite(ELEV_MOTOR_DOWN, 0);
}

void swingOut()
{
    // analogWrite(SERVO_PLATFORM, X);
    // delay(Y);
    // analogWrite(Z);
}

void swingIn()
{
    // analogWrite(X);
}

void elevSwitchInterrupt()
{
    if (raising)
    {
        stopPlatform();
        raising = false;
    }
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

    if (!raising && !elevStopped && (elevCounter <= (-previousFoodHeight / ELEV_PULSE_DISTANCE) + stoppingTicks))
    {
        stopPlatform();
        elevStopped = true;
    }

    // Possibility for error: It will never raise more than 57.5 mm. But we shouldn't have to move more than that.
    if (raising)
    {
        if (currentStation.equals(servingArea) && elevCounter >= 14 / ELEV_PULSE_DISTANCE)
        {
            stopPlatform();
            elevStopped = true;
        }
        else if (elevCounter >= 57.5 / ELEV_PULSE_DISTANCE) // VALUE NOT FINALIZED
        {
            stopPlatform();
            elevStopped = true;
        }
    }
}