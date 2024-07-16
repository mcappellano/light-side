#include "sweeper.h"
#include "main.h"

// double sweeperPosition = FULLY_RETRACT_POS; // Assuming we have to start fully retracted
const float SWEEP_PULSE_DISTANCE = 2.618; // VALUE NOT FINALIZED - The distance that the sweeper moves for every pulse sent by the rotary encoder
int sweepCounter = 0;
volatile bool extending = false; // Make sure to set this variable back to false after extending the sweeper when pushing the plate off
                                 // Use a delay for the approximate right amount of time - doesn't have to be perfect
                                 // Delay is fine because nothing else is happening while pushing off
volatile bool readyToLeave = false;

void extendSweeper(uint8_t dutyCycle)
{
    sweepCounter = 0;
    extending = true;
    analogWrite(SWEEP_MOTOR_OUT, dutyCycle);
    analogWrite(SWEEP_MOTOR_BACK, 0);
}

void retractSweeper(uint8_t dutyCycle)
{
    sweepCounter = 0;
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
    if (extending)
    {
        stopSweeper();
        extending = false;
    }
}

void sweepEncoderInterrupt()
{
    if (digitalRead(SWEEP_ENCODER_2) != digitalRead(SWEEP_ENCODER_1))
        sweepCounter++;
    else
        sweepCounter--;

    if (!extending && (sweepCounter >= currentStation.sweepLength / SWEEP_PULSE_DISTANCE - 1)) // Unless sweepCounter goes negative when retracting?
    {
        stopSweeper();
        readyToLeave = true;
    }
}