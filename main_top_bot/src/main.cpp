#include "main.h"
#include "drive.h"
#include "reflectance.h"
#include "elevator.h"
#include "sweeper.h"
#include "tests.h"
#include "nav.h"
#include "fryarm.h"

Station::Station(int num, double height, int sweepLength, int item) : num(num), height(height), sweepLength(sweepLength), item(item) {}

bool Station::equals(const Station &other) const
{
    return (this->num == other.num) &&
           (this->height == other.height) &&
           (this->sweepLength == other.sweepLength);
}

// All measurements in mm
Station start(0, 0, 150, NA);
Station patties(1, 35, 150, NA);
Station buns(3, 35, 150, NA);
Station potatoes(5, 35, 150, NA);
Station tomatoes(10, 0, 0, NA);
Station exchange(12, 35, 150, EMPTY);
Station cooktop(14, 35, 150, EMPTY);
Station plates(16, 0, 0, NA);

Station currentStation = start;
Station nextStation = buns;

Station stationOrder[8] = {buns, exchange, patties, cooktop, buns, exchange, potatoes, cooktop};
int orderNum = 0;

int node = -1;

void setup()
{
    Serial.begin(115200);

    // Pin setups
    pinMode(REFLEC1, INPUT);
    pinMode(REFLEC2, INPUT);
    pinMode(REFLEC3, INPUT);
    pinMode(REFLEC4, INPUT);

    pinMode(ELEV_SWITCH, INPUT); // LOW means switch is pressed. Connect C to the proper ESP pin, NO to ESP GND, and NC to nothing.
    pinMode(SWEEP_SWITCH, INPUT);

    pinMode(ELEV_ENCODER_1, INPUT);
    pinMode(ELEV_ENCODER_2, INPUT);
    pinMode(SWEEP_ENCODER_1, INPUT);
    pinMode(SWEEP_ENCODER_2, INPUT);

    pinMode(motor1F, OUTPUT);
    pinMode(motor1B, OUTPUT);
    pinMode(motor2F, OUTPUT);
    pinMode(motor2B, OUTPUT);
    pinMode(motor3F, OUTPUT);
    pinMode(motor3B, OUTPUT);
    pinMode(motor4F, OUTPUT);
    pinMode(motor4B, OUTPUT);

    pinMode(ELEV_MOTOR_UP, OUTPUT);
    pinMode(ELEV_MOTOR_DOWN, OUTPUT);
    pinMode(SWEEP_MOTOR_OUT, OUTPUT);
    pinMode(SWEEP_MOTOR_BACK, OUTPUT);

    // Set our PWM frequency (50 Hz)
    analogWriteFrequency(freqHz);

    // Interrupts
    attachInterrupt(digitalPinToInterrupt(REFLEC1), tapeInterrupt, RISING);
    attachInterrupt(digitalPinToInterrupt(REFLEC2), tapeInterrupt, RISING);

    attachInterrupt(digitalPinToInterrupt(ELEV_ENCODER_1), elevEncoderInterrupt, CHANGE);
    attachInterrupt(digitalPinToInterrupt(ELEV_ENCODER_2), elevEncoderInterrupt, CHANGE);
    attachInterrupt(digitalPinToInterrupt(SWEEP_ENCODER_1), sweepEncoderInterrupt, CHANGE);
    attachInterrupt(digitalPinToInterrupt(SWEEP_ENCODER_2), sweepEncoderInterrupt, CHANGE);

    // Switches
    attachInterrupt(digitalPinToInterrupt(ELEV_SWITCH), elevSwitchInterrupt, FALLING);
    attachInterrupt(digitalPinToInterrupt(SWEEP_SWITCH), sweepSwitchInterrupt, FALLING);

    // Timers
    tapeTimer = timerBegin(0, 80, true);
    timerAttachInterrupt(tapeTimer, &tapeTimerInterrupt, true);
    timerAlarmWrite(tapeTimer, tapedelay_ms * 1000, false);

    slowDownTimer = timerBegin(1, 80, true);
    timerAttachInterrupt(slowDownTimer, &slowDownTimerInterrupt, true);
    timerAlarmWrite(slowDownTimer, 500 * 1000, false);

    crossTimer = timerBegin(2, 80, true);
    timerAttachInterrupt(crossTimer, &crossTimerInterrupt, true);

    Serial.println("");
    Serial.println("Setup");

    delay(1000);

    crossCountersTape();
}

/* The loop determines the next station we have to go to, and sends the robot there.
Once arrived, we either sweep in the item or push it out onto the counter.
After waiting for this action to finish, we go back to the beginning of the loop. */
void loop()
{
    // nextStation = stationOrder[orderNum++];
    // if (orderNum >= 8)
    //     orderNum = 0;

    // goNextStation();
    // if (currentStation.equals(exchange) || currentStation.equals(cooktop))
    // {
    //     exchangeItem();
    //     while (extending)
    //     {
    //     }
    // }
    // else if (currentStation.equals(potatoes))
    // {
    //     cutFries();
    //     // Right here we will have to drive to make sure we are in the right place relative to the fries
    //     retractSweeper(dcQuarter, true); // Maybe make it dcThreeQs
    //     while (!swept)
    //     {
    //     }
    // }
    // else
    // {
    //     retractSweeper(dcQuarter, true); // Maybe make it dcThreeQs
    //     while (!swept)
    //     {
    //     }
    // }
}

/*
Testing code week of July 22nd:
raisePlatform(dcQuarter);
delay(2000);
previousFoodHeight = plates.height;
lowerPlatform(dcQuarter);
delay(2000);
previousFoodHeight = exchange.height;
lowerPlatform(dcQuarter);

extendSweeper(dcThreeQs);
delay(2000);
currentStation = exchange;
retractSweeper(dcQuarter, true);

raisePlatform(dcQuarter);
delay(2000);
currentStation = lettuce;
nextStation = plates;
goNextStation();
currentStation = exchange;
retractSweeper(dcQuarter, true);

driveBackward(dcThreeQs);
delay(1500);
driveBackward(dcEighth);
delay(1000);
stopDriving();
delay(1000);
driveForward(dcThreeQs);
delay(1500);
driveForward(dcEighth);
delay(1000);
stopDriving();
*/

/*
H-BRIDGE TESTING:
driveForward(dcQuarter);
delay(1000);
driveForward(dcHalf);
delay(1000);
driveForward(dcHalf * 1.5);
delay(1000);
driveForward(dcMax);
delay(1000);

stopDriving();
delay(1000);

driveBackward(dcMax);
delay(1000);
driveBackward(dcHalf * 1.5);
delay(1000);
driveBackward(dcHalf);
delay(1000);
driveBackward(dcQuarter);
delay(1000);

stopDriving();
*/

/*
UNUSED TIMER:
arrivalCheckTimer = timerBegin(0, 80, true);
timerAttachInterrupt(arrivalCheckTimer, &arrivalCheckInterrupt, true);
timerAlarmWrite(arrivalCheckTimer, 2000, true);
timerAlarmEnable(arrivalCheckTimer);
*/

/*
Old code for OLED:
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