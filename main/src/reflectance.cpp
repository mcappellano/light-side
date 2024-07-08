#include "reflectance.h"
#include "main.h"

void tapeInterrupt1()
{
    if ((nextNode < currentNode && currentNode < 10) || (nextNode > currentNode && currentNode >= 10))
        tapeCounter++;
}

void tapeInterrupt2()
{
    if ((nextNode > currentNode && currentNode < 10) || (nextNode < currentNode && nextNode >= 10))
        tapeCounter++;
}

// void centreOnTape()
// {
//     // TO DO: turn off motors when both reflectance sensors see black tape
// }