#pragma once

#include <Servo.h>

struct JointConfig
{
    const int pin;
    const int posMin;
    const int posMiddle;
    const int posMax;
};

class Joint
{
public:
    enum Position
    {
        POS_MIN,
        POS_MIDDLE,
        POS_MAX
    };
    
    Joint(const JointConfig& cfg);

    void setup();
    const int getValue(Joint::Position pos) const;
    void setPosition(const int pos) const;

private:
    const int pin;
    const int posMin;
    const int posMiddle;
    const int posMax;
    Servo servo;
};

