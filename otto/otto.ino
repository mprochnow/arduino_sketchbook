#include "Robot.h"

#define RIGHT_ANKLE_POS_LEFT    890
#define RIGHT_ANKLE_POS_MIDDLE  1500
#define RIGHT_ANKLE_POS_RIGHT   1790

#define LEFT_ANKLE_POS_LEFT     1130
#define LEFT_ANKLE_POS_MIDDLE   1470
#define LEFT_ANKLE_POS_RIGHT    2180

#define RIGHT_HIP_POS_MIN       940
#define RIGHT_HIP_POS_MIDDLE    1270
#define RIGHT_HIP_POS_MAX       1580

#define LEFT_HIP_POS_MIN        1050
#define LEFT_HIP_POS_MIDDLE     1450
#define LEFT_HIP_POS_MAX        1820

#define RIGHT_ANKLE_PIN         5
#define LEFT_ANKLE_PIN          4
#define RIGHT_HIP_PIN           3
#define LEFT_HIP_PIN            2
#define SR04_TRIGGER_PIN        8
#define SR04_ECHO_PIN           9
#define RANDOM_SEED_PIN         A0      // Take noise from unconnected analog pin to seed RNG

#define DELAY                   1500
#define STOP_DISTANCE           20


const RobotConfig cfg = {
    {RIGHT_ANKLE_PIN, RIGHT_ANKLE_POS_LEFT, RIGHT_ANKLE_POS_MIDDLE, RIGHT_ANKLE_POS_RIGHT},
    {LEFT_ANKLE_PIN, LEFT_ANKLE_POS_LEFT, LEFT_ANKLE_POS_MIDDLE, LEFT_ANKLE_POS_RIGHT},
    {RIGHT_HIP_PIN, RIGHT_HIP_POS_MIN, RIGHT_HIP_POS_MIDDLE, RIGHT_HIP_POS_MAX},
    {LEFT_HIP_PIN, LEFT_HIP_POS_MIN, LEFT_HIP_POS_MIDDLE, LEFT_HIP_POS_MAX},
    DELAY,
    STOP_DISTANCE,
    {SR04_TRIGGER_PIN, SR04_ECHO_PIN}
};

Robot robot(cfg);
bool started = false;

void setup()
{
    randomSeed(analogRead(RANDOM_SEED_PIN));
    
    robot.setup();
}

void loop()
{
    if (!started)
    {
        delay(5000);
        robot.startStepFoward();
        started = true;
    }

    robot.stepForward();
}

