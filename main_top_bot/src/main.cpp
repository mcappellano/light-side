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

    // raisePlatform(dcQuarter, false);
    // delay(3000);
    
    // lowerPlatform(dcQuarter,true);

    // extendSweeper(dcQuarter);
    // delay(1000);
    // retractSweeper(dcQuarter,false);
    // analogWrite(27,dcQuarter);
    // delay(1000);
    // analogWrite(14,dcQuarter);

    // timeTrials();

    // currentStation = cooktop;
    // raisePlatform(dcQuarter, false);
}

void loop()
{
    Serial.println(sweepCounter);
    delay(10);
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