#include "main.h"
#include "drive.h"
#include "reflectance.h"
#include "elevator.h"
#include "sweeper.h"
#include "tests.h"

Station::Station(int num, double height, int sweepLength) : num(num), height(height), sweepLength(sweepLength) {}

Station start(-1, 0, 0);
Station tomatoes(0, 4.5, 198); // All measurements in mm
Station exchange(1, 15, 191);  // Only buns are being exchanged here. Top bun height doesn't matter
Station cooktop(2, 10, 195);   // Only height of patty matters; fries are not being stacked
Station plates(3, 40, 150);    // VALUES NOT FINALIZED - replace 40 with the height between the two platforms, replace 150 with the distance the plate must be swept in
Station cheese(10, 4.3, 198);
Station lettuce(13, 4.3, 201);
Station servingArea(99, 0, 0); // May or may not need this, may or may not need to change the num value

Station currentStation = start;
Station nextStation = plates;       // For top bot code, replace this with buns
Station nextNextStation = exchange; // Same for top bot

std::map<int, Station> numsToStation = {{0, tomatoes}, {1, exchange}, {2, cooktop}, {3, plates}, {10, cheese}, {13, lettuce}};

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

    attachInterrupt(digitalPinToInterrupt(ELEV_ENCODER_1), elevEncoderInterrupt, RISING);
    attachInterrupt(digitalPinToInterrupt(SWEEP_ENCODER_1), sweepEncoderInterrupt, RISING);

    attachInterrupt(digitalPinToInterrupt(ELEV_SWITCH), elevSwitchInterrupt, FALLING);
    attachInterrupt(digitalPinToInterrupt(SWEEP_SWITCH), sweepSwitchInterrupt, FALLING);

    // Timers
    tapeTimer = timerBegin(0, 80, true);
    timerAttachInterrupt(tapeTimer, &tapeTimerInterrupt, true);
    timerAlarmWrite(tapeTimer, tapedelay_ms * 1000, false);

    Serial.println("");
    Serial.println("Setup");

    // GENERAL TESTING:
    // currentNode = 3;
    // nextStation = 2;
    // traverseCounter(true);
    // driveForward(dcQuarter);
    // driveUpward(dcEighth);
    testElevator();
    // testSweeper(); // DETERMINE SWEEP_PULSE_DISTANCE BEFORE RUNNING THIS TEST
}

void loop()
{
    /*
    if (currentStation.num == start.num)
    {
        // GO TO PLATE STATION, RAISE PLATFORM, EXTEND SWEEPER
    }
    */

    /*
    This will contain logic that decides where the robot will go next. Once that is decided, all we need to do is call goNextStation().
    So this loop determine the next food station we must go to, assign that to nextStation or maybe nextNextStation, call goNextStation(),
    call retractSweeper() (make sure to set sweepCounter to 0 beforehand), and loop back to the beginning.
    We must ensure that goNextStation() is not called again until we are ready to move to the next station. Do this by checking
    until the variable readyToLeave is true, while making sure to set it back to false afterwards.
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
Unneeded OLED code:
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