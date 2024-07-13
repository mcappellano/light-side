#include "sweeper.h"
#include "main.h"

double sweeperPosition = FULLY_RETRACT_POS; // Assuming we have to start fully retracted
int sweepCounter = 0;

void extendSweeper(uint8_t dutyCycle)
{
    analogWrite(SWEEP_MOTOR_OUT, dutyCycle);
    analogWrite(SWEEP_MOTOR_BACK, 0);
}

void retractSweeper(uint8_t dutyCycle)
{
    analogWrite(SWEEP_MOTOR_OUT, 0);
    analogWrite(SWEEP_MOTOR_BACK, dutyCycle);
}

void stopSweeper()
{
    analogWrite(SWEEP_MOTOR_OUT, 0);
    analogWrite(SWEEP_MOTOR_BACK, 0);
}

void sweepSwitchInterrupt()
{
    sweeperPosition = EXTEND_POS;
}

void sweepEncoderInterrupt()
{
    if (digitalRead(SWEEP_ENCODER_2) != digitalRead(SWEEP_ENCODER_1))
        sweepCounter++;
    else
        sweepCounter--;
}