#include "tests.h"
#include "main.h"
#include "drive.h"
#include "sweeper.h"
#include "elevator.h"
#include "reflectance.h"

void testElevator()
{
    elevCounter = 0;
    raisePlatform(dcQuarter);

    // while (true)
    // {
    //     if (platformHeight == MAX_HEIGHT)
    //     {
    //         stopPlatform();
    //         break;
    //     }
    // }

    // print encoder info
    float distanceMoved = elevCounter * ELEV_PULSE_DISTANCE;
    Serial.println("");
    Serial.println("TO THE TOP");
    Serial.println("Number of ticks:");
    Serial.println(elevCounter);
    Serial.println("Distance moved (mm):");
    Serial.println(distanceMoved);

    delay(2000);

    /*
    // MOVE DOWN BY THE HEIGHT OF A BOTTOM BUN
    elevCounter = 0;
    previousFoodHeight = exchange.height;
    lowerPlatform(dcQuarter);
    // while (true)
    // {
    //     if (abs(elevCounter) >= currentStation.height / ELEV_PULSE_DISTANCE)
    //     {
    //         stopPlatform();
    //         break;
    //     }
    // }


    // print encoder info
    float distanceMoved = elevCounter * ELEV_PULSE_DISTANCE;
    Serial.println("");
    Serial.println("DOWN BY BOTTOM BUN HEIGHT");
    Serial.println("Number of ticks:");
    Serial.println(elevCounter);
    Serial.println("Distance moved (mm):");
    Serial.println(distanceMoved);

    delay(2000);
    */

    /*
    // MOVE DOWN BY THE HEIGHT OF A TOMATO
    elevCounter = 0;
    previousFoodHeight = tomatoes.height;
    lowerPlatform(dcQuarter);

    // print encoder info
    distanceMoved = elevCounter * ELEV_PULSE_DISTANCE;
    Serial.println("");
    Serial.println("DOWN BY BOTTOM BUN HEIGHT");
    Serial.println("Number of ticks:");
    Serial.println(elevCounter);
    Serial.println("Distance moved (mm):");
    Serial.println(distanceMoved);
    */
}

// DETERMINE SWEEP_PULSE_DISTANCE BEFORE RUNNING THIS TEST
void testSweeper()
{
    extendSweeper(45); // Should stop when it hits the switch

    sweepCounter = 0;
    currentStation = cheese;
    retractSweeper(45);

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