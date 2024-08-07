/* Check the navREADME to see the detailed explanation of this code!
Open it on the GitHub website to see the diagrams.
Note that this code is specific to the top robot. */

#include "nav.h"
#include "main.h"
#include "drive.h"
#include "reflectance.h"
#include "sweeper.h"
#include "elevator.h"

hw_timer_t *slowDownTimer = NULL;
volatile int tapeCounter = 0;
volatile bool arrived = false;
int tapeToSee = 0;
bool adjusted = false;
bool forward2 = true;
bool crossed = false;

void goNextStation()
{
    crossed = false;
    previousFoodHeight = currentStation.height;
    node = currentStation.num;

    // Cross counter if necessary
    if ((node >= 10 && nextStation.num < 10) || (node < 10 && nextStation.num >= 10))
    {
        if (nextStation.equals(exchange) || nextStation.equals(cooktop))
            lowerPlatform(dcQuarter, true);
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
        raisePlatform(dcQuarter, false);

    else if (currentStation.equals(start))
    {
        extendSweeper(dcQuarter);
        raisePlatform(dcQuarter, false);
    }
    else if (currentStation.equals(exchange) || currentStation.equals(cooktop))
    {
        previousFoodHeight = DROP_DIST;
        lowerPlatform(dcQuarter, false);
    }

    // Allow tape to be counted starting a short duration after leaving the current piece of tape
    if (!crossed && !currentStation.equals(start))
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
        if (!adjusted)
            handleEdgeCases();
    }

    // Don't allow any more tape pieces to be unintentionally counted until we are ready again
    alreadySeen = true;

    // Now back up until centred on the tape
    if (forward == true)
        driveForward(reverseSpeed);
    else
        driveBackward(reverseSpeed);

    delay(150);

    while (digitalRead(REFLEC1) == LOW && digitalRead(REFLEC2) == LOW) {} // PREVIOUSLY || (not &&)

    stopDriving();

    // Update relevant variables
    currentStation = nextStation;
    distanceToSweep = currentStation.sweepLength;
    node = currentStation.num;
}

void handleEdgeCases()
{
    if (nextStation.equals(cooktop) && currentStation.equals(patties))
    {
        timerAlarmWrite(slowDownTimer, 2000 * 1000, false);
        timerWrite(slowDownTimer, 0);
        timerAlarmEnable(slowDownTimer);
        adjusted = true;
    }
    else if (currentStation.equals(exchange) && nextStation.equals(patties))
    {
        setCrossTimer(500);
        driveBackward(dcQuarter);
        adjusted = true;
    }
}

void IRAM_ATTR slowDownTimerInterrupt()
{
    if ((forward2 && node >= 10) || (!forward2 && node < 10))
        driveBackward(dcEighth);
    else
        driveForward(dcEighth);

    timerAlarmDisable(slowDownTimer);
}