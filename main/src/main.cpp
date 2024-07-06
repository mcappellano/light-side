#include "main.h"
#include "drive.h"
#include "reflectance.h"

int currentNode = START_POSITION;
int nextNode = PLATES;       // For top bot code, replace this with BUNS
int nextNextNode = EXCHANGE; // Same for top bot
int tapeCounter = 0;

// OLED display
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET -1    // This display does not have a reset pin accessible

Adafruit_SSD1306 display_handler(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void setup()
{
    pinMode(REFLEC1, INPUT);
    pinMode(REFLEC2, INPUT);

    //pinMode(21, OUTPUT);
    //pinMode(8, OUTPUT);

    pinMode(motor1F, OUTPUT);
    pinMode(motor1B, OUTPUT);
    pinMode(motor2F, OUTPUT);
    pinMode(motor2B, OUTPUT);
    pinMode(motor3F, OUTPUT);
    pinMode(motor3B, OUTPUT);
    pinMode(motor4F, OUTPUT);
    pinMode(motor4B, OUTPUT);

    analogWriteResolution(resolution);
    analogWriteFrequency(freqHz);

    // ledcAttach(motor1F, freqHz, resolution);
    // ledcAttach(motor1B, freqHz, resolution);
    // ledcAttach(motor2F, freqHz, resolution);
    // ledcAttach(motor2B, freqHz, resolution);
    // ledcAttach(motor3F, freqHz, resolution);
    // ledcAttach(motor3B, freqHz, resolution);
    // ledcAttach(motor4F, freqHz, resolution);
    // ledcAttach(motor4B, freqHz, resolution);
    
    // ledcSetup(motor1F, freqHz, resolution);
    // ledcSetup(motor1B, freqHz, resolution);
    // ledcSetup(motor2F, freqHz, resolution);
    // ledcSetup(motor2B, freqHz, resolution);
    // ledcSetup(motor3F, freqHz, resolution);
    // ledcSetup(motor3B, freqHz, resolution);
    // ledcSetup(motor4F, freqHz, resolution);
    // ledcSetup(motor4B, freqHz, resolution);

    attachInterrupt(digitalPinToInterrupt(REFLEC1), tapeInterrupt1, RISING); // might have to be FALLING
    attachInterrupt(digitalPinToInterrupt(REFLEC2), tapeInterrupt2, RISING); // might have to be FALLING

    display_handler.begin(SSD1306_SWITCHCAPVCC, 0x3C);
    display_handler.clearDisplay();
    display_handler.setTextSize(1);
    display_handler.setTextColor(SSD1306_WHITE);
}

void loop()
{
    // analogWrite(21, 127);
    // analogWrite(8, 127);
    // digitalWrite(21, HIGH);
    // digitalWrite(21, HIGH);
    // display_handler.clearDisplay();
    // display_handler.setCursor(0, 0);

    motorsForward(dcHalf);

    // display_handler.display();
}
