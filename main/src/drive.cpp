#include "drive.h"
#include "main.h"

hw_timer_t *accelTimer = NULL;
uint8_t currentDutyCycle = 0;
volatile int loopNum = 0;
bool accelForward = true;

uint32_t freqHz = 50;
uint8_t dcSixteenth = 15;
uint8_t dcMin = 24;
uint8_t dcEighth = 31;
uint8_t dcQuarter = 63;
uint8_t dcHalf = 127;
uint8_t dcThreeQs = 191;
uint8_t dcMax = 245; // FOR SOME REASON, the motor buzzes when ran at 100% at doesn't spin. But 245 works (96%).

void crossCounters(uint8_t driveSpeed, uint8_t spinSpeed)
{
    driveUpward(driveSpeed); // might have to switch this to be downward, and below to be upward (careful on first test)
    delay(500);
    spinAround(spinSpeed);
    delay(500);
    driveDownward(driveSpeed);
    delay(500);
    stopDriving();

    if (currentStation.num >= 10)
        node -= 10;
    else
        node += 10;
}

/* Find the perfect combination of motors speeds
so that the robot moves counter to counter
while simulatenously rotating 180 degrees
(consult image I found online and add the vectors to the two motions we want) */
void crossCountersCool()
{
    // IMPORTANT NOTE: double duty cycle is not necessarily double RPM
    changingPWMs(dcQuarter, dcEighth, false, dcQuarter, dcQuarter, true, 0, dcEighth, true, 0, 0, true, 10);
    changingPWMs(dcEighth, 0, false, dcQuarter, dcQuarter, true, dcEighth, dcQuarter, true, 0, 0, true, 10);
    changingPWMs(0, 0, false, dcQuarter, dcEighth, true, dcQuarter, dcQuarter, true, 0, dcEighth, false, 10);
    changingPWMs(0, 0, false, dcEighth, 0, true, dcQuarter, dcQuarter, true, dcEighth, dcQuarter, false, 10);

    if (currentStation.num >= 10)
        node -= 10;
    else
        node += 10;
}

void changingPWMs(uint8_t motor1Start, uint8_t motor1End, bool forw1, uint8_t motor2Start, uint8_t motor2End, bool forw2, uint8_t motor3Start, uint8_t motor3End, bool forw3, uint8_t motor4Start, uint8_t motor4End, bool forw4, int loops)
{
    uint8_t motor1Speed = motor1Start;
    uint8_t motor2Speed = motor2Start;
    uint8_t motor3Speed = motor3Start;
    uint8_t motor4Speed = motor4Start;

    stopDriving();

    for (int i = 0; i < loops; i++)
    {
        if (forw1)
            analogWrite(motor1F, motor1Speed);
        else
            analogWrite(motor1B, motor1Speed);

        if (forw2)
            analogWrite(motor2F, motor2Speed);
        else
            analogWrite(motor2B, motor2Speed);

        if (forw3)
            analogWrite(motor3F, motor3Speed);
        else
            analogWrite(motor3B, motor3Speed);

        if (forw4)
            analogWrite(motor4F, motor4Speed);
        else
            analogWrite(motor4B, motor4Speed);

        motor1Speed -= (motor1Start - motor1End) / loops; // This will cause slight rounding errors. Hopefully not enough to notice.
        motor2Speed -= (motor2Start - motor2End) / loops;
        motor3Speed -= (motor3Start - motor3End) / loops;
        motor4Speed -= (motor4Start - motor4End) / loops;
        delay(75); // change the number of loops and the delay to get the timing right
    }
}

uint8_t *calibrateDutyCycle(uint8_t dutyCycle)
{
    uint8_t motor1;
    uint8_t motor2;
    uint8_t motor3;
    uint8_t motor4;

    if (abs(dutyCycle - dcThreeQs) < 15) // Driving at or near 3/4 duty cycle
    {
        motor1 = dutyCycle * 0.7; // These constants are what we have to determine while calibrating
        motor2 = dutyCycle * 0.7;
        motor3 = dutyCycle * 0.7;
        motor4 = dutyCycle * 0.7;
    }
    else if (abs(dutyCycle - dcHalf) < 15) // Driving at or near 1/2 duty cycle
    {
        motor1 = dutyCycle * 0.7;
        motor2 = dutyCycle * 0.7;
        motor3 = dutyCycle * 0.7;
        motor4 = dutyCycle * 0.7;
    }
    else if (abs(dutyCycle - dcQuarter) < 15) // Driving at or near 1/4 duty cycle
    {
        motor1 = dutyCycle * 0.7;
        motor2 = dutyCycle * 0.7;
        motor3 = dutyCycle * 0.7;
        motor4 = dutyCycle * 0.7;
    }
    else if (abs(dutyCycle - dcEighth) < 5) // Driving at or near 1/8 duty cycle
    {
        motor1 = dutyCycle * 0.7;
        motor2 = dutyCycle * 0.7;
        motor3 = dutyCycle * 0.7;
        motor4 = dutyCycle * 0.7;
    }

    uint8_t calibrated[4] = {motor1, motor2, motor3, motor4};
    return calibrated;
}

void spinAround(uint8_t dutyCycle)
{
    analogWrite(motor1F, dutyCycle);
    analogWrite(motor1B, 0);

    analogWrite(motor2F, 0);
    analogWrite(motor2B, dutyCycle);

    analogWrite(motor3F, 0);
    analogWrite(motor3B, dutyCycle);

    analogWrite(motor4F, dutyCycle);
    analogWrite(motor4B, 0);
}

void driveForward(uint8_t dutyCycle)
{
    analogWrite(motor1F, dutyCycle);
    analogWrite(motor1B, 0);

    analogWrite(motor2F, dutyCycle + 16);
    analogWrite(motor2B, 0);

    analogWrite(motor3F, dutyCycle + 16);
    analogWrite(motor3B, 0);

    analogWrite(motor4F, dutyCycle);
    analogWrite(motor4B, 0);
}

void driveBackward(uint8_t dutyCycle)
{
    analogWrite(motor1F, 0);
    analogWrite(motor1B, dutyCycle);

    analogWrite(motor2F, 0);
    analogWrite(motor2B, dutyCycle + 12);

    analogWrite(motor3F, 0);
    analogWrite(motor3B, dutyCycle + 12);

    analogWrite(motor4F, 0);
    analogWrite(motor4B, dutyCycle);
}

// Speeds up more gradually
void driveForward2(uint8_t dutyCycle)
{
    loopNum = 0;
    accelForward = true;
    currentDutyCycle = dutyCycle;
    timerWrite(accelTimer, 0);
    timerAlarmEnable(accelTimer);
}

void driveBackward2(uint8_t dutyCycle)
{
    loopNum = 0;
    accelForward = false;
    currentDutyCycle = dutyCycle;
    timerWrite(accelTimer, 0);
    timerAlarmEnable(accelTimer);
}

void IRAM_ATTR accelTimerInterrupt()
{
    uint8_t gradualDC = currentDutyCycle * ((2 + loopNum) / 10);
    if (accelForward)
        driveForward(gradualDC);
    else
        driveBackward(gradualDC);

    loopNum++;
    if (2 + loopNum >= 10)
        timerAlarmDisable(accelTimer);
}

void driveDiagonal(uint8_t dutyCycle)
{
    analogWrite(motor1F, 0);
    analogWrite(motor1B, dutyCycle * 0.8);

    analogWrite(motor2F, 0);
    analogWrite(motor2B, dutyCycle);

    analogWrite(motor3F, 0);
    analogWrite(motor3B, dutyCycle * 0.8);

    analogWrite(motor4F, 0);
    analogWrite(motor4B, dutyCycle);
}

void driveUpward(uint8_t dutyCycle)
{
    analogWrite(motor1F, 0);
    analogWrite(motor1B, dutyCycle);

    analogWrite(motor2F, dutyCycle);
    analogWrite(motor2B, 0);

    analogWrite(motor3F, 0);
    analogWrite(motor3B, dutyCycle);

    analogWrite(motor4F, dutyCycle);
    analogWrite(motor4B, 0);
}

void driveDownward(uint8_t dutyCycle)
{
    analogWrite(motor1F, dutyCycle);
    analogWrite(motor1B, 0);

    analogWrite(motor2F, 0);
    analogWrite(motor2B, dutyCycle);

    analogWrite(motor3F, dutyCycle);
    analogWrite(motor3B, 0);

    analogWrite(motor4F, 0);
    analogWrite(motor4B, dutyCycle);
}

void stopDriving()
{
    analogWrite(motor1F, 0);
    analogWrite(motor1B, 0);

    analogWrite(motor2F, 0);
    analogWrite(motor2B, 0);

    analogWrite(motor3F, 0);
    analogWrite(motor3B, 0);

    analogWrite(motor4F, 0);
    analogWrite(motor4B, 0);
}
