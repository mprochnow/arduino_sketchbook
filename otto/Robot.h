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
    template<typename T, size_t N>
    void moveJoints(T (&moves)[N])
    {
        this->moveJointsImpl(N, moves);
    }

    void moveJointsImpl(const int numMoves, Move* moves);

    const Joint rightAnkle;
    const Joint leftAnkle;
    const Joint rightHip;
    const Joint leftHip;
    const unsigned long delay;
    const unsigned int stopDistance;
    const DistanceSensor distanceSensor;
};


