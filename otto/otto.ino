#include <Servo.h>

#define DELAY 3

struct Joint
{
    const unsigned int pin;
    const unsigned int posMin;
    const unsigned int posMiddle;
    const unsigned int posMax;
    const unsigned int distMinToMiddle;
    const unsigned int distMaxToMiddle;
    Servo servo;

    unsigned int currentPos;

    Joint(
        const unsigned int pin, 
        const unsigned int posMin, 
        const unsigned int posMiddle, 
        const unsigned int posMax) :
        pin(pin), 
        posMin(posMin), 
        posMiddle(posMiddle), 
        posMax(posMax),
        distMinToMiddle(posMiddle-posMin),
        distMaxToMiddle(posMax-posMiddle),
        currentPos(0)
    {
    }

    void init()
    {
        this->servo.attach(this->pin);
        this->servo.writeMicroseconds(this->posMiddle);
        this->currentPos = this->posMiddle;
    }

    void dec(unsigned int steps = 1)
    {
        this->currentPos -= steps;
        this->servo.writeMicroseconds(this->currentPos);
    }

    void inc(unsigned int steps = 1)
    {
        this->currentPos += steps;
        this->servo.writeMicroseconds(this->currentPos);
    }
};

Joint rightAnkle(5, 810, 1510, 1860);
Joint leftAnkle(4, 1140, 1490, 2190);
Joint rightHip(3, 950, 1300, 1650);
Joint leftHip(2, 1150, 1500, 1850);

/*
Joint rightAnkle(5, 950, 1510, 1860);
Joint leftAnkle(4, 1140, 1490, 2190);
Joint rightHip(3, 950, 1290, 1650);
Joint leftHip(2, 1100, 1500, 1930);
*/

void stepForward();

void setup() {
    rightAnkle.init();
    leftAnkle.init();
    rightHip.init();
    leftHip.init();
}

bool running = false;

void loop()
{
    int i = 0;

    if (!running)
    {
        delay(5000);

        for (i = 0; i < leftAnkle.distMinToMiddle; ++i)
        {
            rightAnkle.dec(2);
            leftAnkle.dec();

            delay(DELAY);
        }

        running = true;
    }

    stepForward();
}

void stepForward() {
    int i = 0;

    // left step
    for (i = 0; i < leftAnkle.distMinToMiddle; ++i)
    {
        rightAnkle.inc(2);
        leftAnkle.inc();
        rightHip.inc();
        leftHip.inc();

        delay(DELAY);
    }

    // lean right
    for (i = 0; i < rightAnkle.distMaxToMiddle; ++i)
    {
        rightAnkle.inc();
        leftAnkle.inc(2);

        delay(DELAY);
    }

    // rotate from left to center
    for (i = 0; i < rightHip.distMaxToMiddle; ++i)
    {
        rightHip.dec();
        leftHip.dec();

        delay(DELAY);
    }

    // right step
    for (i = 0; i < rightAnkle.distMaxToMiddle; ++i) {
        rightAnkle.dec();
        leftAnkle.dec(2);
        rightHip.dec();
        leftHip.dec();

        delay(DELAY);
    }

    // lean left
    for (i = 0; i < leftAnkle.distMinToMiddle; ++i) {
        rightAnkle.dec(2);
        leftAnkle.dec();

        delay(DELAY);
    }

    // rotate from right to center
    for (i = 0; i < rightHip.distMinToMiddle; ++i) {
        rightHip.inc();
        leftHip.inc();

        delay(DELAY);
    }
}

