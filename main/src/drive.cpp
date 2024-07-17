/* Please check the driveREADME to see the detailed explanation of this code!
Open it on the GitHub website to see the diagrams.
Note that this code is specific to the bottom robot. */

#include "drive.h"
#include "reflectance.h"
#include "main.h"
#include "sweeper.h"
#include "elevator.h"

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
    extendSweeper(dcEighth);  // Modify sweeper speed here
    lowerPlatform(dcQuarter); // Modify platform speed here

    // Allow tape to be counted starting a short duration after leaving the current piece of tape
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
    BETTER OPTION (see below):
    Find the perfect combination of motors speeds
    so that the robot moves counter to counter
    while simulatenously rotating 180 degrees
    (consult image I found online and add the vectors to the two motions we want)
    */

    // IMPORTANT NOTE: double duty cycle is not necessarily double RPM
    changingPWMs(dcQuarter, dcEighth, false, dcQuarter, dcQuarter, true, 0, dcEighth, true, 0, 0, true, 10);
    changingPWMs(dcEighth, 0, false, dcQuarter, dcQuarter, true, dcEighth, dcQuarter, true, 0, 0, true, 10);
    changingPWMs(0, 0, false, dcQuarter, dcEighth, true, dcQuarter, dcQuarter, true, 0, dcEighth, false, 10);
    changingPWMs(0, 0, false, dcEighth, 0, true, dcQuarter, dcQuarter, true, dcEighth, dcQuarter, false, 10);

    if (currentStation.num >= 10)
        node -= 10;
    else
        node += 10;
}

void changingPWMs(uint8_t motor1Start, uint8_t motor1End, bool forw1, uint8_t motor2Start, uint8_t motor2End, bool forw2, uint8_t motor3Start, uint8_t motor3End, bool forw3, uint8_t motor4Start, uint8_t motor4End, bool forw4, int loops)
{
    uint8_t motor1Speed = motor1Start;
    uint8_t motor2Speed = motor2Start;
    uint8_t motor3Speed = motor3Start;
    uint8_t motor4Speed = motor4Start;

    stopDriving();

    for (int i = 0; i < loops; i++)
    {
        if (forw1)
            analogWrite(motor1F, motor1Speed);
        else
            analogWrite(motor1B, motor1Speed);

        if (forw2)
            analogWrite(motor2F, motor2Speed);
        else
            analogWrite(motor2B, motor2Speed);

        if (forw3)
            analogWrite(motor3F, motor3Speed);
        else
            analogWrite(motor3B, motor3Speed);

        if (forw4)
            analogWrite(motor4F, motor4Speed);
        else
            analogWrite(motor4B, motor4Speed);

        motor1Speed -= (motor1Start - motor1End) / loops; // This will cause slight rounding errors. Hopefully not enough to notice.
        motor2Speed -= (motor2Start - motor2End) / loops;
        motor3Speed -= (motor3Start - motor3End) / loops;
        motor4Speed -= (motor4Start - motor4End) / loops;
        delay(75); // change the number of loops and the delay to get the timing right
    }
}

uint8_t *calibrateDutyCycle(uint8_t dutyCycle)
{
    uint8_t motor1;
    uint8_t motor2;
    uint8_t motor3;
    uint8_t motor4;

    if (abs(dutyCycle - dcQuarter) < 15)
    {
        motor1 = dutyCycle * 0.7; // These constants are what we have to determine while calibrating
        motor2 = dutyCycle * 0.7;
        motor3 = dutyCycle * 0.7;
        motor4 = dutyCycle * 0.7;
    }
    else if (abs(dutyCycle - dcEighth) < 5)
    {
        motor1 = dutyCycle * 0.7; // These constants are what we have to determine while calibrating
        motor2 = dutyCycle * 0.7;
        motor3 = dutyCycle * 0.7;
        motor4 = dutyCycle * 0.7;
    }

    uint8_t calibrated[4] = {motor1, motor2, motor3, motor4};
    return calibrated;
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
    analogWrite(motor2B, dutyCycle + 15);

    analogWrite(motor3F, 0);
    analogWrite(motor3B, dutyCycle + 15);

    analogWrite(motor4F, 0);
    analogWrite(motor4B, dutyCycle);
}

void driveDiagonal(uint8_t dutyCycle)
{
    analogWrite(motor1F, 0);
    analogWrite(motor1B, dutyCycle * 0.8);

    analogWrite(motor2F, 0);
    analogWrite(motor2B, dutyCycle);

    analogWrite(motor3F, 0);
    analogWrite(motor3B, dutyCycle * 0.8);

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

    analogWrite(motor4F, dutyCycle);
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
