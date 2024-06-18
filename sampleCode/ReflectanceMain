#include <Wire.h>
#include <Adafruit_SSD1306.h>

#define SWITCH PA4
#define DIGITAL_REFLECTANCE PB10
#define ANALOG_REFLECTANCE PA7
#define REFLECTANCE_THRESHOLD PA6
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET -1    // This display does not have a reset pin accessible
Adafruit_SSD1306 display_handler(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Function definitions
void handle_interrupt();
void checkSwitch();
void checkReflectance();
void checkReflectanceAnalog();

// Variables
volatile int counter = 0;
volatile int reflectanceThreshold = 1.5;

void setup()
{
  pinMode(SWITCH, INPUT_PULLUP);
  pinMode(DIGITAL_REFLECTANCE, INPUT);
  pinMode(ANALOG_REFLECTANCE, INPUT);
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
  checkReflectanceAnalog();

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
    display_handler.println("BLACK");
  else
    display_handler.println("WHITE");
}

void checkReflectanceAnalog()
{
  int reflectance = analogRead(ANALOG_REFLECTANCE);

  display_handler.println(reflectance);

  // if (reflectance > reflectanceThreshold)
  //  display_handler.println("BLACK");
  // else
  //  display_handler.println("WHITE");
}

void handle_interrupt()
{
  counter++;
}
