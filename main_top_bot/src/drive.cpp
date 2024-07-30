#include "drive.h"
#include "main.h"

hw_timer_t *crossTimer = NULL;
bool next = true;
uint8_t speeds[4] = {0, 0, 0, 0};
uint32_t freqHz = 50;
uint8_t dcEighth = 31;
uint8_t dc316 = 48;
uint8_t dcQuarter = 63;
uint8_t dcThreeQs = 191;

// With the weight of the elevator and sweeper (but without fry arm)
void crossCounters()
{
    driveDownward(dcQuarter);
    setCrossTimer(550);
    spinAround(dcQuarter);
    setCrossTimer(880);
    stopDriving();
    setCrossTimer(300);
    // driveUpward(dcQuarter);
    // setCrossTimer(350);
    driveUpward(dcEighth);
    setCrossTimer(1000);
    stopDriving();

    // Update where we are
    if (currentStation.num >= 10)
        node -= 10;
    else
        node += 10;
}

void IRAM_ATTR crossTimerInterrupt()
{
    next = true;
    timerAlarmDisable(crossTimer);
}

void setCrossTimer(int ms)
{
    next = false;
    timerAlarmWrite(crossTimer, ms * 1000, false);
    timerWrite(crossTimer, 0);
    timerAlarmEnable(crossTimer);
    while (!next)
    {
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
        motor1 = dutyCycle * 1.108;
        motor2 = dutyCycle * 0.944;
        motor3 = dutyCycle * 1.156;
        motor4 = dutyCycle * 0.98;
    }
    else if (abs(dutyCycle - dc316) < 5) // Driving at or near 3/16 duty cycle - 220 RPM
    {
        motor1 = dutyCycle * 1.04;
        motor2 = dutyCycle * 1.013;
        motor3 = dutyCycle * 1.173;
        motor4 = dutyCycle * 0.987;
    }
    else if (abs(dutyCycle - dcQuarter) < 15) // Driving at or near 1/4 duty cycle - 264 RPM
    {
        motor1 = dutyCycle * 1.36;
        motor2 = dutyCycle * 1.6;
        motor3 = dutyCycle * 1.4;
        motor4 = dutyCycle * 1.28;
    }
    else if (abs(dutyCycle - dcThreeQs) < 15) // Driving at or near 3/4 duty cycle - 288 RPM
    {
        motor1 = dutyCycle * 0.933;
        motor2 = dutyCycle * 1.333;
        motor3 = dutyCycle * 1.066;
        motor4 = dutyCycle * 0.82;
    }

    speeds[0] = motor1;
    speeds[1] = motor2;
    speeds[2] = motor3;
    speeds[3] = motor4;
}

void spinAround(uint8_t dutyCycle)
{
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
        speeds[0] += 2;
        speeds[3] += 2;
        speeds[1] += 0;
        speeds[2] += 0;
        waitTime = 70;
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
        speeds[0] += 8;
        speeds[3] += 8;
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

    delay(waitTime); // Motors 2 and 3 seem to start a tiny bit early, this helps account for that

    analogWrite(motor2F, 0);
    analogWrite(motor2B, speeds[1]);

    analogWrite(motor3F, 0);
    analogWrite(motor3B, speeds[2]);
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