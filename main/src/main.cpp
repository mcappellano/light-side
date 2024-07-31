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

Station start(0, 0, 100, NA);               // Same sweep distance as plates and same number as tomato (for technicalities of the start sequence)
Station tomatoes(0, 3, 165 + 65, NA);       // PREVIOUSLY 4.4 - 176 + 65 ... All measurements in mm
Station exchange(1, 0, 165 + 65, EMPTY);    // PREVIOUSLY 15 // 169 + 65 ... Only buns are being exchanged here. Top bun height doesn't matter
Station cooktop(2, 9, 165 + 65, EMPTY);     // PREVIOUSLY 10 - 173 + 65 ... Only height of patty matters; fries are not being stacked
Station plates(3, 13.5, 140 + 65, NA);      // 149 + 65, Previously 24.5, 23.175 - 149... VALUES NOT FINALIZED - replace 40 with the height between the two platforms, replace 150 with the distance the plate must be swept in
Station cheese(10, 4.3, 162 + 65, NA);      // PREVIOUSLY 4.3  -  176 + 65 ...
Station lettuce(13, 3, 165 + 65, NA);       // PREVIOUSLY 4.3  -  179 + 65 ...
Station servingArea(11.5, 1, 325 + 65, NA); // 340 is the total distance the sweeper must move back (from fully extended to fully retracted)
Station burgerBack(1, 15, 25, NA);          // This is a "fake" station that is only used to know the distance needed to sweep the burger to the back of the plate

Station currentStation = start;
Station nextStation = plates;

Station stationOrder[9] = {plates, exchange, cheese, tomatoes, lettuce, cooktop, exchange, cooktop, servingArea};
int delayOrder[9] = {0, 2000, 0, 0, 0, 0, 0, 0};
int orderNum = 0;

double node = -1;

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

    accelTimer = timerBegin(1, 80, true);
    timerAttachInterrupt(accelTimer, &accelTimerInterrupt, true);
    timerAlarmWrite(accelTimer, 500 * 1000, true);

    slowDownTimer = timerBegin(2, 80, true);
    timerAttachInterrupt(slowDownTimer, &slowDownTimerInterrupt, true);
    timerAlarmWrite(slowDownTimer, 500 * 1000, false);

    crossTimer = timerBegin(3, 80, true);
    timerAttachInterrupt(crossTimer, &crossTimerInterrupt, true);

    Serial.println("");
    Serial.println("Setup");

    delay(1000);

    burger();
}

void loop()
{
    /*
    This will contain logic that decides where the robot will go next. Once that is decided, all we need to do is call goNextStation().
    So this loop determines the next food station we must go to, assign that to nextStation, call goNextStation(),
    and loop back to the beginning.
    We must ensure that goNextStation() is not called again until we are ready to move to the next station. Do this by checking
    until the variable readyToLeave is true, while making sure to set it back to false afterwards. (Not sure yet if this is needed)
    */

    // nextStation = stationOrder[orderNum];
    // delay(delayOrder[orderNum++]);
    // if (orderNum >= 9)
    //     orderNum = 0;

    // goNextStation();
    // if (currentStation.equals(servingArea))
    //     serveMeal();
    // else
    //     retractSweeper(dcQuarter, true); // maybe make it dcThreeQs
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