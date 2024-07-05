#include "main.h"
#include "drive.h"
#include "reflectance.h"

int currentNode = START_POSITION;
int nextNode = PLATES;       // For top bot code, replace this with BUNS
int nextNextNode = EXCHANGE; // Same for top bot
int tapeCounter = 0;

Adafruit_SSD1306 display_handler(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void setup()
{
    pinMode(REFLEC1, INPUT);
    pinMode(REFLEC2, INPUT);

    ledcSetup(motor1F, freqHz, resolution);
    ledcSetup(motor1B, freqHz, resolution);
    ledcSetup(motor2F, freqHz, resolution);
    ledcSetup(motor2B, freqHz, resolution);
    ledcSetup(motor3F, freqHz, resolution);
    ledcSetup(motor3B, freqHz, resolution);
    ledcSetup(motor4F, freqHz, resolution);
    ledcSetup(motor4B, freqHz, resolution);

    attachInterrupt(digitalPinToInterrupt(REFLEC1), tapeInterrupt1, RISING); // might have to be FALLING
    attachInterrupt(digitalPinToInterrupt(REFLEC2), tapeInterrupt2, RISING); // might have to be FALLING

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

    motorsForward(dcHalf);

    display_handler.display();
};
