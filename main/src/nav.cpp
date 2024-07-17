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

void goNextStation()
{
    previousFoodHeight = currentStation.height;

    // Cross to the other counter if necessary
    if (nextStation.num >= currentStation.num + 10 || nextStation.num <= currentStation.num - 10)
        crossCounters();

    // Get ready to cross the correct number of tape pieces
    arrived = false;
    tapeCounter = 0;
    if (node <= 10)
        tapeToSee = abs(nextStation.num - node);
    else
        tapeToSee = 1;

    // TO DO: wait until the crossing counter timer interrupt changes a variable that signals we made it to the other side

    // Accounts for the "forward" direction changing when we spin 180 degrees
    if ((nextStation.num > currentStation.num && currentStation.num < 10) || (nextStation.num < currentStation.num && nextStation.num >= 10))
        traverseCounter(false, dcQuarter, dcEighth); // DOUBLE CHECK DIRECTIONS
    else if ((nextStation.num < currentStation.num && currentStation.num < 10) || (nextStation.num > currentStation.num && currentStation.num >= 10))
        traverseCounter(true, dcQuarter, dcEighth);
}

/*
IMPORTANT:
While traversing forward/backward, we will also be extending the arm out, and lowering the platform.
At some point we must consider the edge cases of there being only a counter crossing movement and no traversing,
and that of moving to and from the serving area!
*/
void traverseCounter(bool forward, uint8_t driveSpeed, uint8_t reverseSpeed)
{
    arrived = false; // for testing
    tapeCounter = 0; // for testing
    tapeToSee = 2;   // for testing

    // Start driving along the counter
    if (forward == true)
        driveBackward(driveSpeed);
    else
        driveForward(driveSpeed);

    // Move sweeper and platform to ready positions
    extendSweeper(dcEighth); // Modify sweeper speed here
    lowerPlatform(dcEighth); // Modify platform speed here

    // Allow tape to be counted starting a short duration after leaving the current piece of tape
    alreadySeen = true;
    timerWrite(tapeTimer, 0);
    timerAlarmEnable(tapeTimer);
    tapeCounter = 0;

    /* We must now wait until we have arrived at the food station.
    The motion of the sweeper must happen faster than the time it takes to traverse from two adjacent food stations.
    The platform and sweeper are stopped at the right height/length by the encoder interrupts. */
    while (!arrived)
    {
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

    while (digitalRead(REFLEC1) == LOW || digitalRead(REFLEC2) == LOW)
        delay(1);

    stopDriving();

    // Update relevant variables
    currentStation = nextStation;
    node = currentStation.num;
    nextStation = nextNextStation;
}

void goServe()
{
}