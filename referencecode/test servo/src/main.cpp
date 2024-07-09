/**
 * @brief Example generating an analog out signal and then reading it as an
 *    	analog in signal and displaying it on the OLED.
 * @pre Have an OLED display connected to the Blue Pill. Connect PA0 to PA1 on
 *      Blue Pill.
 */
#include <Wire.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET -1    // This display does not have a reset pin accessible
Adafruit_SSD1306 display_handler(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define AIN_PIN PA5        // analog read pin
#define AOUT_PIN_PWM1 PA_1 // analog out pin for PWM (same pin but different)
#define AOUT_PIN_PWM2 PA_2 // analog out pin for PWM (same pin but different)
#define AOUT_PIN_PWM3 PA_3
#define AOUT_PIN_PWM4 PA_6
#define BUTTON PB10

// data type - this is confusing!)

// Variable to store the value of the analog in pin.
volatile uint16_t value = 0; // ADC is 12bit

// Variable to show the program is running
uint32_t loop_counter = 0;

volatile int pinNum = 1;

// Function prototypes:
void runMotor(int pinNum, uint32_t frequency_Hz, uint32_t dutycycle);

/**
 * @brief Initialize the analog in and out pins. We will use the analog out
 *    	pin as PWM. Also initialize the OLED display.
 * @param none
 * @retval none
 */
void setup()
{
  // Initialize pin modes
  pinMode(AIN_PIN, INPUT);
  pinMode(BUTTON, INPUT_PULLUP);
  pinMode(AOUT_PIN_PWM1, OUTPUT);
  pinMode(AOUT_PIN_PWM2, OUTPUT);
  pinMode(AOUT_PIN_PWM3, OUTPUT);
  pinMode(AOUT_PIN_PWM4, OUTPUT);

  // Initialize OLED display
  display_handler.begin(SSD1306_SWITCHCAPVCC, 0x3C);

  // Displays Adafruit logo by default. call clearDisplay immediately if you
  // don't want this.
  display_handler.display();
  delay(2000);

  // Displays "Hello world!" on the screen
  display_handler.clearDisplay();
  display_handler.setTextSize(1);
  display_handler.setTextColor(SSD1306_WHITE);
  display_handler.setCursor(0, 0);
  display_handler.println("Begin test!");
  display_handler.display();

  delay(1000);
}

/**
 * @brief Read the value of the analog-in pin and set the timing for the LED
 *    	blink period depending on this value.
 * @param none
 * @retval none
 */
double potDutyCylcle()
{
  double dutyCycle;
  double analogValue = analogRead(AIN_PIN);

  dutyCycle = 100 * (analogValue / 1024);

  return dutyCycle;
}

void loop()
{
  // Determine the PWM signal
  //  uint32_t frequency_Hz = 50;
  //  uint32_t dutyCycle = 25;
  //  runMotor(pinNum, frequency_Hz, dutyCycle);

  // display_handler.clearDisplay();
  // display_handler.setCursor(0, 0);
  // display_handler.println("pinNum, dutyCycle");
  // display_handler.println(pinNum);
  // display_handler.println(dutyCycle);
  // display_handler.display();

  // delay(2000);

  // dutyCycle = 75;
  // runMotor(pinNum, frequency_Hz, dutyCycle);

  // display_handler.clearDisplay();
  // display_handler.setCursor(0, 0);
  // display_handler.println("pinNum, dutyCycle");
  // display_handler.println(pinNum);
  // display_handler.println(dutyCycle);
  // display_handler.display();

  // delay(2000);

  // dutyCycle = 50;
  // runMotor(pinNum, frequency_Hz, dutyCycle);

  display_handler.clearDisplay();
  display_handler.setCursor(0, 0);
  // display_handler.println("pinNum, dutyCycle");
  // display_handler.println(pinNum);
  // display_handler.println(dutyCycle);

  if (!digitalRead(BUTTON))
    display_handler.println("PRESSED");
  else
    display_handler.println("LOOKS NORMAL");

  display_handler.display();

  // delay(1000);

  // dutycycle = 6;

  // runMotor(pinNum, frequency_Hz, dutycycle);

  // delay(1000);

  // dutycycle = 7;

  // runMotor(pinNum, frequency_Hz, dutycycle);

  // delay(1000);

  // dutycycle = 9;

  // runMotor(pinNum, frequency_Hz, dutycycle);

  //delay(2000);

  // // if(loop_counter == 0){
  // //   runMotor(pinNum, frequency_Hz, dutycycle);
  // // }

  // //Read the PWM signal as analog in
  // value = analogRead(AIN_PIN);

  // if (digitalRead(BUTTON)) {
  //   if (pinNum == 1)
  //     pinNum = 2;
  //   else
  //     pinNum = 1;

  //   runMotor(pinNum, frequency_Hz, dutycycle);
  //   delay(50);
  // }
}

void runMotor(int pinNum, uint32_t frequency_Hz, uint32_t dutyCycle)
{

  if (pinNum == 1)
  {
    pwm_start(AOUT_PIN_PWM2, frequency_Hz, 0,
              TimerCompareFormat_t::PERCENT_COMPARE_FORMAT);
    pwm_start(AOUT_PIN_PWM1, frequency_Hz, dutyCycle,
              TimerCompareFormat_t::PERCENT_COMPARE_FORMAT);
    pwm_start(AOUT_PIN_PWM4, frequency_Hz, 0,
              TimerCompareFormat_t::PERCENT_COMPARE_FORMAT);
    pwm_start(AOUT_PIN_PWM3, frequency_Hz, dutyCycle,
              TimerCompareFormat_t::PERCENT_COMPARE_FORMAT);
  }

  else if (pinNum == 2)
  {
    pwm_start(AOUT_PIN_PWM1, frequency_Hz, 0,
              TimerCompareFormat_t::PERCENT_COMPARE_FORMAT);
    pwm_start(AOUT_PIN_PWM2, frequency_Hz, dutyCycle,
              TimerCompareFormat_t::PERCENT_COMPARE_FORMAT);
    pwm_start(AOUT_PIN_PWM3, frequency_Hz, 0,
              TimerCompareFormat_t::PERCENT_COMPARE_FORMAT);
    pwm_start(AOUT_PIN_PWM4, frequency_Hz, dutyCycle,
              TimerCompareFormat_t::PERCENT_COMPARE_FORMAT);
  }
}