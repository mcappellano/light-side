/* Please check the driveREADME to see the detailed explanation of this code!
Open it on the GitHub website to see the diagrams.
Note that this code is specific to the bottom robot. */

#include "drive.h"
#include "reflectance.h"
#include "main.h"

void goNextNode()
{
    // Cross to the other counter if necessary
    if (nextNode >= currentNode + 10)
        crossDown();
    else if (nextNode <= currentNode - 10)
        crossUp();

    // Get ready to cross the correct number of tape pieces
    tapeCounter = 0;
    if (currentNode <= 10)
        tapeToSee = abs(nextNode - currentNode);
    else
        tapeToSee = 1;

    // Accounts for the "forward" direction changing when we spin 180 degrees
    if ((nextNode > currentNode && currentNode < 10) || nextNode < currentNode && nextNode >= 10)
        driveForward();
    else if ((nextNode < currentNode && currentNode < 10) || nextNode > currentNode && currentNode >= 10)
        driveBackward();
}

void driveForward()
{
    // TO DO: start spinning the motors in one direction

    while (tapeCounter < tapeToSee)
        delay(1);

    // TO DO: spin motors in opposite direction at slower speed

    centreOnTape();

    currentNode = nextNode;
    nextNode = nextNextNode;
}

void driveBackward()
{
    // TO DO: start spinning the motors in the other direction

    while (tapeCounter < tapeToSee)
        delay(1);

    // TO DO: spin motors in opposite direction at slower speed

    centreOnTape();

    currentNode = nextNode;
    nextNode = nextNextNode;
}

void crossUp()
{
    // TO DO: drive up halfway

    spinAround();

    // TO DO: drive up the rest of the way

    currentNode -= 10;
}

void crossDown()
{
    // TO DO: drive down halfway

    spinAround();

    // TO DO: drive down the rest of the way

    currentNode += 10;
}

void spinAround()
{
    // TO DO: rotate 180 degrees
}
