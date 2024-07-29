#include "drive.h"
#include "main.h"

hw_timer_t *accelTimer = NULL;
uint8_t currentDutyCycle = 0;
volatile double loopNum = 0;
bool accelForward = true;

uint8_t speeds[4] = {0, 0, 0, 0};
uint32_t freqHz = 50;
// uint8_t dcSixteenth = 15;
// uint8_t dcMin = 24;
uint8_t dcEighth = 31;
uint8_t dc316 = 48;
uint8_t dcQuarter = 63;
// uint8_t dcHalf = 127;
uint8_t dcThreeQs = 191;
// uint8_t dcMax = 245; // FOR SOME REASON, the motor buzzes when ran at 100% and doesn't spin. But 245 works (96%).

void crossCounters()
{
    /*
    // WITHOUT WEIGHT OF ELEVATOR AND SWEEPER
    driveDownward(dcQuarter);
    delay(475);
    spinAround(dcQuarter);
    delay(650);
    stopDriving();
    delay(150);
    driveUpward(dcQuarter);
    delay(350);
    driveUpward(dcEighth);
    delay(800);
    stopDriving();
    */

    // WITH ELEVATOR AND SWEEPER
    driveDownward(dcQuarter);
    delay(550);
    spinAround(dcQuarter);
    delay(975); // 950
    stopDriving();
    delay(300);
    driveUpward(dcQuarter);
    delay(500);
    driveUpward(dcEighth);
    delay(1200);
    stopDriving();

    // Update where we are
    if (currentStation.num >= 10)
        node -= 10;
    else
        node += 10;

    // Edge cases for stations directly across from each other
    if (abs(nextStation.num - currentStation.num) == 10)
    {
        if (currentStation.equals(cheese) || currentStation.equals(tomatoes))
            node++;
        if (currentStation.equals(plates) || currentStation.equals(lettuce))
            node--;
    }
}

/* Find the perfect combination of motors speeds
so that the robot moves counter to counter
while simulatenously rotating 180 degrees
(consult image I found online and add the vectors to the two motions we want) */
void crossCountersCool()
{
    // IMPORTANT NOTE: double duty cycle is not necessarily double RPM
    changingPWMs(dcQuarter, dcEighth, false, dcQuarter, dcQuarter, true, 0, dcEighth, true, 0, 0, true, 5);
    changingPWMs(dcEighth, 0, false, dcQuarter, dcQuarter, true, dcEighth, dcQuarter, true, 0, 0, true, 5);
    changingPWMs(0, 0, false, dcQuarter, dcEighth, true, dcQuarter, dcQuarter, true, 0, dcEighth, false, 5);
    changingPWMs(0, 0, false, dcEighth, 0, true, dcQuarter, dcQuarter, true, dcEighth, dcQuarter, false, 5);

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

void calibrateDutyCycle(uint8_t dutyCycle)
{
    uint8_t motor1 = 0;
    uint8_t motor2 = 0;
    uint8_t motor3 = 0;
    uint8_t motor4 = 0;

    if (abs(dutyCycle - dcEighth) <= 10) // Driving at or near 1/8 duty cycle - 162 RPM
    {
        motor1 = dutyCycle * 1;
        motor2 = dutyCycle * 0.96;
        motor3 = dutyCycle * 1.088;
        motor4 = dutyCycle * 0.88;
    }
    else if (abs(dutyCycle - dc316) < 5) // Driving at or near 3/16 duty cycle - 220 RPM
    {
        motor1 = dutyCycle * 0.987;
        motor2 = dutyCycle * 1.013;
        motor3 = dutyCycle * 1.094;
        motor4 = dutyCycle * 0.823;
    }
    else if (abs(dutyCycle - dcQuarter) < 15) // Driving at or near 1/4 duty cycle - 264 RPM
    {
        motor1 = dutyCycle * 1.412;
        motor2 = dutyCycle * 1.44;
        motor3 = dutyCycle * 1.48;
        motor4 = dutyCycle * 1;
    }
    else if (abs(dutyCycle - dcThreeQs) < 15) // Driving at or near 3/4 duty cycle - 288 RPM
    {
        motor1 = dutyCycle * 1.186;
        motor2 = dutyCycle * 1.333;
        motor3 = dutyCycle * 1.26;
        motor4 = dutyCycle * 0.56;
    }

    // uint8_t calibrated[4] = {motor1, motor2, motor3, motor4};
    // return calibrated;
    speeds[0] = motor1;
    speeds[1] = motor2;
    speeds[2] = motor3;
    speeds[3] = motor4;
}

void spinAround(uint8_t dutyCycle)
{
    // uint8_t *speeds = calibrateDutyCycle(dutyCycle);
    calibrateDutyCycle(dutyCycle);

    analogWrite(motor1F, speeds[0]);
    analogWrite(motor1B, 0);

    analogWrite(motor2F, 0);
    analogWrite(motor2B, speeds[1]);

    analogWrite(motor3F, 0);
    analogWrite(motor3B, speeds[2]);

    analogWrite(motor4F, speeds[3]);
    analogWrite(motor4B, 0);
}

void driveForward(uint8_t dutyCycle)
{
    int waitTime = 0;
    calibrateDutyCycle(dutyCycle);

    // Further tuning to make it drive slightly into the wall
    if (dutyCycle == dcThreeQs)
    {
        speeds[0] += 8; // 2
        speeds[3] += 8; // 2
        speeds[1] += 0;
        speeds[2] += 0;
        waitTime = 80; // 50
    }

    if (dutyCycle == dcEighth)
    {
        speeds[0] += 1;
        speeds[3] += 1;
        speeds[1] += 0;
        speeds[2] += 0;
    }

    analogWrite(motor1F, speeds[0] + 2);
    analogWrite(motor1B, 0);

    analogWrite(motor4F, speeds[3] + 2);
    analogWrite(motor4B, 0);

    delay(waitTime);

    analogWrite(motor2F, speeds[1]);
    analogWrite(motor2B, 0);

    analogWrite(motor3F, speeds[2]);
    analogWrite(motor3B, 0);
}

void driveBackward(uint8_t dutyCycle)
{
    int waitTime = 0;
    calibrateDutyCycle(dutyCycle);

    // Further tuning to make it drive slightly into the wall
    if (dutyCycle == dcThreeQs)
    {
        speeds[0] -= 2; // 6
        speeds[3] -= 2; // 6
        speeds[1] += 0;
        speeds[2] += 0;
        waitTime = 70;
    }

    if (dutyCycle == dcEighth)
    {
        speeds[0] -= 2;
        speeds[3] -= 2;
        speeds[1] += 0;
        speeds[2] += 0;
    }

    analogWrite(motor1F, 0);
    analogWrite(motor1B, speeds[0]);

    analogWrite(motor4F, 0);
    analogWrite(motor4B, speeds[3]);

    delay(waitTime); // Motors 2 and 3 seem to start a tiny bit early

    analogWrite(motor2F, 0);
    analogWrite(motor2B, speeds[1]);

    analogWrite(motor3F, 0);
    analogWrite(motor3B, speeds[2]);
}

// Speeds up more gradually
void driveForward2(uint8_t dutyCycle)
{
    // loopNum = 0;
    // accelForward = true;
    // currentDutyCycle = dutyCycle;
    // timerWrite(accelTimer, 0);
    // timerAlarmEnable(accelTimer);

    uint8_t gradualDC = dutyCycle * 0.2;
    for (int i = 0; i < 4; i++)
    {
        driveForward(gradualDC);
        gradualDC += dutyCycle * 0.2;
        delay(50);
    }
}

void driveBackward2(uint8_t dutyCycle)
{
    // loopNum = 0;
    // accelForward = false;
    // currentDutyCycle = dutyCycle;
    // timerWrite(accelTimer, 0);
    // timerAlarmEnable(accelTimer);

    // float dutyCycle2 = static_cast<float>(dutyCycle);
    // float gradualDC = dutyCycle2 * 0.2;
    // driveBackward(static_cast<uint8_t>(gradualDC));
    // for (int i = 0; i < 4; i++)
    // {
    //     delay(50);
    //     driveBackward(static_cast<uint8_t>(gradualDC));
    //     gradualDC += dutyCycle2 * 0.2;
    //     Serial.println(static_cast<uint8_t>(gradualDC));
    // }
}

void IRAM_ATTR accelTimerInterrupt()
{
    uint8_t gradualDC = currentDutyCycle * ((2.0 + loopNum) / 10.0);
    if (accelForward)
        driveForward(gradualDC);
    else
        driveBackward(gradualDC);

    loopNum++;
    if (2.0 + loopNum >= 10.0)
        timerAlarmDisable(accelTimer);
}

void driveUpward(uint8_t dutyCycle)
{
    calibrateDutyCycle(dutyCycle);

    // Further tuning to make up for uneven weight distribution between wheels
    if (dutyCycle == dcQuarter)
    {
        speeds[0] -= 25;
        speeds[1] -= 45;
        speeds[2] += 0;
        speeds[3] += 0;
    }

    if (dutyCycle == dcEighth)
    {
        speeds[0] += 5;
        speeds[1] += 5;
        speeds[2] += 28;
        speeds[3] += 22;
    }

    analogWrite(motor1F, 0);
    analogWrite(motor1B, speeds[0]); // - 25

    analogWrite(motor2F, speeds[1]); // - 45
    analogWrite(motor2B, 0);

    analogWrite(motor3F, 0);
    analogWrite(motor3B, speeds[2]); // - 0

    analogWrite(motor4F, speeds[3]); // - 0
    analogWrite(motor4B, 0);
}

void driveDownward(uint8_t dutyCycle)
{
    // uint8_t *speeds = calibrateDutyCycle(dutyCycle);
    calibrateDutyCycle(dutyCycle);

    analogWrite(motor1F, speeds[0] - 25); // - 25
    analogWrite(motor1B, 0);

    analogWrite(motor2F, 0);
    analogWrite(motor2B, speeds[1] - 45); // 45

    analogWrite(motor3F, speeds[2]); // - 0
    analogWrite(motor3B, 0);

    analogWrite(motor4F, 0);
    analogWrite(motor4B, speeds[3]); // - 0
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