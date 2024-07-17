#include "tests.h"
#include "main.h"
#include "drive.h"
#include "sweeper.h"
#include "elevator.h"
#include "reflectance.h"
#include "nav.h"

void testElevator()
{
    raisePlatform(dcQuarter);

    delay(2500);

    float distanceMoved = elevCounter * ELEV_PULSE_DISTANCE;

    // print encoder info
    Serial.println("");
    Serial.println("TO THE TOP");
    Serial.println("Number of ticks:");
    Serial.println(elevCounter);
    Serial.println("Distance moved (mm):");
    Serial.println(distanceMoved);

    delay(1000);

    // MOVE DOWN BY THE HEIGHT OF A BOTTOM BUN
    previousFoodHeight = exchange.height;
    lowerPlatform(dcEighth);

    delay(2000);

    // print encoder info
    distanceMoved = elevCounter * ELEV_PULSE_DISTANCE;
    Serial.println("");
    Serial.println("DOWN BY BOTTOM BUN HEIGHT");
    Serial.println("Number of ticks:");
    Serial.println(elevCounter);
    Serial.println("Distance moved (mm):");
    Serial.println(distanceMoved);

    delay(1000);

    // MOVE DOWN BY THE HEIGHT OF A TOMATO
    previousFoodHeight = tomatoes.height;
    lowerPlatform(dcEighth);

    delay(2000);

    // print encoder info
    distanceMoved = elevCounter * ELEV_PULSE_DISTANCE;
    Serial.println("");
    Serial.println("DOWN BY TOMATO HEIGHT");
    Serial.println("Number of ticks:");
    Serial.println(elevCounter);
    Serial.println("Distance moved (mm):");
    Serial.println(distanceMoved);
}

// DETERMINE SWEEP_PULSE_DISTANCE BEFORE RUNNING THIS TEST
void testSweeper()
{
    extendSweeper(dcQuarter);

    delay(3000);

    float distanceMoved = sweepCounter * SWEEP_PULSE_DISTANCE;
    Serial.println("");
    Serial.println("EXTENDING");
    Serial.println("Number of ticks:");
    Serial.println(sweepCounter);
    Serial.println("Distance moved (mm):");
    Serial.println(distanceMoved);

    currentStation = plates;
    retractSweeper(dcQuarter);

    delay(3000);

    distanceMoved = sweepCounter * SWEEP_PULSE_DISTANCE;
    Serial.println("");
    Serial.println("PLATE SWEEP");
    Serial.println("Number of ticks:");
    Serial.println(sweepCounter);
    Serial.println("Distance moved (mm):");
    Serial.println(distanceMoved);

    extendSweeper(dcQuarter);

    delay(3000);

    currentStation = exchange;
    retractSweeper(dcQuarter);

    delay(3000);

    distanceMoved = sweepCounter * SWEEP_PULSE_DISTANCE;
    Serial.println("");
    Serial.println("BUN SWEEP");
    Serial.println("Number of ticks:");
    Serial.println(sweepCounter);
    Serial.println("Distance moved (mm):");
    Serial.println(distanceMoved);

    /*
    Task list for sweeper:
        - Find out how far the sweeper moves out for each tick (from Heejae)
            - Fully extended is 11mm
            - Burger buns sweep in moves to 202mm, 202-11 = 191mm <--
            - Patty is a further 4mm, 202+4-11 = 195mm <--
            - Both cheese and tomato are a further 3mm, 202+4+3-11 = 198mm <--
            - Lettuce is a further 3mm, 202+4+3+3-11 = 201mm <--
        - Measure the distance it needs to move from the reference position (fully extended) to each of the other 2 (or 3) positions
        - Calculate the number of ticks needed to get to each of those positions
        - Implement it in code
        - Test it
    */
}

// Drive to a tape line (on the same counter) while extending arm and lowering platform, then sweep in
void majorTest1()
{
    raisePlatform(dcQuarter);

    delay(3000);

    traverseCounter(true, dcQuarter, dcEighth);

    retractSweeper(dcEighth);
}