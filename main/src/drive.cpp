/* Please check the driveREADME to see the detailed explanation of this code!
Open it on the GitHub website to see the diagrams.
Note that this code is specific to the bottom robot. */

#include "drive.h"
#include "reflectance.h"
#include "main.h"

int tapeToSee = 0;
uint32_t freqHz = 50;
uint8_t resolution = 8;
uint8_t dcQuarter = 63;
uint8_t dcHalf = 127;
uint8_t dcMax = 255;

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

    // TO DO: wait until the robot has finished crossing

    // Accounts for the "forward" direction changing when we spin 180 degrees
    if ((nextNode > currentNode && currentNode < 10) || (nextNode < currentNode && nextNode >= 10))
        traverseForward();
    else if ((nextNode < currentNode && currentNode < 10) || (nextNode > currentNode && currentNode >= 10))
        traverseBackward();
}

void traverseForward()
{
    // TO DO: start spinning the motors in one direction

    // while (tapeCounter < tapeToSee)
    //     delay(1);

    // TO DO: spin motors in opposite direction at slower speed

    // centreOnTape();

    // currentNode = nextNode;
    // nextNode = nextNextNode;
}

void traverseBackward()
{
    // TO DO: start spinning the motors in the other direction

    // while (tapeCounter < tapeToSee)
    //     delay(1);

    // TO DO: spin motors in opposite direction at slower speed

    // centreOnTape();

    // currentNode = nextNode;
    // nextNode = nextNextNode;
}

void crossUp()
{
    // TO DO: drive up halfway

    // spinAround();

    // TO DO: drive up the rest of the way

    // currentNode -= 10;
}

void crossDown()
{
    // TO DO: drive down halfway

    // spinAround();

    // TO DO: drive down the rest of the way

    // currentNode += 10;
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

void motorsForward(uint8_t dutyCycle)
{
    analogWrite(motor1F, dutyCycle);
    analogWrite(motor1B, 0);

    analogWrite(motor2F, dutyCycle);
    analogWrite(motor2B, 0);

    analogWrite(motor3F, dutyCycle);
    analogWrite(motor3B, 0);

    analogWrite(motor4F, dutyCycle);
    analogWrite(motor4B, 0);
}

void motorsBackward(uint8_t dutyCycle)
{
    analogWrite(motor1F, 0);
    analogWrite(motor1B, dutyCycle);

    analogWrite(motor2F, 0);
    analogWrite(motor2B, dutyCycle);

    analogWrite(motor3F, 0);
    analogWrite(motor3B, dutyCycle);

    analogWrite(motor4F, 0);
    analogWrite(motor4B, dutyCycle);
}

void motorsUpwards(uint8_t dutyCycle)
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

void motorsDownwards(uint8_t dutyCycle)
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

void stopMotors()
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
