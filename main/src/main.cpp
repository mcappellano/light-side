#include "main.h"
#include "drive.h"
#include "reflectance.h"

int currentNode = START_POSITION;
int nextNode = PLATES;       // For top bot code, replace this with BUNS
int nextNextNode = EXCHANGE; // Same for top bot
int tapeCounter = 0;

// OLED display - CURRENTLY NOT BEING USED
// #define SCREEN_WIDTH 128 // OLED display width, in pixels
// #define SCREEN_HEIGHT 64 // OLED display height, in pixels
// #define OLED_RESET -1    // This display does not have a reset pin accessible
// Adafruit_SSD1306 display_handler(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void setup()
{
    Serial.begin(115200);

    pinMode(REFLEC1, INPUT);
    pinMode(REFLEC2, INPUT);

    pinMode(motor1F, OUTPUT);
    pinMode(motor1B, OUTPUT);
    pinMode(motor2F, OUTPUT);
    pinMode(motor2B, OUTPUT);
    pinMode(motor3F, OUTPUT);
    pinMode(motor3B, OUTPUT);
    pinMode(motor4F, OUTPUT);
    pinMode(motor4B, OUTPUT);

    // analogWriteResolution(resolution);
    analogWriteFrequency(freqHz);

    attachInterrupt(digitalPinToInterrupt(REFLEC1), tapeInterrupt1, RISING); // might have to be FALLING
    attachInterrupt(digitalPinToInterrupt(REFLEC2), tapeInterrupt2, RISING); // might have to be FALLING

    // display_handler.begin(SSD1306_SWITCHCAPVCC, 0x3C);
    // display_handler.clearDisplay();
    // display_handler.setTextSize(1);
    // display_handler.setTextColor(SSD1306_WHITE);

    //motorsForward(dcMax);
    motorsUpwards(dcQuarter);
}

void loop()
{
    // display_handler.display();
}
