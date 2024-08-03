#include "main.h"
#include "drive.h"
#include "reflectance.h"
#include "elevator.h"
#include "sweeper.h"
#include "tests.h"
#include "nav.h"

Station::Station(double num, double height, int sweepLength, int item) : num(num), height(height), sweepLength(sweepLength), item(item) {}

bool Station::equals(const Station &other) const
{
    return (this->num == other.num) &&
           (this->height == other.height) &&
           (this->sweepLength == other.sweepLength);
}

Station start(0, 0, 100, NA);       // Same sweep distance as plates and same number as tomato (for technicalities of the start sequence)
Station tomatoes(0, 3, 205, NA);    // 215 - PREVIOUSLY 4.4
Station exchange(1, 8, 216, EMPTY); // PREVIOUSLY 0 - Only buns are being exchanged here. Top bun height doesn't matter
Station cooktop(2, 9, 193, EMPTY);  // PREVIOUSLY 10 - 203 ... Only height of patty matters; fries are not being stacked
Station plates(3, 13.5, 201, NA);   // 193
Station cheese(10, 4.3, 206, NA);   // 206
Station lettuce(13, 3, 207, NA);    // PREVIOUSLY 4.3  -  210
Station servingArea(11.5, 1, 359, NA);
Station burgerBack(1, 15, 25, NA); // This is a "fake" station that is only used to know the distance needed to sweep the burger to the back of the plate

Station currentStation = start;
Station nextStation = plates;

Station stationOrder[3] = {plates, exchange, servingArea}; // tomatoes, cheese, lettuce, cooktop, exchange,
int delayOrder[3] = {0, 0, 0};
int orderNum = 0;

double node = -1;

bool higherCounter = true; // CHANGE THIS VARIABLE BASED ON WHICH COUNTER WE ARE ON

void setup()
{
    Serial.begin(115200);

    // Pin setups
    pinMode(REFLEC1, INPUT);
    pinMode(REFLEC2, INPUT);

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

    slowDownTimer = timerBegin(2, 80, true);
    timerAttachInterrupt(slowDownTimer, &slowDownTimerInterrupt, true);
    timerAlarmWrite(slowDownTimer, 500 * 1000, false);

    crossTimer = timerBegin(3, 80, true);
    timerAttachInterrupt(crossTimer, &crossTimerInterrupt, true);

    // accelTimer = timerBegin(1, 80, true);
    // timerAttachInterrupt(accelTimer, &accelTimerInterrupt, true);
    // timerAlarmWrite(accelTimer, 500 * 1000, true);

    // Adjustments if on higher counter
    // if (higherCounter)
    // {
    //     plates.height = 12;
    // }

    Serial.println("");
    Serial.println("Setup");

    // ACTUAL CODE ----------------------------------------------------
    delay(1000); // Take this out eventually
    driveUpward(dcQuarter);
    delay(900);
    stopDriving();

    // TESTING CODE ---------------------------------------------------
    // raisePlatform(dcQuarter);
    // extendSweeper(dcQuarter);
    // delay(3000);
    // previousFoodHeight = 40;
    // lowerPlatform(dcQuarter, false);
    // retractSweeper()
    // delay(3000);
    // previousFoodHeight = 5;
    // lowerPlatform(dcQuarter, true);
    // while (true)
    // {
    // }
}

/* The loop decides where the robot will go next, and calls goNextStation().
Once arrived, it retracts the sweeper to collect the item. Finally, it loops back and repeats.
If the next station is the serving area, it serves the meal. */
void loop()
{
    nextStation = stationOrder[orderNum];
    delay(delayOrder[orderNum++]);
    if (nextStation.equals(exchange) && orderNum < 5)
    {
        exchange.sweepLength = 216;
        exchange.item = BOTTOM_BUN;
    }
    else if (nextStation.equals(exchange) && orderNum > 5)
    {
        exchange.sweepLength = 193;
        exchange.item = TOP_BUN;
    }

    goNextStation();
    if (currentStation.equals(servingArea))
        serveMeal();
    else
    {
        retractSweeper(dcQuarter, true, false); // Maybe make it dcThreeQs
        // if (currentStation.equals(plates))      // Depending on the height of the counter, we may or may not need this part
        // {
        //     previousFoodHeight = 0;
        //     lowerPlatform(dcQuarter, false);
        // }
        driveUpward(dcQuarter);
        setCrossTimer(150);
        stopDriving();
        while (!swept)
        {
        }
    }

    if (nextStation.equals(servingArea))
        orderNum = 0;
}

// Old code
/*
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

extendSweeper(dcQuarter);
delay(3000);
currentStation = cheese;
retractSweeper(dcQuarter, true);
delay(2000);
currentStation = servingArea;
retractSweeper(dcQuarter, false);
delay(2000);
raisePlatform(dcQuarter);
delay(2000);
extendSweeper(dcQuarter);
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