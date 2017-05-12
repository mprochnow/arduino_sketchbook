#include <Arduino.h>
#include "DistanceSensor.h"

DistanceSensor::DistanceSensor(const DistanceSensorConfig& cfg):
    triggerPin(cfg.triggerPin), echoPin(cfg.echoPin)
{
}

void DistanceSensor::setup()
{
    pinMode(this->triggerPin, OUTPUT);
    pinMode(this->echoPin, INPUT);
}

unsigned long DistanceSensor::getDistance()
{
    unsigned long duration = 0;
    
    digitalWrite(this->triggerPin, LOW);
    delayMicroseconds(5);
    digitalWrite(this->triggerPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(this->triggerPin, LOW);

    duration = pulseIn(this->echoPin, HIGH, 3000);

    return duration * 0.01716;
}

