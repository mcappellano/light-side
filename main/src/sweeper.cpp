#include "sweeper.h"
#include "main.h"

int sweeperPosition = FULLY_RETRACT_POS; // Assuming we have to start fully retracted

void motorsExtend()
{
}

void motorsRetract()
{
}

void stopSweeper()
{
}

void sweepSwitchInterrupt()
{
    sweeperPosition = EXTEND_POS;
}