#include "sweeper.h"
#include "main.h"

int sweeperPosition = FULLY_RETRACT_POS; // Assuming we have to start fully retracted

void extendSweeper(uint8_t dutyCycle)
{
}

void retractSweeper(uint8_t dutyCycle)
{
}

void stopSweeper()
{
}

void sweepSwitchInterrupt()
{
    sweeperPosition = EXTEND_POS;
}