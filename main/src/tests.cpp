#include "tests.h"
#include "main.h"
#include "drive.h"
#include "sweeper.h"
#include "elevator.h"
#include "reflectance.h"

void testElevator()
{
    // GO TO THE TOP UNTIL SWITCH IS PRESSED
    platformHeight = 0;
    raisePlatform(100);

    while (true)
    {
        if (platformHeight == MAX_HEIGHT)
        {
            stopPlatform();
            break;
        }
    }

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
    lowerPlatform(70);
    while (true)
    {
        if (abs(elevCounter) >= BOTTOM_BUN_HEIGHT / ELEV_PULSE_DISTANCE)
        {
            stopPlatform();
            break;
        }
    }

    delay(2000);

    // print encoder info
    float distanceMoved = elevCounter * ELEV_PULSE_DISTANCE;
    Serial.println("");
    Serial.println("DOWN BY BOTTOM BUN HEIGHT");
    Serial.println("Number of ticks:");
    Serial.println(elevCounter);
    Serial.println("Distance moved (mm):");
    Serial.println(distanceMoved);

    // MOVE DOWN BY THE HEIGHT OF A TOMATO
    elevCounter = 0;
    lowerPlatform(70);
    while (true)
    {
        if (abs(elevCounter) >= TOMATO_HEIGHT / ELEV_PULSE_DISTANCE)
        {
            stopPlatform();
            break;
        }
    }

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

void testSweeper()
{
    extendSweeper(45);

    while (true)
    {
        if (sweeperPosition == EXTEND_POS)
        {
            stopSweeper();
            break;
        }
    }

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