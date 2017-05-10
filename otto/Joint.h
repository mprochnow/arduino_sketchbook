#pragma once

#include <Servo.h>

class Joint
{
public:
    enum Position
    {
        POS_MIN,
        POS_MIDDLE,
        POS_MAX
    };
    
    Joint(const int pin, const int posMin, const int posMiddle, const int posMax);

    void init();

    const int getValue(Joint::Position pos) const;
    
    void setPosition(const int pos) const;

private:
    const int pin;
    const int posMin;
    const int posMiddle;
    const int posMax;
    Servo servo;
};

