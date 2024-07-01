/* Please check the driveREADME to see the detailed explanation of this code!
Open it on the GitHub website to see the diagrams.
Note that this code is specific to the bottom robot. */

#include "drive.h"
#include "main.h"

void goNextNode()
{
    if (nextNode >= currentNode + 10)
        crossDown();
    else if (nextNode <= currentNode - 10)
        crossUp();

    if (nextNode > currentNode)
        driveForward();
    else if (nextNode < currentNode)
        driveBackward();
}

void driveForward()
{
}

void driveBackward()
{
}

void crossUp()
{
}

void crossDown()
{
}

void spinAround()
{
}

void centreOnTape()
{
}
