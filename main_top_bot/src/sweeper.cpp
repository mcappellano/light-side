#include "sweeper.h"
#include "main.h"
#include "drive.h"

// double sweeperPosition = FULLY_RETRACT_POS; // Assuming we have to start fully retracted
const double SWEEP_PULSE_DISTANCE = 0.6545; // previously 2.618 (4x) - The distance that the sweeper moves for every pulse sent by the rotary encoder
volatile int sweepCounter = 0;
volatile bool extending = false; // Make sure to set this variable back to false after extending the sweeper when pushing the plate off
                                 // Use a delay for the approximate right amount of time - doesn't have to be perfect
                                 // Delay is fine because nothing else is happening while pushing off
volatile bool readyToLeave = false;
volatile bool sweepStopped = false; // true, don't forget to PUT THIS BACK!!!
int sweepPrevious = 0;

void extendSweeper(uint8_t dutyCycle)
{
    sweepCounter = 0;
    extending = true;
    sweepStopped = false;
    analogWrite(SWEEP_MOTOR_OUT, dutyCycle);
    analogWrite(SWEEP_MOTOR_BACK, 0);
}

void retractSweeper(uint8_t dutyCycle, bool reset)
{
    if (reset)
        sweepCounter = 0;

    sweepStopped = false;
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
    int MSB = digitalRead(SWEEP_ENCODER_1);
    int LSB = digitalRead(SWEEP_ENCODER_2);

    int updatedEncoder = (MSB << 1) | LSB;
    int full = (updatedEncoder << 2) | sweepPrevious;

    if ((full == 0b0001) || (full == 0b1000) || (full == 0b0111) || (full == 0b1110))
        sweepCounter++;
    else if ((full == 0b0010) || (full == 0b0100) || (full == 0b1011) || (full == 0b1101))
        sweepCounter--;

    sweepPrevious = updatedEncoder;

    if (!extending && !sweepStopped)
    {
        if (sweepCounter >= (currentStation.sweepLength / SWEEP_PULSE_DISTANCE) - 15) // Unless sweepCounter goes negative when retracting?
        {
            stopSweeper();
            readyToLeave = 1;
            sweepStopped = true;
        }
        // else if (sweepCounter >= (currentStation.sweepLength / SWEEP_PULSE_DISTANCE) - 100)
        //     stopSweeper();
    }
}