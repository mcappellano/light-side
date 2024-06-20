#include "main.h"
#include "sensors.h"

void checkSwitch()
{
    if (digitalRead(SWITCH))
        display_handler.println("SWITCH PRESSED");
    else
        display_handler.println("SWITCH NOT PRESSED");

    display_handler.setCursor(0, 20);
    display_handler.print("Button presses: ");
    display_handler.print(counter);
}

void getReflectance()
{
    if (digitalRead(DIGITAL_REFLECTANCE))
        display_handler.println("BLACK TAPE");
    else
        display_handler.println("WHITE FLOOR");
}

void handle_interrupt()
{
    counter++;
}