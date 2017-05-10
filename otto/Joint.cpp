#include "Joint.h"

Joint::Joint(const int pin, const int posMin, const int posMiddle, const int posMax) :
    pin(pin), posMin(posMin), posMiddle(posMiddle), posMax(posMax)
{
}

void Joint::init()
{
    this->servo.attach(this->pin);
    this->servo.writeMicroseconds(this->posMiddle);
}

const int Joint::getValue(Joint::Position pos) const
{
    switch (pos)
    {
        case POS_MIN: return this->posMin;
        case POS_MIDDLE: return this->posMiddle;
        case POS_MAX: return this->posMax;
        default: return 0;
    }
}

void Joint::setPosition(const int pos) const
{
    this->servo.writeMicroseconds(pos);
}

