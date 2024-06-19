#include "main.h"

void setup()
{
    pinMode(SWITCH, INPUT_PULLUP);
    pinMode(DIGITAL_REFLECTANCE, INPUT);
    pinMode(ANALOG_REFLECTANCE, INPUT);
    pinMode(REFLEC_POT, INPUT);

    attachInterrupt(digitalPinToInterrupt(SWITCH), handle_interrupt, RISING);

    display_handler.begin(SSD1306_SWITCHCAPVCC, 0x3C);
    // display_handler.display();
    // delay(2000);
    display_handler.clearDisplay();
    display_handler.setTextSize(1);
    display_handler.setTextColor(SSD1306_WHITE);
}

void loop()
{
    display_handler.clearDisplay();
    display_handler.setCursor(0, 0);

    // Replace this function call with whichever one you like
    checkReflectanceDigital();

    display_handler.display();
};

/*
The following functions perform various simple tasks,
and set up the OLED with text to be displayed.
*/
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

void checkReflectanceDigital()
{
    if (digitalRead(DIGITAL_REFLECTANCE))
        display_handler.println("BLACK TAPE");
    else
        display_handler.println("WHITE FLOOR");

    int pot_val = analogRead(REFLEC_POT);
    display_handler.println("");
    display_handler.print("Pot value: ");
    display_handler.print(pot_val);
}

void checkReflectanceAnalog()
{
    int reflectance = analogRead(ANALOG_REFLECTANCE);

    if (reflectance > reflectanceThreshold)
        display_handler.println("BLACK");
    else
        display_handler.println("WHITE");
}

void checkHomemadeReflectance()
{
    int reflectance = analogRead(HOMEMADE_REFLECTANCE);

    display_handler.println(reflectance);
}

void handle_interrupt()
{
    counter++;
}
