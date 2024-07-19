#include "reflectance.h"
#include "main.h"
#include "nav.h"

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