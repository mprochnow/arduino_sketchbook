#pragma once

struct DistanceSensorConfig
{
    const int triggerPin;
    const int echoPin;
};

class DistanceSensor
{
public:
    DistanceSensor(const DistanceSensorConfig& cfg);

    void setup();
    unsigned long getDistance();
private:
    const int triggerPin;
    const int echoPin;
};

