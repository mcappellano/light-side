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
bool lowerFurther = false;

hw_timer_t *slowDownTimer = NULL;

void goNextStation()
{
    crossed = false;
    previousFoodHeight = currentStation.height;
    node = currentStation.num;
    alreadySeen = true;

    // Cross counter if necessary
    if (abs(nextStation.num - currentStation.num) >= 7)
    {
        // Make adjustment before crossing counter if necessary
        if (currentStation.equals(cheese) || currentStation.equals(plates))
        {
            driveBackward(dcQuarter);
            delay(350);
            stopDriving();
            delay(250);
        }
        else if (currentStation.equals(tomatoes) || currentStation.equals(lettuce))
        {
            driveForward(dcQuarter);
            delay(200);
            stopDriving();
            delay(200);
        }

        crossCounters();
        while (!crossed)
        {
        }
    }

    // Get ready to cross the correct number of tape pieces
    arrived = false;
    adjusted = false;
    tapeCounter = 0;
    if (node < 10)
        tapeToSee = abs(nextStation.num - node);
    else if (node >= 10)
        tapeToSee = 1;

    if (currentStation.equals(servingArea))
        tapeToSee = 2;

    // Workaround fix for bug going from cheese to lettuce
    if (currentStation.equals(cheese) && nextStation.equals(lettuce))
    {
        driveBackward(dcThreeQs);
        delay(200);
    }

    // Accounts for the "forward" direction changing when we spin 180 degrees
    if ((nextStation.num > node && node < 10) || (nextStation.num < node && nextStation.num >= 10))
        traverseCounter(false, dcThreeQs, dcEighth);
    else if ((nextStation.num < node && node < 10) || (nextStation.num > node && node >= 10))
        traverseCounter(true, dcThreeQs, dcEighth);
}

// Returns true or false based on whether we are the current station and next station are directly across from each other
bool directlyAcross()
{
    if ((currentStation.equals(lettuce) && nextStation.equals(plates)) || (currentStation.equals(plates) && nextStation.equals(lettuce)))
        return true;
    if ((currentStation.equals(cheese) && nextStation.equals(tomatoes)) || (currentStation.equals(tomatoes) && nextStation.equals(cheese)))
        return true;
    if ((currentStation.equals(lettuce) && nextStation.equals(cooktop)) || (currentStation.equals(cheese) && nextStation.equals(exchange)))
        return true;

    return false;
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

    if (currentStation.equals(exchange) && nextStation.equals(tomatoes))
        extendSweeper(dcThreeQs);

    // Start driving along the counter
    if (forward == true)
        driveBackward(driveSpeed);
    else
        driveForward(driveSpeed);

    // Move sweeper and platform to ready positions
    if (!nextStation.equals(servingArea))
    {
        if (currentStation.equals(start))
        {
            extendSweeper(50);
            raisePlatform(dcQuarter, false);
        }
        else if (currentStation.equals(exchange) && exchange.item == BOTTOM_BUN)
        {
            lowerPlatform(dcQuarter, false);
            setCrossTimer(200);
            extendSweeper(dcQuarter);
        }
        else if (!currentStation.equals(exchange))
        {
            extendSweeper(dcQuarter); // Modify sweeper speed here
            if (!currentStation.equals(start) && !currentStation.equals(servingArea))
                lowerPlatform(dcQuarter, false); // Modify platform speed here
        }
    }
    else
    {
        alreadySeen = true;
        if (forward == true)
            driveBackward(dcThreeQs); // Change speed for approaching serving area here
        else
            driveForward(dcThreeQs);

        // currentStation = servingArea;
        // retractSweeper(dcThreeQs, false, true, false); // With the current code, lettuce/cheese cannot be the final items we get before serving
    }

    // Allow tape to be counted starting a short duration after leaving the current piece of tape
    if (!crossed)
    {
        alreadySeen = true;
        timerWrite(tapeTimer, 0);
        timerAlarmEnable(tapeTimer);
        tapeCounter = 0;
    }
    else
        alreadySeen = false;

    // Fix for bug introduced somewhere else... I think it is fixed so we can take this out
    if (node == 1.5)
        alreadySeen = false;

    /* We must now wait until we have arrived at the food station.
    The motion of the sweeper must happen faster than the time it takes to traverse from two adjacent food stations.
    The platform and sweeper are stopped at the right height/length by the encoder interrupts. */
    while (!arrived)
    {
        if (!adjusted)
            handleEdgeCases();
    }

    // In case they don't finish before making it to the food station
    // stopSweeper();
    // stopPlatform();

    if (!nextStation.equals(servingArea))
    {
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
    }
    else
    {
        while (!arrived)
        {
        }
    }

    // Update relevant variables
    currentStation = nextStation;
    node = currentStation.num;
    distanceToSweep = currentStation.sweepLength;

    // if (currentStation.equals(exchange) && exchange.item == TOP_BUN)
    //     distanceToSweep = 148;
}

// Handle edge cases that require slowing down before arriving at the tape
void handleEdgeCases()
{
    double next = nextStation.num;

    // Going to tomatoes or plates (which are at the very edge of the table)
    if (((next == 0 && !currentStation.equals(exchange)) || next == 3 || (next == 2 && currentStation.equals(lettuce))) && tapeCounter == tapeToSee - 1)
    {
        if (forward2 == true)
            driveBackward(dcEighth);
        else
            driveForward(dcEighth);

        adjusted = true;
        if (next == 3)
            lowerPlatform(dcQuarter, false);
    }

    // Going to tomatoes from exchange (this saves about a second)
    else if (next == 0 && currentStation.equals(exchange))
    {
        timerAlarmWrite(slowDownTimer, 400 * 1000, false);
        timerWrite(slowDownTimer, 0);
        timerAlarmEnable(slowDownTimer);
        adjusted = true;
    }

    // Going to cheese (which is at the edge of the table)
    else if (next == 10)
    {
        alreadySeen = false;
        if (currentStation.equals(tomatoes))
            driveForward(dcEighth);
        else
        {
            if (node == 11)
                timerAlarmWrite(slowDownTimer, 500 * 1000, false);
            else if (node == 12)
                timerAlarmWrite(slowDownTimer, 2200 * 1000, false);
            else if (node == 13)
                timerAlarmWrite(slowDownTimer, 2500 * 1000, false);

            timerWrite(slowDownTimer, 0);
            timerAlarmEnable(slowDownTimer);
        }
        adjusted = true;
    }

    // Going to lettuce (which is at the edge of the table)
    else if (next == 13)
    {
        alreadySeen = false;
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

    // Going to serving area (which doesn't have tape to signal stopping)
    else if (next == 11.5)
    {
        // Fix to wacky bug somewhere else...
        if (currentStation.num == 11.5)
            node = 11;
        if (node == 11)
            timerAlarmWrite(slowDownTimer, 300 * 1000, false);
        else if (node == 12)
            timerAlarmWrite(slowDownTimer, 700, false);
        else if (node == 10 || node == 13)
            timerAlarmWrite(slowDownTimer, 2500 * 1000, false);

        timerWrite(slowDownTimer, 0);
        timerAlarmEnable(slowDownTimer);
        adjusted = true;
    }

    // Special case for going from serving area to plates between meals
    else if (node == 1.5 && tapeCounter == tapeToSee - 1)
    {
        driveForward(dcEighth);
        previousFoodHeight = 0;
        lowerPlatform(dcQuarter, false);
        adjusted = true;
    }

    // else if (next == 1 && tapeCounter == tapeToSee - 1)
    // {
    //     extendSweeper(dcQuarter);
    //     adjusted = true;
    // }
}

// Directly after arriving at serving area
void serveMeal()
{
    Serial.println(serveReady);
    while (!serveReady)
    {
    }
    serveReady = false;
    extendSweeper(dcThreeQs);
    delay(800);
    stopSweeper();
}

// Directly after sweeping in top bun, if we are not getting a fry or salad
void moveBackIfServing()
{
    if (nextStation.equals(servingArea))
    {
        raisePartial = true;
        lowerFurther = true;
        raisePlatform(dcQuarter, true);
    }
}

void IRAM_ATTR slowDownTimerInterrupt()
{
    if (nextStation.equals(servingArea))
    {
        stopDriving();
        arrived = true;
    }
    else if (forward2 == true)
        driveBackward(dcEighth);
    else if (forward2 == false)
        driveForward(dcEighth);

    timerAlarmDisable(slowDownTimer);
}