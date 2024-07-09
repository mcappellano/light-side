#include "main.h"
#include "drive.h"
#include "reflectance.h"
#include "elevator.h"
#include "sweeper.h"

int currentNode = START_POSITION;
int nextNode = PLATES;       // For top bot code, replace this with BUNS
int nextNextNode = EXCHANGE; // Same for top bot
int tapeCounter = 0;

void setup()
{
    Serial.begin(115200);

    pinMode(REFLEC1, INPUT);
    pinMode(REFLEC2, INPUT);

    pinMode(ELEV_SWITCH, INPUT_PULLUP); // LOW means switch is pressed. Connect C to the proper ESP pin, NO to ESP GND, and NC to nothing.
    pinMode(SWEEP_SWITCH, INPUT_PULLUP);

    pinMode(motor1F, OUTPUT);
    pinMode(motor1B, OUTPUT);
    pinMode(motor2F, OUTPUT);
    pinMode(motor2B, OUTPUT);
    pinMode(motor3F, OUTPUT);
    pinMode(motor3B, OUTPUT);
    pinMode(motor4F, OUTPUT);
    pinMode(motor4B, OUTPUT);

    analogWriteFrequency(freqHz);

    attachInterrupt(digitalPinToInterrupt(REFLEC1), tapeInterrupt1, RISING); // might have to be FALLING
    attachInterrupt(digitalPinToInterrupt(REFLEC2), tapeInterrupt2, RISING); // might have to be FALLING

    /*
    For testing:
    currentNode = 3;
    nextNode = 2;
    traverseForward();
    */
}

void loop()
{
}

/*
Unneeded OLED code:
Before setup:
Adafruit_SSD1306 display_handler(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

At end of setup:
display_handler.begin(SSD1306_SWITCHCAPVCC, 0x3C);
display_handler.clearDisplay();
display_handler.setTextSize(1);
display_handler.setTextColor(SSD1306_WHITE);

In loop:
display_handler.display();
*/
