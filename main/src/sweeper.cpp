#include "sweeper.h"
#include "main.h"

int sweeperPosition = FULLY_RETRACT_POS; // Assuming we have to start fully retracted

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