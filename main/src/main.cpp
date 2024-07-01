#include "main.h"
#include "sensors.h"
#include "drive.h"
#include "reflectance.h"

int currentNode = START_POSITION;
int nextNode = PLATES; // For top bot code, replace this with BUNS
int tapeCounter = 0;

void setup()
{
    pinMode(REFLECTANCE, INPUT);

    attachInterrupt(digitalPinToInterrupt(REFLECTANCE), tapeInterrupt, RISING);

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
    getReflectance();

    display_handler.display();
};
