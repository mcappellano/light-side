#include "reflectance.h"
#include "main.h"
#include "drive.h"

hw_timer_t *tapeTimer = NULL;
volatile bool alreadySeen = false;

void IRAM_ATTR tapeInterrupt()
{
    if (!alreadySeen)
    {
        tapeCounter++;
        alreadySeen = true;
        if (tapeCounter >= tapeToSee)
        {
            arrived = true;
        }
        timerWrite(tapeTimer, 0);
        timerAlarmEnable(tapeTimer);
    }
}

void IRAM_ATTR tapeTimerInterrupt()
{
    alreadySeen = false;
    timerAlarmDisable(tapeTimer);
}

// void tapeInterrupt2()
// {
//     if ((nextNode > currentNode && currentNode < 10) || (nextNode < currentNode && nextNode >= 10))
//         tapeCounter++;
// }

// For the first one: if ((nextNode < currentNode && currentNode < 10) || (nextNode > currentNode && currentNode >= 10))