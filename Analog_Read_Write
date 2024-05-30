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
#define OLED_RESET 	-1 // This display does not have a reset pin accessible
Adafruit_SSD1306 display_handler(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define AIN_PIN PA0	// analog read pin
#define AOUT_PIN PA1   // analog out pin
#define AOUT_PIN_PWM PA_1 // analog out pin for PWM (same pin but different
                      	// data type - this is confusing!)

// Variable to store the value of the analog in pin.
volatile uint16_t value = 0; //ADC is 12bit

// Variable to show the program is running
uint32_t loop_counter = 0;

/**
 * @brief Initialize the analog in and out pins. We will use the analog out
 *    	pin as PWM. Also initialize the OLED display.
 * @param none
 * @retval none
 */
void setup()
{
  //Initialize analog in pin
  pinMode(AIN_PIN, INPUT);
  pinMode(AOUT_PIN, OUTPUT);

  //Initialize OLED display
  display_handler.begin(SSD1306_SWITCHCAPVCC, 0x3C);
 
  // Displays Adafruit logo by default. call clearDisplay immediately if you
  // don't want this.
  display_handler.display();
  delay(2000);

  // Displays "Hello world!" on the screen
  display_handler.clearDisplay();
  display_handler.setTextSize(1);
  display_handler.setTextColor(SSD1306_WHITE);
  display_handler.setCursor(0,0);
  display_handler.println("Hello world!");
  display_handler.display();
}

/**
 * @brief Read the value of the analog-in pin and set the timing for the LED
 *    	blink period depending on this value.
 * @param none
 * @retval none
 */
void loop()
{
  //Output a PWM signal
  uint32_t frequency_Hz = 100000;
  uint32_t dutycycle = 50;
  pwm_start(AOUT_PIN_PWM, frequency_Hz, dutycycle,
        	TimerCompareFormat_t::PERCENT_COMPARE_FORMAT);
  //Read the PWM signal as analog in
  value = analogRead(AIN_PIN);

  //Display the read signal's raw value (12 bit: 0 to 4096)
  display_handler.clearDisplay();
  display_handler.setCursor(0, 0);
  display_handler.println(loop_counter++);
  display_handler.print(value, DEC);
  display_handler.display();

  delay(500);
}
