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
    retractSweeper(dcQuarter, true);

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
    retractSweeper(dcQuarter, true);

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

    delay(2000);

    traverseCounter(true, dcThreeQs, dcEighth);

    retractSweeper(dcQuarter, true);
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
    raisePlatform(dcQuarter);
    delay(3000);

    previousFoodHeight = 3; // TO GET PERFECTLY THE HEIGHT OF COUNTER
    lowerPlatform(dcQuarter);
    delay(2000);

    extendSweeper(dcThreeQs);
    delay(4000);
    currentStation = plates;
    previousFoodHeight = plates.height;
    retractSweeper(dcQuarter, true);
    delay(3000);

    extendSweeper(dcThreeQs);
    delay(2000);
    lowerPlatform(dcQuarter);
    delay(2000);
    currentStation = exchange;
    previousFoodHeight = exchange.height;
    retractSweeper(dcQuarter, true);
    delay(3000);

    extendSweeper(dcThreeQs);
    delay(2000);
    // lowerPlatform(dcQuarter); Don't uncomment
    // delay(2000); Don't uncomment
    currentStation = cooktop;
    previousFoodHeight = cooktop.height;
    retractSweeper(dcQuarter, true);
    delay(3000);

    extendSweeper(dcThreeQs);
    delay(2000);
    lowerPlatform(dcQuarter);
    delay(2000);
    currentStation = cheese;
    previousFoodHeight = cheese.height;
    retractSweeper(dcQuarter, true);
    delay(3000);

    extendSweeper(dcThreeQs);
    delay(2000);
    lowerPlatform(dcQuarter);
    delay(2000);
    currentStation = tomatoes;
    previousFoodHeight = tomatoes.height;
    retractSweeper(dcQuarter, true);
    delay(3000);

    extendSweeper(dcThreeQs);
    delay(2000);
    lowerPlatform(dcQuarter);
    delay(2000);
    currentStation = lettuce;
    previousFoodHeight = lettuce.height;
    retractSweeper(dcQuarter, true);
    delay(3000);

    extendSweeper(dcThreeQs);
    delay(2000);
    lowerPlatform(dcQuarter);
    delay(2000);
    currentStation = exchange;
    previousFoodHeight = 60; // VALUE NOT FINALIZED
    retractSweeper(dcQuarter, true);
    delay(3000);

    // lowerPlatform(dcQuarter);
    // delay(3000);
    // currentStation = servingArea;
    // retractSweeper(dcQuarter, false);
    // delay(3000);
    // raisePlatform(dcQuarter);
    // delay(3000);
    // extendSweeper(dcQuarter);
}

void cheesePlate()
{
    currentStation = start;
    driveUpward(dcEighth);
    delay(2000);
    nextStation = plates;
    goNextStation();
    retractSweeper(dcQuarter, true);

    nextStation = cheese;
    goNextStation();
    retractSweeper(dcQuarter, true);
    driveBackward(dcThreeQs);
    delay(1900);

    lowerPlatform(dcQuarter);
    delay(3000);
    currentStation = servingArea;
    retractSweeper(dcQuarter, false);
    delay(3000);
    raisePlatform(dcQuarter);
    delay(3000);
    extendSweeper(dcQuarter);
}