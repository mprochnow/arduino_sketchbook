#pragma once

#include "Joint.h"
#include "DistanceSensor.h"

struct RobotConfig
{
    const JointConfig rightAnkle;
    const JointConfig leftAnkle;
    const JointConfig rightHip;
    const JointConfig leftHip;
    const unsigned long delay; 
    const unsigned int stopDistance;
    const DistanceSensorConfig distanceSensor;
};

class Robot
{
public:
    struct Move
    {
        Joint* joint;
        Joint::Position from;
        Joint::Position to;
    
        int direction = 0;
        int distance = 0;
        float increment = 0.0;
    
        Move(Joint* joint, Joint::Position from, Joint::Position to);
    };
    
    Robot(const RobotConfig& cfg);

    void setup();
    void startStepFoward();
    void stepForward();
    void turnLeft();
    void turnRight();
    
private:
    template<typename T, size_t n>
    void moveJoints(T* (&moves)[n])
    {
        this->moveJointsImpl(n, moves);
    }

    void moveJointsImpl(const int numMoves, Move** moves);

    const Joint rightAnkle;
    const Joint leftAnkle;
    const Joint rightHip;
    const Joint leftHip;
    const unsigned long delay;
    const unsigned int stopDistance;
    const DistanceSensor distanceSensor;

    Move rightAnkleMiddleToMin;
    Move rightAnkleMinToMiddle;
    Move rightAnkleMiddleToMax;
    Move rightAnkleMaxToMiddle;

    Move leftAnkleMiddleToMin;
    Move leftAnkleMinToMiddle;
    Move leftAnkleMiddleToMax;
    Move leftAnkleMaxToMiddle;

    Move rightHipMiddleToMin;
    Move rightHipMinToMiddle;
    Move rightHipMiddleToMax;
    Move rightHipMaxToMiddle;

    Move leftHipMiddleToMin;
    Move leftHipMinToMiddle;
    Move leftHipMiddleToMax;
    Move leftHipMaxToMiddle;
};


