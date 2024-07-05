#include "reflectance.h"
#include "main.h"

void tapeInterrupt1()
{
    // enclose increment in an if statement for when the first TCRT is the first to cross the tape
    tapeCounter++;
}

void tapeInterrupt2()
{
    // enclose increment in an if statement for when the second TCRT is the first to cross the tape
    tapeCounter++;
}

void centreOnTape()
{
    // TO DO: turn off motors when both reflectance sensors see black tape
}

void getReflectance()
{
    if (digitalRead(REFLEC1))
        display_handler.println("BLACK TAPE");
    else
        display_handler.println("WHITE FLOOR");
}