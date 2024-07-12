#include "elevator.h"
#include "main.h"

const float ELEV_PULSE_DISTANCE = 6.25 / 24;

double platformHeight = MAX_HEIGHT;
double previousHeight = MAX_HEIGHT;
double previousFoodHeight = 0;

int elevCounter = 0;

// The following three functions will have to be adjusted if the motor needs a constant rpm to not fall down the rail.
void raisePlatform(uint8_t dutyCycle)
{
    analogWrite(ELEV_MOTOR_UP, dutyCycle);
    analogWrite(ELEV_MOTOR_DOWN, 0);
}

// To go downwards, it will likely be enough to just turn off the UP direction PWM and let gravity to it. There may not even be a need for the DOWN pin.
void lowerPlatform(uint8_t dutyCycle)
{
    analogWrite(ELEV_MOTOR_UP, 0);
    analogWrite(ELEV_MOTOR_DOWN, dutyCycle);
}

// Run at the base rpm that makes the elevator not move at all (not necessarily 0)
void stopPlatform()
{
    analogWrite(ELEV_MOTOR_UP, 0);
    analogWrite(ELEV_MOTOR_DOWN, 0);
}

void swingOut()
{
    // TO DO
}

void swingIn()
{
    // TO DO
}

void elevSwitchInterrupt()
{
    platformHeight = MAX_HEIGHT;
}

void elevEncoderInterrupt()
{
    if (digitalRead(ELEV_ENCODER_2) != digitalRead(ELEV_ENCODER_1))
        elevCounter++;
    else
        elevCounter--;
}

void testElevator()
{
    raisePlatform(100);

    delay(250);
    stopPlatform();

    // while (true)
    // {
    //     Serial.println(elevCounter);
    //     if (platformHeight == MAX_HEIGHT)
    //     {
    //         stopPlatform();
    //         break;
    //     }
    // }

    float distanceMoved = elevCounter * ELEV_PULSE_DISTANCE;
    Serial.println("");
    Serial.println("Number of ticks:");
    Serial.println(elevCounter);
    Serial.println("");
    Serial.println("Distance moved:");
    Serial.println(distanceMoved);
}