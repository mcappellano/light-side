#include "reflectance.h"
#include "main.h"

hw_timer_t *tapeTimer = NULL;
volatile bool alreadySeen = false;

void tapeInterrupt()
{
    if (!alreadySeen)
    {
        tapeCounter++;
        alreadySeen = true;
        timerAlarmWrite(tapeTimer, tapedelay_ms * 1000, false);
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