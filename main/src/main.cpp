#include "main.h"
#include "drive.h"
#include "reflectance.h"
#include "elevator.h"
#include "sweeper.h"
#include "tests.h"
#include "nav.h"

Station::Station(int num, double height, int sweepLength, int item) : num(num), height(height), sweepLength(sweepLength), item(item) {}

bool Station::equals(const Station &other) const
{
    return (this->num == other.num) &&
           (this->height == other.height) &&
           (this->sweepLength == other.sweepLength);
}

Station start(0, 0, 149, NA);        // Same sweep distance as plates and same number as tomato (for technicalities of the start sequence)
Station tomatoes(0, 4.4, 176, NA);   // 198... All measurements in mm
Station exchange(1, 15, 169, EMPTY); // 191... Only buns are being exchanged here. Top bun height doesn't matter
Station cooktop(2, 10, 173, EMPTY);  // 195.. Only height of patty matters; fries are not being stacked
Station plates(3, 12, 149, NA);      // Previously 24.5, 23.175 - 149... VALUES NOT FINALIZED - replace 40 with the height between the two platforms, replace 150 with the distance the plate must be swept in
Station cheese(10, 4.3, 176, NA);    // 198...
Station lettuce(13, 4.3, 179, NA);   // 201...
Station servingArea(11, 0, 100, NA);

Station currentStation = start;
Station nextStation = plates; // For top bot code, replace this with buns

// std::map<int, Station> numsToStation = {{0, tomato}, {1, exchange}, {2, cooktop}, {3, plates}, {10, cheese}, {13, lettuce}};

int node = -1;

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

    Serial.println("");
    Serial.println("Setup");

    delay(1000);

    // raisePlatform(dcQuarter);
    // delay(2000);
    // previousFoodHeight = plates.height;
    // lowerPlatform(dcQuarter);
    // delay(2000);
    // previousFoodHeight = exchange.height;
    // lowerPlatform(dcQuarter);

    // extendSweeper(dcThreeQs);
    // delay(2000);
    // currentStation = exchange;
    // retractSweeper(dcQuarter, true);

    // raisePlatform(dcQuarter);
    // delay(2000);
    // currentStation = lettuce;
    // nextStation = plates;
    // goNextStation();
    // currentStation = exchange;
    // retractSweeper(dcQuarter, true);

    // driveBackward(dcThreeQs);
    // delay(1500);
    // driveBackward(dcEighth);
    // delay(1000);
    // stopDriving();
    // delay(1000);
    // driveForward(dcThreeQs);
    // delay(1500);
    // driveForward(dcEighth);
    // delay(1000);
    // stopDriving();

    // majorTest2();

    extendSweeper(dcThreeQs);
    delay(5000);
    currentStation = exchange;
    retractSweeper(dcThreeQs, true);

}

void loop()
{
    /*
    // If we are just starting from the start position, execute a different sequence to get set up
    if (currentStation.equals(start))
    {
        raisePlatform(dcQuarter);
        driveUpward(dcHalf); // might have to be downward
        delay(1500); // VALUE NOT FINALIZED - should be enough time that we get to the counter without quite touching it
        node = 0;
        goNextNode(); // From here it will go to plates and act as normal. It won't lower the platform at all since there is a special condition in traverseCounters()
    }
    else
    {
     // Everything else (see description below)
    }
    */

    /*
    This will contain logic that decides where the robot will go next. Once that is decided, all we need to do is call goNextStation().
    So this loop determines the next food station we must go to, assign that to nextStation, call goNextStation(),
    and loop back to the beginning.
    We must ensure that goNextStation() is not called again until we are ready to move to the next station. Do this by checking
    until the variable readyToLeave is true, while making sure to set it back to false afterwards. (Not sure yet if this is needed)
    */
}

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