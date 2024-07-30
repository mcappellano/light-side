#include "tests.h"
#include "main.h"
#include "drive.h"
#include "sweeper.h"
#include "elevator.h"
#include "reflectance.h"
#include "nav.h"

void testElevator()
{
    raisePlatform(dcQuarter, false);

    delay(2500);

    float distanceMoved = elevCounter * ELEV_PULSE_DIST;

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
    lowerPlatform(dcEighth, false);

    delay(2000);

    // print encoder info
    distanceMoved = elevCounter * ELEV_PULSE_DIST;
    Serial.println("");
    Serial.println("DOWN BY BOTTOM BUN HEIGHT");
    Serial.println("Number of ticks:");
    Serial.println(elevCounter);
    Serial.println("Distance moved (mm):");
    Serial.println(distanceMoved);

    delay(1000);

    // MOVE DOWN BY THE HEIGHT OF A TOMATO
    previousFoodHeight = tomatoes.height;
    lowerPlatform(dcEighth, false);

    delay(2000);

    // print encoder info
    distanceMoved = elevCounter * ELEV_PULSE_DIST;
    Serial.println("");
    Serial.println("DOWN BY TOMATO HEIGHT");
    Serial.println("Number of ticks:");
    Serial.println(elevCounter);
    Serial.println("Distance moved (mm):");
    Serial.println(distanceMoved);
}

// DETERMINE SWEEP_PULSE_DIST BEFORE RUNNING THIS TEST
void testSweeper()
{
    extendSweeper(dcQuarter);

    delay(3000);

    float distanceMoved = sweepCounter * SWEEP_PULSE_DIST;
    Serial.println("");
    Serial.println("EXTENDING");
    Serial.println("Number of ticks:");
    Serial.println(sweepCounter);
    Serial.println("Distance moved (mm):");
    Serial.println(distanceMoved);

    currentStation = plates;
    retractSweeper(dcQuarter, true);

    delay(3000);

    distanceMoved = sweepCounter * SWEEP_PULSE_DIST;
    Serial.println("");
    Serial.println("PLATE SWEEP");
    Serial.println("Number of ticks:");
    Serial.println(sweepCounter);
    Serial.println("Distance moved (mm):");
    Serial.println(distanceMoved);

    extendSweeper(dcQuarter);

    delay(3000);

    currentStation = exchange;
    retractSweeper(dcQuarter, true);

    delay(3000);

    distanceMoved = sweepCounter * SWEEP_PULSE_DIST;
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
    raisePlatform(dcQuarter, false);

    delay(2000);

    traverseCounter(true, dcThreeQs, dcEighth);

    retractSweeper(dcQuarter, true);
}

// Purely driving; traverse between the nodes in the expected order required to make a burger.
// During the delays is when we would be sweeping in the food item or pushing it onto the counter.
void majorTest2()
{
    // Start at bun station, go to exchange
    currentStation = buns;
    nextStation = exchange;
    goNextStation();
    delay(2000);

    // Get patty
    // currentStation = exchange; // added as a test can be removed after
    nextStation = patties;
    goNextStation();
    delay(2000);

    // // Cook patty
    nextStation = cooktop;
    goNextStation();
    delay(2000);

    // // Get fry
    nextStation = potatoes;
    goNextStation();
    delay(2000);

    // // Cook fry
    nextStation = cooktop;
    goNextStation();
    delay(2000);

    // // Get top bun
    nextStation = buns;
    goNextStation();
    delay(2000);

    // // Exchange top bun
    nextStation = exchange;
    goNextStation();
    delay(2000);
}

void timeTrials()
{
    raisePlatform(dcQuarter, false);
    delay(2000);
    // previousFoodHeight = 5; // VALUE NOT FINALIZED - the height in mm that the platform must lower from the fully raised position to having the platform at counter height
    // lowerPlatform(dcQuarter, false);
    // delay(500);
    previousFoodHeight = 35;

    currentStation = start;
    driveUpward(dcEighth);
    delay(1500); // VALUE NOT FINALIZED - the time required driving slowly towards the counter from the start position
    nextStation = buns;
    goNextStation();
    retractSweeper(dcQuarter, true);
    while (!swept)
    {
    }

    nextStation = exchange;
    goNextStation();
    exchangeItem();
    while (extending)
    {
    }

    nextStation = patties;
    goNextStation();
    retractSweeper(dcQuarter, true);
    while (!swept)
    {
    }

    nextStation = cooktop;
    goNextStation();
    exchangeItem();
    while (extending)
    {
    }

    nextStation = buns;
    goNextStation();
    retractSweeper(dcQuarter, true);
    while (!swept)
    {
    }

    nextStation = exchange;
    goNextStation();
    exchangeItem();
}