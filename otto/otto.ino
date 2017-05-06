#include <limits.h>
#include <Servo.h>

#define DELAY 2

struct Joint
{
    enum Position
    {
        POS_MIN,
        POS_MIDDLE,
        POS_MAX
    };
    
    const int pin;
    const int posMin;
    const int posMiddle;
    const int posMax;
    Servo servo;

    Joint(
        const int pin, 
        const int posMin, 
        const int posMiddle, 
        const int posMax) :
        pin(pin), 
        posMin(posMin), 
        posMiddle(posMiddle), 
        posMax(posMax)
    {
    }

    void init()
    {
        this->servo.attach(this->pin);
        this->servo.writeMicroseconds(this->posMiddle);
    }

    const int getValue(Joint::Position pos) const
    {
        switch (pos)
        {
            case POS_MIN: return this->posMin;
            case POS_MIDDLE: return this->posMiddle;
            case POS_MAX: return this->posMax;
            default: return 0;
        }
    }

    void setPosition(const int pos) const
    {
        this->servo.writeMicroseconds(pos);
    }
};

struct Move
{
    Joint* joint;
    Joint::Position from;
    Joint::Position to;

    int direction = 0;
    int distance = 0;
    float increment = 0.0;

    Move(Joint* joint, Joint::Position from, Joint::Position to):
        joint(joint), from(from), to(to)
    {}
};

void moveJoints(const int numMoves, Move* moves)
{
    int i = 0, j = 0;
    int minDistance = INT_MAX;

    for (i = 0; i < numMoves; ++i)
    {
        Move* move = &moves[i];
        const Joint* joint = move->joint;
        const int from = joint->getValue(move->from);
        const int to = joint->getValue(move->to);

        // determine move direction and distance
        if (from > to)
        {
            move->direction = -1;
            move->distance = from - to;
        }
        else
        {
            move->direction = 1;
            move->distance = to - from;
        }

        // determine minimum move distance of all joints
        if (move->distance < minDistance)
        {
            minDistance = move->distance;
        }
    }

    // determine increment for each position update
    for (i = 0; i < numMoves; ++i)
    {
        Move* move = &moves[i];
        const Joint* joint = move->joint;

        move->increment = static_cast<float>(move->distance) / static_cast<float>(minDistance);
    }

    // move servos
    for (i = 0; i < minDistance; ++i)
    {
        for (j = 0; j < numMoves; ++j)
        {
            Move* move = &moves[j];
            const Joint* joint = move->joint;
            int pos = joint->getValue(move->from) + move->direction * i * move->increment;

            joint->setPosition(static_cast<float>(pos));
        }

        delay(DELAY);
    }
}

Joint rightAnkle(5, 950, 1510, 1860);
Joint leftAnkle(4, 1140, 1490, 2190);
Joint rightHip(3, 950, 1290, 1650);
Joint leftHip(2, 1100, 1500, 1930);

void stepForward() {
    Move stepLeft[] = {
        Move(&rightAnkle, Joint::POS_MIN, Joint::POS_MIDDLE),
        Move(&leftAnkle, Joint::POS_MIN, Joint::POS_MIDDLE),
        Move(&rightHip, Joint::POS_MIDDLE, Joint::POS_MAX),
        Move(&leftHip, Joint::POS_MIDDLE, Joint::POS_MAX)
    };

    Move leanRight[] = {
        Move(&rightAnkle, Joint::POS_MIDDLE, Joint::POS_MAX),
        Move(&leftAnkle, Joint::POS_MIDDLE, Joint::POS_MAX)
    };

    Move rotateFromLeftToCenter[] = {
        Move(&rightHip, Joint::POS_MAX, Joint::POS_MIDDLE),
        Move(&leftHip, Joint::POS_MAX, Joint::POS_MIDDLE)
    };

    Move stepRight[] = {
        Move(&rightAnkle, Joint::POS_MAX, Joint::POS_MIDDLE),
        Move(&leftAnkle, Joint::POS_MAX, Joint::POS_MIDDLE),
        Move(&rightHip, Joint::POS_MIDDLE, Joint::POS_MIN),
        Move(&leftHip, Joint::POS_MIDDLE, Joint::POS_MIN)
    };

    Move leanLeft[] = {
        Move(&rightAnkle, Joint::POS_MIDDLE, Joint::POS_MIN),
        Move(&leftAnkle, Joint::POS_MIDDLE, Joint::POS_MIN)
    };

    Move rotateFromRightToCenter[] = {
        Move(&rightHip, Joint::POS_MIN, Joint::POS_MIDDLE),
        Move(&leftHip, Joint::POS_MIN, Joint::POS_MIDDLE)
    };

    moveJoints(4, stepLeft);
    moveJoints(2, leanRight);
    moveJoints(2, rotateFromLeftToCenter);
    moveJoints(4, stepRight);
    moveJoints(2, leanLeft);
    moveJoints(2, rotateFromRightToCenter);
}

void setup() {
    rightAnkle.init();
    leftAnkle.init();
    rightHip.init();
    leftHip.init();
}

bool started = false;

void loop()
{
    int i = 0;

    if (!started)
    {
        Move startLeanLeft[] = { 
            Move(&rightAnkle, Joint::POS_MIDDLE, Joint::POS_MIN), 
            Move(&leftAnkle, Joint::POS_MIDDLE, Joint::POS_MIN)
        };

        delay(5000);

        moveJoints(2, startLeanLeft);

        started = true;
    }

    stepForward();
}

