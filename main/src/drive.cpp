/* Please check the driveREADME to see the detailed explanation of this code!
Open it on the GitHub website to see the diagrams.
Note that this code is specific to the bottom robot. */

#include "drive.h"
#include "reflectance.h"
#include "main.h"

int tapeToSee = 0;
uint32_t freqHz = 50;
uint8_t dcEighth = 31;
uint8_t dcQuarter = 63;
uint8_t dcHalf = 127;
uint8_t dcMax = 255;

void goNextNode()
{
    // Cross to the other counter if necessary
    if (nextNode >= currentNode + 10 || nextNode <= currentNode - 10)
        crossCounters();

    // Get ready to cross the correct number of tape pieces
    tapeCounter = 0;
    if (currentNode <= 10)
        tapeToSee = abs(nextNode - currentNode);
    else
        tapeToSee = 1;

    // TO DO: wait until the robot has finished crossing

    // Accounts for the "forward" direction changing when we spin 180 degrees
    if ((nextNode > currentNode && currentNode < 10) || (nextNode < currentNode && nextNode >= 10))
        traverseBackward();
    else if ((nextNode < currentNode && currentNode < 10) || (nextNode > currentNode && currentNode >= 10))
        traverseForward();
}

void traverseForward()
{
    tapeCounter = 0; // for testing
    tapeToSee = 1;   // for testing

    motorsForward(dcMax);
    while (tapeCounter < tapeToSee)
        delay(1);

    motorsBackward(dcEighth);
    delay(10);
    while (digitalRead(REFLEC1) == LOW || digitalRead(REFLEC2) == LOW)
        delay(1);

    stopMotors();

    // currentNode = nextNode;
    // nextNode = nextNextNode;
}

void traverseBackward()
{
    tapeCounter = 0; // for testing
    tapeToSee = 1;   // for testing

    motorsBackward(dcMax);
    while (tapeCounter < tapeToSee)
        delay(1);

    motorsForward(dcEighth);
    delay(10);
    while (digitalRead(REFLEC1) == LOW || digitalRead(REFLEC2) == LOW)
        delay(1);

    stopMotors();

    // currentNode = nextNode;
    // nextNode = nextNextNode;
}

void crossCounters()
{
    motorsUpward(dcHalf);
    // delay until we have left wall sufficiently
    // spinAround();
    // delay
    motorsDownward(dcHalf);

    if (currentNode >= 10)
        currentNode -= 10;
    else
        currentNode += 10;
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

    analogWrite(motor2F, dutyCycle - 3); // minus 3 steers it slightly into the wall
    analogWrite(motor2B, 0);

    analogWrite(motor3F, dutyCycle - 3);
    analogWrite(motor3B, 0);

    analogWrite(motor4F, dutyCycle);
    analogWrite(motor4B, 0);
}

void motorsBackward(uint8_t dutyCycle)
{
    analogWrite(motor1F, 0);
    analogWrite(motor1B, dutyCycle - 3); // minus 3 steers it slightly into the wall

    analogWrite(motor2F, 0);
    analogWrite(motor2B, dutyCycle);

    analogWrite(motor3F, 0);
    analogWrite(motor3B, dutyCycle);

    analogWrite(motor4F, 0);
    analogWrite(motor4B, dutyCycle - 3);
}

void motorsUpward(uint8_t dutyCycle)
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

void motorsDownward(uint8_t dutyCycle)
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
