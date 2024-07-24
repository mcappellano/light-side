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
    if (abs(nextStation.num - currentStation.num) >= 7)
    {
        // Make adjustment before crossing counter if necessary
        if (currentStation.equals(cheese) || currentStation.equals(plates))
        {
            driveBackward(dcQuarter);
            delay(250);
        }
        else if (currentStation.equals(tomatoes) || currentStation.equals(lettuce))
        {
            driveForward(dcQuarter);
            delay(150);
        }
        crossCounters();
        crossed = true;
    }

    // Get ready to cross the correct number of tape pieces
    arrived = false;
    adjusted = false;
    tapeCounter = 0;
    if (node <= 10)
        tapeToSee = abs(nextStation.num - node);
    else
        tapeToSee = 1;

    // TO DO: wait until the crossing counter timer interrupt changes a variable that signals we made it to the other side

    // Accounts for the "forward" direction changing when we spin 180 degrees
    if ((nextStation.num > node && node < 10) || (nextStation.num < node && nextStation.num >= 10))
        traverseCounter(false, dcThreeQs, dcEighth);
    else if ((nextStation.num < node && node < 10) || (nextStation.num > node && node >= 10))
        traverseCounter(true, dcThreeQs, dcEighth);
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
    extendSweeper(dcQuarter); // Modify sweeper speed here

    previousFoodHeight = plates.height; // REMOVE THIS LINE!!!
    if (!currentStation.equals(start))
       lowerPlatform(dcEighth); // Modify platform speed here

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
        if (!adjusted)
            handleEdgeCases();
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
        if (currentStation.equals(plates))
            driveBackward(dcEighth);
        else
        {
            if (node == 12)
                timerAlarmWrite(slowDownTimer, 500 * 1000, false);
            else if (node == 11)
                timerAlarmWrite(slowDownTimer, 2000 * 1000, false);
            else if (node == 10)
                timerAlarmWrite(slowDownTimer, 2500 * 1000, false);

            timerWrite(slowDownTimer, 0);
            timerAlarmEnable(slowDownTimer);
        }
        adjusted = true;
    }
}

// At this point we have already dropped the burger onto the plate, and then collected salad and fries.
// We are now going from either lettuce, tomato, or cooktop to the serving area.
// WAIT ---------------- IT WOULD BE SMART TO GET SALAD LETTUCE ALWAYS LAST - SAVES TIME and means we don't have to make two separate cases for this part of the code
void goServe()
{
    if (node < 10)
        crossCounters();

    if (node <= 11)
        driveBackward2(dcThreeQs);
    else if (node >= 12)
        driveForward2(dcThreeQs);

    // DO THIS: If we make the crossCounters timing use a timer instead of delays, we will be able to lower and retract while crossing counters
    previousFoodHeight = 60; // VALUE NOT FINALIZED - THE HEIGHT IN MM TO LOWER TO GET THE TOP OF THE BURGER OUT OF THE WAY FOR THE SWEEPER (it's the distance from the rim of the plate to the top of the burger)
    lowerPlatform(dcQuarter);

    currentStation = servingArea;
    retractSweeper(dcEighth, true); // VALUE NOT FINALIZED - In main.cpp, change the third entry of the servingArea station object to the distance in mm that the sweeper has to retract from the position after sweeping a fry or salad item to fully retracted.

    // Enter cross correlation code, call stopDriving() once we detect enough of a signal

    raisePlatform(dcQuarter);
    delay(2000);
    extendSweeper(dcQuarter);
    delay(2000);
    swingIn();

    // Ready to go to nextStation, which should be set to the plates station
}

void IRAM_ATTR slowDownTimerInterrupt()
{
    if (forward2 == true)
        driveBackward(dcEighth);
    else
        driveForward(dcEighth);

    timerAlarmDisable(slowDownTimer);
}