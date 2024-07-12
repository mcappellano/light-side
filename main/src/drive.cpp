/* Please check the driveREADME to see the detailed explanation of this code!
Open it on the GitHub website to see the diagrams.
Note that this code is specific to the bottom robot. */

#include "drive.h"
#include "reflectance.h"
#include "main.h"
#include "sweeper.h"
#include "elevator.h"

// hw_timer_t *arrivalCheckTimer = NULL;
volatile int tapeCounter = 0;
volatile bool arrived = false;
int tapeToSee = 0;

uint32_t freqHz = 50;
uint8_t dcThirtieth = 7;
uint8_t dcSixteenth = 15;
uint8_t dcEighth = 31;
uint8_t dcQuarter = 63;
uint8_t dcHalf = 127;
uint8_t dcMax = 245; // FOR SOME REASON, the motor buzzes when ran at 100% at doesn't spin. But 245 works (96%).

uint8_t dcIncreasePercentage = 0.2; // This is the percentage of the chosen duty cycle by which we would like to increase/decrease select motors

void goNextNode()
{
    // Cross to the other counter if necessary
    if (nextNode >= currentNode + 10 || nextNode <= currentNode - 10)
        crossCounters();

    // Get ready to cross the correct number of tape pieces
    arrived = false;
    tapeCounter = 0;
    if (currentNode <= 10)
        tapeToSee = abs(nextNode - currentNode);
    else
        tapeToSee = 1;

    // TO DO: wait until the crossing counter timer interrupt changes a variable that signals we made it to the other side

    // Accounts for the "forward" direction changing when we spin 180 degrees
    if ((nextNode > currentNode && currentNode < 10) || (nextNode < currentNode && nextNode >= 10))
        traverseCounter(false);
    else if ((nextNode < currentNode && currentNode < 10) || (nextNode > currentNode && currentNode >= 10))
        traverseCounter(true);
}

/*
IMPORTANT:
While traversing forward/backward, we will also be extending the arm out, and lowering the platform.
At some point we must consider the edge cases of there being only a counter crossing movement and no traversing,
and that of moving to and from the serving area!
*/
void traverseCounter(bool forward)
{
    arrived = false; // for testing
    tapeCounter = 0; // for testing
    tapeToSee = 2;   // for testing

    if (forward == true)
        driveForward(dcEighth);
    else
        driveBackward(dcEighth);

    extendSweeper(dcEighth);
    lowerPlatform(dcEighth);

    delay(20); // Might need these two lines of code to prevent counting the tape we are driving off of from counting
    tapeCounter = 0;

    while (!arrived) // The motions of the sweeper and elevator must happen faster than the time it takes to traverse from two adjacent food stations
    {
        Serial.println(tapeCounter);
        if (sweeperPosition >= EXTEND_POS)
            stopSweeper();
        if (platformHeight <= previousHeight - previousFoodHeight)
        {
            stopPlatform();
            previousHeight = platformHeight;
        }
    }

    stopSweeper();                   // In case it doesn't finish before making it to the tape
    stopPlatform();                  // Also in case
    previousHeight = platformHeight; // Also

    // Now back up until we centre exactly on the tape
    if (forward == true)
        driveBackward(dcEighth);
    else
        driveForward(dcEighth);

    while (digitalRead(REFLEC1) == LOW || digitalRead(REFLEC2) == LOW)
        delay(1);

    stopDriving();

    // currentNode = nextNode;
    // nextNode = nextNextNode;
}

void crossCounters()
{
    /*
    FIRST OPTION:
    motorsUpward(dcHalf); // might have to switch this to be downward, and below to be upward (careful on first test)
    delay until we have left wall sufficiently
    spinAround();
    delay
    motorsDownward(dcHalf);
    */

    /*
    BETTER OPTION:
    Find the perfect combination of motors speeds
    so that the robot moves counter to counter
    while simulatenously rotating 180 degrees
    (consult image I found online and add the vectors to the two motions we want)
    */

    if (currentNode >= 10)
        currentNode -= 10;
    else
        currentNode += 10;
}

void spinAround(uint8_t dutyCycle)
{
    analogWrite(motor1F, dutyCycle);
    analogWrite(motor1B, 0);

    analogWrite(motor2F, 0);
    analogWrite(motor2B, dutyCycle);

    analogWrite(motor3F, 0);
    analogWrite(motor3B, dutyCycle);

    analogWrite(motor4F, dutyCycle);
    analogWrite(motor4B, 0);
}

void driveForward(uint8_t dutyCycle)
{
    // uint8_t dutyCycleIncreased = dutyCycle + dutyCycle * dcIncreasePercentage; // Using these allows the duty cycle to change and not mess up the calibrations
    // uint8_t dutyCycleDecreased = dutyCycle - dutyCycle * dcIncreasePercentage; // We would have to add these two lines of code to all the driving functions

    analogWrite(motor1F, dutyCycle);
    analogWrite(motor1B, 0);

    analogWrite(motor2F, dutyCycle);
    analogWrite(motor2B, 0);

    analogWrite(motor3F, dutyCycle);
    analogWrite(motor3B, 0);

    analogWrite(motor4F, dutyCycle);
    analogWrite(motor4B, 0);
}

void driveBackward(uint8_t dutyCycle)
{
    analogWrite(motor1F, 0);
    analogWrite(motor1B, dutyCycle);

    analogWrite(motor2F, 0);
    analogWrite(motor2B, dutyCycle + 7);

    analogWrite(motor3F, 0);
    analogWrite(motor3B, dutyCycle + 7);

    analogWrite(motor4F, 0);
    analogWrite(motor4B, dutyCycle);
}

void driveUpward(uint8_t dutyCycle)
{
    analogWrite(motor1F, 0);
    analogWrite(motor1B, dutyCycle);

    analogWrite(motor2F, dutyCycle);
    analogWrite(motor2B, 0);

    analogWrite(motor3F, 0);
    analogWrite(motor3B, dutyCycle);

    analogWrite(motor4F, dutyCycle + 7);
    analogWrite(motor4B, 0);
}

void driveDownward(uint8_t dutyCycle)
{
    analogWrite(motor1F, dutyCycle);
    analogWrite(motor1B, 0);

    analogWrite(motor2F, 0);
    analogWrite(motor2B, dutyCycle);

    analogWrite(motor3F, dutyCycle);
    analogWrite(motor3B, 0);

    analogWrite(motor4F, 0);
    analogWrite(motor4B, dutyCycle);
}

void stopDriving()
{
    analogWrite(motor1F, 0);
    analogWrite(motor1B, 0);

    analogWrite(motor2F, 0);
    analogWrite(motor2B, 0);

    analogWrite(motor3F, 0);
    analogWrite(motor3B, 0);

    analogWrite(motor4F, 0);
    analogWrite(motor4B, 0);
}

/*
For the unused timer:
void IRAM_ATTR arrivalCheckInterrupt()
{
    if (tapeCounter >= tapeToSee)
        arrived = true;
}
*/
