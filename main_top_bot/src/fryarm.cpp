#include "fryarm.h"
#include "drive.h"

//
void cutFries()
{
    lowerAligner();
    driveBackward(dcEighth); // DIRECTIONS MAY OR MAY NOT BE CORRECT
    delay(250);
    stopDriving();
    lowerCutter();
    driveForward(dcEighth);
    delay(250);
    stopDriving();
    raiseCutter();
    raiseAligner();
}

void lowerAligner()
{
}

void raiseAligner()
{
}

void lowerCutter()
{
}

void raiseCutter()
{
}