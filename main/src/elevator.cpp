#include "elevator.h"
#include "main.h"

double platformHeight = MAX_HEIGHT;
double previousHeight = MAX_HEIGHT;
double previousFoodHeight = 0;

void raisePlatform(uint8_t dutyCycle)
{
}

void lowerPlatform(uint8_t dutyCycle)
{
}

void stopPlatform()
{
}

void swingOut()
{
}

void swingIn()
{
}

void elevSwitchInterrupt()
{
    platformHeight = MAX_HEIGHT;
}