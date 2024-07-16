#include "elevator.h"
#include "main.h"

const float ELEV_PULSE_DISTANCE = 1.63625; // millimitres
int elevCounter = 0;
volatile bool maxHeight = false;
volatile bool raising = false;
// double platformHeight = MAX_HEIGHT;
// double previousHeight = MAX_HEIGHT;
double previousFoodHeight = 23.175; // The distance between the two platforms

void raisePlatform(uint8_t dutyCycle)
{
    elevCounter = 0;
    raising = true;
    analogWrite(ELEV_MOTOR_UP, dutyCycle);
    analogWrite(ELEV_MOTOR_DOWN, 0);
}

void lowerPlatform(uint8_t dutyCycle)
{
    elevCounter = 0;
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
    // TO DO
}

void swingIn()
{
    // TO DO
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
    if (digitalRead(ELEV_ENCODER_2) != digitalRead(ELEV_ENCODER_1))
        elevCounter++;
    else
        elevCounter--;

    if (!raising && (elevCounter <= (-previousFoodHeight / ELEV_PULSE_DISTANCE) + 1)) // ADD 1 TO ACCOUNT FOR MOTOR STOPPING TIME (adding a positive makes a negative number "smaller") - MAY CHANGE WITH LOAD ADDED
        stopPlatform();
}