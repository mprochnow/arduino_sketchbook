#include <limits.h>
#include <Servo.h>

#define RIGHT_ANKLE_POS_MIN     900
#define RIGHT_ANKLE_POS_MIDDLE  1510
#define RIGHT_ANKLE_POS_MAX     1950

#define LEFT_ANKLE_POS_MIN      1050
#define LEFT_ANKLE_POS_MIDDLE   1490
#define LEFT_ANKLE_POS_MAX      2200

#define RIGHT_HIP_POS_MIN       960
#define RIGHT_HIP_POS_MIDDLE    1290
#define RIGHT_HIP_POS_MAX       1600

#define LEFT_HIP_POS_MIN        1100
#define LEFT_HIP_POS_MIDDLE     1500
#define LEFT_HIP_POS_MAX        1870

#define RIGHT_ANKLE_PIN         5
#define LEFT_ANKLE_PIN          4
#define RIGHT_HIP_PIN           3
#define LEFT_HIP_PIN            2
#define SR04_TRIGGER_PIN        8
#define SR04_ECHO_PIN           9
#define RANDOM_SEED_PIN         A0      // Take noise from unconnected analog pin to seed RNG

#define DELAY                   1500
#define STOP_DISTANCE           20

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

        delayMicroseconds(DELAY);
    }
}

Joint rightAnkle(RIGHT_ANKLE_PIN, RIGHT_ANKLE_POS_MIN, RIGHT_ANKLE_POS_MIDDLE, RIGHT_ANKLE_POS_MAX);
Joint leftAnkle(LEFT_ANKLE_PIN, LEFT_ANKLE_POS_MIN, LEFT_ANKLE_POS_MIDDLE, LEFT_ANKLE_POS_MAX);
Joint rightHip(RIGHT_HIP_PIN, RIGHT_HIP_POS_MIN, RIGHT_HIP_POS_MIDDLE, RIGHT_HIP_POS_MAX);
Joint leftHip(LEFT_HIP_PIN, LEFT_HIP_POS_MIN, LEFT_HIP_POS_MIDDLE, LEFT_HIP_POS_MAX);

bool started = false;

long getDistance()
{
    long duration = 0;
    
    digitalWrite(SR04_TRIGGER_PIN, LOW);
    delayMicroseconds(5);
    digitalWrite(SR04_TRIGGER_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(SR04_TRIGGER_PIN, LOW);

    duration = pulseIn(SR04_ECHO_PIN, HIGH);

    return duration * 0.01716;
}

void turnLeft()
{
    Move leanLeft[] = { 
        Move(&rightAnkle, Joint::POS_MIDDLE, Joint::POS_MIN), 
        Move(&leftAnkle, Joint::POS_MIDDLE, Joint::POS_MIN)
    };

    Move rotateLeftHip[] = {
        Move(&rightAnkle, Joint::POS_MIN, Joint::POS_MIDDLE),
        Move(&leftAnkle, Joint::POS_MIN, Joint::POS_MIDDLE),
        Move(&leftHip, Joint::POS_MIDDLE, Joint::POS_MAX)
    };

    Move leanRight[] = {
        Move(&rightAnkle, Joint::POS_MIDDLE, Joint::POS_MAX),
        Move(&leftAnkle, Joint::POS_MIDDLE, Joint::POS_MAX)
    };

    Move rotateRightHip[] = {
        Move(&rightAnkle, Joint::POS_MAX, Joint::POS_MIDDLE),
        Move(&leftAnkle, Joint::POS_MAX, Joint::POS_MIDDLE),
        Move(&rightHip, Joint::POS_MIDDLE, Joint::POS_MAX),
        Move(&leftHip, Joint::POS_MAX, Joint::POS_MIDDLE)
    };

    Move rotateBothHips[] = {
        Move(&rightAnkle, Joint::POS_MIN, Joint::POS_MIDDLE),
        Move(&leftAnkle, Joint::POS_MIN, Joint::POS_MIDDLE),
        Move(&rightHip, Joint::POS_MAX, Joint::POS_MIDDLE),
        Move(&leftHip, Joint::POS_MIDDLE, Joint::POS_MAX)
    };

    Move finishTurn[] = {
        Move(&rightAnkle, Joint::POS_MAX, Joint::POS_MIDDLE), 
        Move(&leftAnkle, Joint::POS_MAX, Joint::POS_MIDDLE),
        Move(&leftHip, Joint::POS_MAX, Joint::POS_MIDDLE)
    };

    moveJoints(2, leanLeft);
    moveJoints(3, rotateLeftHip);
    moveJoints(2, leanRight);
    moveJoints(4, rotateRightHip);
    moveJoints(2, leanLeft);
    moveJoints(4, rotateBothHips);
    moveJoints(2, leanRight);
    moveJoints(3, finishTurn);
}

void turnRight()
{
    Move leanRight[] = {
        Move(&rightAnkle, Joint::POS_MIDDLE, Joint::POS_MAX),
        Move(&leftAnkle, Joint::POS_MIDDLE, Joint::POS_MAX)
    };

    Move rotateRightHip[] = {
        Move(&rightAnkle, Joint::POS_MAX, Joint::POS_MIDDLE),
        Move(&leftAnkle, Joint::POS_MAX, Joint::POS_MIDDLE),
        Move(&rightHip, Joint::POS_MIDDLE, Joint::POS_MIN)
    };

    Move leanLeft[] = {
        Move(&rightAnkle, Joint::POS_MIDDLE, Joint::POS_MIN),
        Move(&leftAnkle, Joint::POS_MIDDLE, Joint::POS_MIN)
    };

    Move rotateLeftHip[] = {
        Move(&rightAnkle, Joint::POS_MIN, Joint::POS_MIDDLE),
        Move(&leftAnkle, Joint::POS_MIN, Joint::POS_MIDDLE),
        Move(&rightHip, Joint::POS_MIN, Joint::POS_MIDDLE),
        Move(&leftHip, Joint::POS_MIDDLE, Joint::POS_MIN)
    };

    Move rotateBothHips[] = {
        Move(&rightAnkle, Joint::POS_MAX, Joint::POS_MIDDLE),
        Move(&leftAnkle, Joint::POS_MAX, Joint::POS_MIDDLE),
        Move(&rightHip, Joint::POS_MIDDLE, Joint::POS_MIN),
        Move(&leftHip, Joint::POS_MIN, Joint::POS_MIDDLE)
    };

    Move finishTurn[] = {
        Move(&rightAnkle, Joint::POS_MIN, Joint::POS_MIDDLE),
        Move(&leftAnkle, Joint::POS_MIN, Joint::POS_MIDDLE),
        Move(&rightHip, Joint::POS_MIN, Joint::POS_MIDDLE)
    };
    
    moveJoints(2, leanRight);
    moveJoints(3, rotateRightHip);
    moveJoints(2, leanLeft);
    moveJoints(4, rotateLeftHip);
    moveJoints(2, leanRight);
    moveJoints(4, rotateBothHips);
    moveJoints(2, leanLeft);
    moveJoints(3, finishTurn);
}

void startStepFoward()
{
    Move startLeanLeft[] = { 
        Move(&rightAnkle, Joint::POS_MIDDLE, Joint::POS_MIN), 
        Move(&leftAnkle, Joint::POS_MIDDLE, Joint::POS_MIN)
    };
    
    moveJoints(2, startLeanLeft);
}

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

    if (getDistance() <= STOP_DISTANCE)
    {
        Move stepDown[] = {
            Move(&rightAnkle, Joint::POS_MAX, Joint::POS_MIDDLE),
            Move(&leftAnkle, Joint::POS_MAX, Joint::POS_MIDDLE),
        };

        moveJoints(2, stepDown);

        if (random(2))
        {
            turnLeft();
        }
        else
        {
            turnRight();
        }

        startStepFoward();
        
        return;
    }
    
    moveJoints(4, stepRight);
    moveJoints(2, leanLeft);
    moveJoints(2, rotateFromRightToCenter);

    if (getDistance() <= STOP_DISTANCE)
    {
        Move stepDown[] = {
            Move(&rightAnkle, Joint::POS_MIN, Joint::POS_MIDDLE),
            Move(&leftAnkle, Joint::POS_MIN, Joint::POS_MIDDLE),
        };

        moveJoints(2, stepDown);

        if (random(2))
        {
            turnLeft();
        }
        else
        {
            turnRight();
        }

        startStepFoward();

        return;
    }
}


void setup()
{
    pinMode(SR04_TRIGGER_PIN, OUTPUT);
    pinMode(SR04_ECHO_PIN, INPUT);

    randomSeed(analogRead(RANDOM_SEED_PIN));
    
    rightAnkle.init();
    leftAnkle.init();
    rightHip.init();
    leftHip.init();
}

void loop()
{
    int i = 0;

    if (!started)
    {
        delay(5000);
        startStepFoward();
        started = true;
    }

    stepForward();
}

