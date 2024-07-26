/* Please check the navREADME to see the detailed explanation of this code!
Open it on the GitHub website to see the diagrams.
Note that this code is specific to the bottom robot. */

#include "nav.h"
#include "main.h"
#include "drive.h"
#include "reflectance.h"
#include "sweeper.h"
#include "elevator.h"

volatile int tapeCounter = 0;
volatile bool arrived = false;
int tapeToSee = 0;
bool adjusted = false;
bool forward2 = true;
bool crossed = false;

hw_timer_t *slowDownTimer = NULL;

void goNextStation()
{
    crossed = false;
    previousFoodHeight = currentStation.height;
    node = currentStation.num;

    // Cross counter if necessary
    if ((node >= 10 && nextStation.num < 10) || (node < 10 && nextStation.num >= 10))
    {
        crossCounters();
        crossed = true;
    }

    // Get ready to cross the correct number of tape pieces
    arrived = false;
    adjusted = false;
    tapeCounter = 0;
    tapeToSee = abs(nextStation.num - node) / 2 + 1;

    // Accounts for the "forward" direction changing when we spin 180 degrees
    if ((nextStation.num > node && node < 10) || (nextStation.num < node && nextStation.num >= 10))
        traverseCounter(false, dcThreeQs, dcEighth); // Modify driving speeds here
    else if ((nextStation.num < node && node < 10) || (nextStation.num > node && node >= 10))
        traverseCounter(true, dcThreeQs, dcEighth); // Modify driving speeds here
}

/*
IMPORTANT:
While traversing forward/backward, we will also be extending the arm out, and lowering the platform.
At some point we must consider the edge cases of there being only a counter crossing movement and no traversing,
and that of moving to and from the serving area!
*/
void traverseCounter(bool forward, uint8_t driveSpeed, uint8_t reverseSpeed)
{
    forward2 = forward;

    // Start driving along the counter
    if (forward == true)
        driveBackward(driveSpeed);
    else
        driveForward(driveSpeed);

    // Move sweeper and platform to ready positions
    if (currentStation.equals(potatoes) || currentStation.equals(patties) || currentStation.equals(buns))
        lowerPlatform(dcQuarter, true);

    // Allow tape to be counted starting a short duration after leaving the current piece of tape
    // IF WE END UP NEEDING THIS, IT MUST BE ADJUSTED. It messes up when we start close to a piece of tape
    if (!crossed)
    {
        alreadySeen = true;
        timerWrite(tapeTimer, 0);
        timerAlarmEnable(tapeTimer);
        tapeCounter = 0;
    }

    /* We must now wait until we have arrived at the food station.
    The motion of the sweeper must happen faster than the time it takes to traverse from two adjacent food stations.
    The platform and sweeper are stopped at the right height/length by the encoder interrupts. */
    while (!arrived)
    {
        // if (!adjusted)
        //     handleEdgeCases();
    }

    // In case they don't finish before making it to the food station
    stopSweeper();
    stopPlatform();

    // Don't allow any more tape pieces to be unintentionally counted until we are ready again
    alreadySeen = true;

    // Now back up until centred exactly on the tape
    if (forward == true)
        driveForward(reverseSpeed);
    else
        driveBackward(reverseSpeed);

    delay(100);

    while (digitalRead(REFLEC1) == LOW && digitalRead(REFLEC2) == LOW) // PREVIOUSLY || (not &&)
        delay(1);

    stopDriving();

    // Update relevant variables
    currentStation = nextStation;
    node = currentStation.num;
}

// Handle edge cases that require slowing down before arriving at the tape - DIFFERS BETWEEN THE TWO BOTS
void handleEdgeCases()
{
    int next = nextStation.num;
    if ((next == 0 || next == 3) && tapeCounter == tapeToSee - 1)
    {
        if (forward2 == true)
            driveBackward(dcEighth);
        else
            driveForward(dcEighth);

        adjusted = true;
    }
    if (next == 10)
    {
        if (currentStation.equals(tomatoes))
            driveForward(dcEighth);
        else
        {
            if (node == 11)
                timerAlarmWrite(slowDownTimer, 500 * 1000, false);
            else if (node == 12)
                timerAlarmWrite(slowDownTimer, 2000 * 1000, false);
            else if (node == 13)
                timerAlarmWrite(slowDownTimer, 2500 * 1000, false);

            timerWrite(slowDownTimer, 0);
            timerAlarmEnable(slowDownTimer);
        }
        adjusted = true;
    }
    if (next == 13)
    {
        if (currentStation.equals(plates) || node == 12)
            driveBackward(dcEighth);
        else
        {
            // if (node == 12)
            //     timerAlarmWrite(slowDownTimer, 300 * 1000, false);
            if (node == 11)
                timerAlarmWrite(slowDownTimer, 2000 * 1000, false);
            else if (node == 10)
                timerAlarmWrite(slowDownTimer, 2500 * 1000, false);

            timerWrite(slowDownTimer, 0);
            timerAlarmEnable(slowDownTimer);
        }
        adjusted = true;
    }
}

void exchangeItem()
{
    raisePlatform(dcQuarter, true);
}

void IRAM_ATTR slowDownTimerInterrupt()
{
    if (forward2 == true)
        driveBackward(dcEighth);
    else
        driveForward(dcEighth);

    timerAlarmDisable(slowDownTimer);
}
