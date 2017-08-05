#include <Arduino.h>
#include <limits.h>
#include "Robot.h"

Robot::Move::Move(Joint* joint, Joint::Position from, Joint::Position to):
    joint(joint), from(from), to(to)
{}

Robot::Robot(const RobotConfig& cfg):
    rightAnkle(cfg.rightAnkle), leftAnkle(cfg.leftAnkle), rightHip(cfg.rightHip), leftHip(cfg.leftHip),
    delay(cfg.delay), stopDistance(cfg.stopDistance), distanceSensor(cfg.distanceSensor)
{
}

void Robot::setup()
{
    this->rightAnkle.setup();
    this->leftAnkle.setup();
    this->rightHip.setup();
    this->leftHip.setup();
    this->distanceSensor.setup();
}

void Robot::startStepFoward()
{
    Move startLeanLeft[] = { 
        Move(&this->rightAnkle, Joint::POS_MIDDLE, Joint::POS_MIN), 
        Move(&this->leftAnkle, Joint::POS_MIDDLE, Joint::POS_MIN)
    };
    
    this->moveJoints(startLeanLeft);
}

void Robot::stepForward()
{
    int distance = 0;
    
    Move stepLeft[] = {
        Move(&this->rightAnkle, Joint::POS_MIN, Joint::POS_MIDDLE),
        Move(&this->leftAnkle, Joint::POS_MIN, Joint::POS_MIDDLE),
        Move(&this->rightHip, Joint::POS_MIDDLE, Joint::POS_MAX),
        Move(&this->leftHip, Joint::POS_MIDDLE, Joint::POS_MAX)
    };

    Move leanRight[] = {
        Move(&this->rightAnkle, Joint::POS_MIDDLE, Joint::POS_MAX),
        Move(&this->leftAnkle, Joint::POS_MIDDLE, Joint::POS_MAX)
    };

    Move rotateFromLeftToCenter[] = {
        Move(&this->rightHip, Joint::POS_MAX, Joint::POS_MIDDLE),
        Move(&this->leftHip, Joint::POS_MAX, Joint::POS_MIDDLE)
    };

    Move stepRight[] = {
        Move(&this->rightAnkle, Joint::POS_MAX, Joint::POS_MIDDLE),
        Move(&this->leftAnkle, Joint::POS_MAX, Joint::POS_MIDDLE),
        Move(&this->rightHip, Joint::POS_MIDDLE, Joint::POS_MIN),
        Move(&this->leftHip, Joint::POS_MIDDLE, Joint::POS_MIN)
    };

    Move leanLeft[] = {
        Move(&this->rightAnkle, Joint::POS_MIDDLE, Joint::POS_MIN),
        Move(&this->leftAnkle, Joint::POS_MIDDLE, Joint::POS_MIN)
    };

    Move rotateFromRightToCenter[] = {
        Move(&this->rightHip, Joint::POS_MIN, Joint::POS_MIDDLE),
        Move(&this->leftHip, Joint::POS_MIN, Joint::POS_MIDDLE)
    };

    this->moveJoints(stepLeft);
    this->moveJoints(leanRight);
    this->moveJoints(rotateFromLeftToCenter);

    distance = this->distanceSensor.getDistance();

    if (distance > 0 && distance <= this->stopDistance)
    {
        Move stepDown[] = {
            Move(&this->rightAnkle, Joint::POS_MAX, Joint::POS_MIDDLE),
            Move(&this->leftAnkle, Joint::POS_MAX, Joint::POS_MIDDLE),
        };

        this->moveJoints(stepDown);

        if (random(2))
        {
            this->turnLeft();
        }
        else
        {
            this->turnRight();
        }

        this->startStepFoward();
        
        return;
    }
    
    this->moveJoints(stepRight);
    this->moveJoints(leanLeft);
    this->moveJoints(rotateFromRightToCenter);

    distance = this->distanceSensor.getDistance();

    if (distance > 0 && distance <= this->stopDistance)
    {
        Move stepDown[] = {
            Move(&this->rightAnkle, Joint::POS_MIN, Joint::POS_MIDDLE),
            Move(&this->leftAnkle, Joint::POS_MIN, Joint::POS_MIDDLE),
        };

        this->moveJoints(stepDown);

        if (random(2))
        {
            this->turnLeft();
        }
        else
        {
            this->turnRight();
        }

        this->startStepFoward();

        return;
    }
}

void Robot::turnLeft()
{
    Move leanLeft[] = { 
        Move(&this->rightAnkle, Joint::POS_MIDDLE, Joint::POS_MIN), 
        Move(&this->leftAnkle, Joint::POS_MIDDLE, Joint::POS_MIN)
    };

    Move rotateLeftHip[] = {
        Move(&this->rightAnkle, Joint::POS_MIN, Joint::POS_MIDDLE),
        Move(&this->leftAnkle, Joint::POS_MIN, Joint::POS_MIDDLE),
        Move(&this->leftHip, Joint::POS_MIDDLE, Joint::POS_MAX)
    };

    Move leanRight[] = {
        Move(&this->rightAnkle, Joint::POS_MIDDLE, Joint::POS_MAX),
        Move(&this->leftAnkle, Joint::POS_MIDDLE, Joint::POS_MAX)
    };

    Move rotateRightHip[] = {
        Move(&this->rightAnkle, Joint::POS_MAX, Joint::POS_MIDDLE),
        Move(&this->leftAnkle, Joint::POS_MAX, Joint::POS_MIDDLE),
        Move(&this->rightHip, Joint::POS_MIDDLE, Joint::POS_MAX),
        Move(&this->leftHip, Joint::POS_MAX, Joint::POS_MIDDLE)
    };

    Move rotateBothHips[] = {
        Move(&this->rightAnkle, Joint::POS_MIN, Joint::POS_MIDDLE),
        Move(&this->leftAnkle, Joint::POS_MIN, Joint::POS_MIDDLE),
        Move(&this->rightHip, Joint::POS_MAX, Joint::POS_MIDDLE),
        Move(&this->leftHip, Joint::POS_MIDDLE, Joint::POS_MAX)
    };

    Move finishTurn[] = {
        Move(&this->rightAnkle, Joint::POS_MAX, Joint::POS_MIDDLE), 
        Move(&this->leftAnkle, Joint::POS_MAX, Joint::POS_MIDDLE),
        Move(&this->leftHip, Joint::POS_MAX, Joint::POS_MIDDLE)
    };

    this->moveJoints(leanLeft);
    this->moveJoints(rotateLeftHip);
    this->moveJoints(leanRight);
    this->moveJoints(rotateRightHip);
    this->moveJoints(leanLeft);
    this->moveJoints(rotateBothHips);
    this->moveJoints(leanRight);
    this->moveJoints(finishTurn);
}

void Robot::turnRight()
{
    Move leanRight[] = {
        Move(&this->rightAnkle, Joint::POS_MIDDLE, Joint::POS_MAX),
        Move(&this->leftAnkle, Joint::POS_MIDDLE, Joint::POS_MAX)
    };

    Move rotateRightHip[] = {
        Move(&this->rightAnkle, Joint::POS_MAX, Joint::POS_MIDDLE),
        Move(&this->leftAnkle, Joint::POS_MAX, Joint::POS_MIDDLE),
        Move(&this->rightHip, Joint::POS_MIDDLE, Joint::POS_MIN)
    };

    Move leanLeft[] = {
        Move(&this->rightAnkle, Joint::POS_MIDDLE, Joint::POS_MIN),
        Move(&this->leftAnkle, Joint::POS_MIDDLE, Joint::POS_MIN)
    };

    Move rotateLeftHip[] = {
        Move(&this->rightAnkle, Joint::POS_MIN, Joint::POS_MIDDLE),
        Move(&this->leftAnkle, Joint::POS_MIN, Joint::POS_MIDDLE),
        Move(&this->rightHip, Joint::POS_MIN, Joint::POS_MIDDLE),
        Move(&this->leftHip, Joint::POS_MIDDLE, Joint::POS_MIN)
    };

    Move rotateBothHips[] = {
        Move(&this->rightAnkle, Joint::POS_MAX, Joint::POS_MIDDLE),
        Move(&this->leftAnkle, Joint::POS_MAX, Joint::POS_MIDDLE),
        Move(&this->rightHip, Joint::POS_MIDDLE, Joint::POS_MIN),
        Move(&this->leftHip, Joint::POS_MIN, Joint::POS_MIDDLE)
    };

    Move finishTurn[] = {
        Move(&this->rightAnkle, Joint::POS_MIN, Joint::POS_MIDDLE),
        Move(&this->leftAnkle, Joint::POS_MIN, Joint::POS_MIDDLE),
        Move(&this->rightHip, Joint::POS_MIN, Joint::POS_MIDDLE)
    };
    
    this->moveJoints(leanRight);
    this->moveJoints(rotateRightHip);
    this->moveJoints(leanLeft);
    this->moveJoints(rotateLeftHip);
    this->moveJoints(leanRight);
    this->moveJoints(rotateBothHips);
    this->moveJoints(leanLeft);
    this->moveJoints(finishTurn);
}

void Robot::moveJointsImpl(const int numMoves, Move* moves)
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

        delayMicroseconds(this->delay);
    }
}

