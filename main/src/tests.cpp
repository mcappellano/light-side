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
    lowerPlatform(dcEighth, false);

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
    lowerPlatform(dcEighth, false);

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
    retractSweeper(dcQuarter, true, false, false);

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
    retractSweeper(dcQuarter, true, false, false);

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
    raisePlatform(dcQuarter, false);

    delay(2000);

    traverseCounter(true, dcThreeQs, dcEighth);

    retractSweeper(dcQuarter, true, false, false);
}

// Purely driving; traverse between the nodes in the expected order required to make a burger.
// During the delays is when we would be sweeping in the food item.
void majorTest2()
{
    /*
    // Get plate - not part of this test
    currentStation = start;
    nextStation = plates;
    goNextStation();
    delay(2000);
    */

    // Get bottom bun
    currentStation = plates;
    nextStation = exchange;
    goNextStation();
    delay(2000);

    // Get tomato
    nextStation = tomatoes;
    goNextStation();
    delay(2000);

    // Get cheese
    nextStation = cheese;
    goNextStation();
    delay(2000);

    // Get patty
    nextStation = cooktop;
    goNextStation();
    delay(2000);

    // Get lettuce
    nextStation = lettuce;
    goNextStation();
    delay(2000);

    // Get top bun
    nextStation = exchange;
    goNextStation();
    delay(2000);

    // Get fry
    nextStation = cooktop;
    goNextStation();
    delay(2000);

    /*
    // Serve - not part of this test
    nextStation = servingArea;
    goNextStation();
    */
}

void buildBurgerStationary()
{
    extendSweeper(dcQuarter);
    delay(3000);

    raisePlatform(dcQuarter, false);
    delay(3000);

    // previousFoodHeight = 1; // TO GET PERFECTLY THE HEIGHT OF COUNTER
    // lowerPlatform(dcQuarter, false);
    // delay(3000);

    currentStation = plates;
    previousFoodHeight = plates.height;
    retractSweeper(dcQuarter, true, false, false);
    while (!swept)
    {
    }
    extendSweeper(dcThreeQs);
    delay(2000);
    lowerPlatform(dcQuarter, false);
    delay(2000);

    currentStation = exchange;
    previousFoodHeight = exchange.height;
    retractSweeper(dcQuarter, true, false, false);
    while (!swept)
    {
    }
    extendSweeper(dcThreeQs);
    delay(2000);
    lowerPlatform(dcQuarter, false);
    delay(2000);

    currentStation = tomatoes;
    previousFoodHeight = tomatoes.height;
    retractSweeper(dcQuarter, true, false, false);
    while (!swept)
    {
    }
    extendSweeper(dcThreeQs);
    delay(2000);
    lowerPlatform(dcQuarter, false);
    delay(2000);

    currentStation = cheese;
    previousFoodHeight = cheese.height;
    retractSweeper(dcQuarter, true, false, false);
    while (!swept)
    {
    }
    extendSweeper(dcThreeQs);
    delay(2000);
    lowerPlatform(dcQuarter, false);
    delay(2000);

    currentStation = lettuce;
    previousFoodHeight = lettuce.height;
    retractSweeper(dcQuarter, true, false, false);
    while (!swept)
    {
    }
    extendSweeper(dcThreeQs);
    delay(2000);
    lowerPlatform(dcQuarter, false);
    delay(2000);

    currentStation = cooktop;
    previousFoodHeight = cooktop.height;
    retractSweeper(dcQuarter, true, false, false);
    while (!swept)
    {
    }
    extendSweeper(dcThreeQs);
    delay(2000);
    lowerPlatform(dcQuarter, false);
    delay(2000);

    exchange.sweepLength = 193;
    currentStation = exchange;
    previousFoodHeight = 35; // VALUE NOT FINALIZED
    retractSweeper(dcQuarter, true, false, false);
    while (!swept)
    {
    }
}

// Don't use. is broken. use cheesePlateOld below
void cheesePlate()
{
    raisePlatform(dcQuarter, false);
    delay(2000);
    previousFoodHeight = 1;
    lowerPlatform(dcQuarter, false);
    delay(500);

    currentStation = start;
    driveUpward(dcEighth);
    delay(2000);

    while (true)
    {
        nextStation = plates;
        goNextStation();
        retractSweeper(dcQuarter, true, false, false);
        while (!swept)
            delay(1);

        plates.sweepLength -= 65;

        nextStation = cheese;
        goNextStation();
        retractSweeper(dcQuarter, true, false, false);
        while (!swept)
            delay(1);

        nextStation = servingArea;
        goNextStation();
        serveMeal();
    }
}

void burger()
{
    currentStation = start;
    driveUpward(dcQuarter);
    delay(900);
    stopDriving();
    nextStation = plates;
    goNextStation();
    retractSweeper(dcQuarter, true, false, false);
    while (!swept)
    {
    }

    delay(4000); // DELAY
    nextStation = exchange;
    goNextStation();
    retractSweeper(dcQuarter, true, false, false);
    while (!swept)
    {
    }

    delay(12000); // DELAY
    nextStation = cooktop;
    goNextStation();
    retractSweeper(dcQuarter, true, false, false);
    while (!swept)
    {
    }

    nextStation = cheese;
    goNextStation();
    retractSweeper(dcQuarter, true, false, false);
    while (!swept)
    {
    }

    nextStation = tomatoes;
    goNextStation();
    retractSweeper(dcQuarter, true, false, false);
    while (!swept)
    {
    }

    nextStation = lettuce;
    goNextStation();
    retractSweeper(dcQuarter, true, false, false);
    while (!swept)
    {
    }

    nextStation = exchange;
    goNextStation();
    retractSweeper(dcQuarter, true, false, false);
    while (!swept)
    {
    }

    nextStation = servingArea;
    goNextStation();
    serveMeal();

    // previousFoodHeight = 3; // VALUE NOT FINALIZED - the height in mm that the platform must lower from the fully raised position to having the platform at counter height
    // lowerPlatform(dcQuarter, false);
    // delay(500);
}

void salad()
{
    raisePlatform(dcQuarter, false);
    delay(2000);
    previousFoodHeight = 3; // VALUE NOT FINALIZED - the height in mm that the platform must lower from the fully raised position to having the platform at counter height
    lowerPlatform(dcQuarter, false);
    delay(500);

    currentStation = start;
    driveUpward(dcEighth);
    delay(2000);

    while (true)
    {
        nextStation = plates;
        goNextStation();
        retractSweeper(dcQuarter, true, false, false);
        while (!swept)
            delay(1);

        nextStation = tomatoes;
        goNextStation();
        retractSweeper(dcQuarter, true, false, false);
        while (!swept)
            delay(1);

        nextStation = lettuce;
        goNextStation();
        retractSweeper(dcQuarter, true, false, false);
        while (!swept)
            delay(1);

        nextStation = servingArea;
        goNextStation();
        serveMeal();
    }
}

void timeTrials()
{
    raisePlatform(dcQuarter, false);
    delay(2000);
    previousFoodHeight = 1;
    lowerPlatform(dcQuarter, false);
    delay(500);

    currentStation = start;
    driveUpward(dcEighth);
    delay(2000);

    for (int i = 0; i < 4; i++)
    {
        nextStation = plates;
        goNextStation();
        retractSweeper(dcQuarter, true, false, false);
        delay(2000);

        nextStation = tomatoes;
        goNextStation();
        retractSweeper(dcQuarter, true, false, false);
        delay(2000);

        nextStation = lettuce;
        goNextStation();
        retractSweeper(dcQuarter, true, false, false);
        delay(2000);

        nextStation = servingArea;
        goNextStation();
        serveMeal();
    }

    for (int i = 0; i < 4; i++)
    {
        nextStation = plates;
        goNextStation();
        retractSweeper(dcQuarter, true, false, false);
        delay(2000);

        nextStation = cheese;
        goNextStation();
        retractSweeper(dcQuarter, true, false, false);
        delay(2000);

        nextStation = servingArea;
        goNextStation();
        serveMeal();
    }
}

void cheesePlateOld()
{
    raisePlatform(dcQuarter, false);
    delay(2000);
    // previousFoodHeight = 1;
    // lowerPlatform(dcQuarter, false);
    // delay(1000);

    currentStation = start;
    driveUpward(dcEighth);
    delay(2000);

    while (true)
    {
        nextStation = plates;
        goNextStation();
        retractSweeper(dcQuarter, true, false, false);
        delay(2000);

        nextStation = cheese;
        goNextStation();
        retractSweeper(dcQuarter, true, false, false);
        delay(2000);

        nextStation = servingArea;
        goNextStation();
        serveMeal();
    }
}