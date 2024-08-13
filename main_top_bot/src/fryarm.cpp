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
    analogWrite(FRY_ALIGNER, 162);
}

void raiseAligner()
{
    analogWrite(FRY_ALIGNER, 60);
}

void lowerCutter()
{
    analogWrite(FRY_CUTTER, 250);
}

void raiseCutter()
{
    analogWrite(FRY_CUTTER, 150);
}
