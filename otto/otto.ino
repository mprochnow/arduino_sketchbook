#include <Servo.h>

#define RIGHT_ANKLE_PIN 5
#define LEFT_ANKLE_PIN  4
#define RIGHT_HIP_PIN   3
#define LEFT_HIP_PIN    2

#define RIGHT_ANKLE_MIDDLE  1500
#define LEFT_ANKLE_MIDDLE   1500
#define RIGHT_HIP_MIDDLE    1300
#define LEFT_HIP_MIDDLE     1500
#define MOVE                350
#define FACTOR              2
#define DELAY               2

Servo rightAnkle;
Servo leftAnkle;
Servo rightHip;
Servo leftHip;

void stepForward();

void setup() {
    rightAnkle.attach(RIGHT_ANKLE_PIN);
    leftAnkle.attach(LEFT_ANKLE_PIN);
    rightHip.attach(RIGHT_HIP_PIN);
    leftHip.attach(LEFT_HIP_PIN);

    rightAnkle.writeMicroseconds(RIGHT_ANKLE_MIDDLE);
    leftAnkle.writeMicroseconds(LEFT_ANKLE_MIDDLE);
    rightHip.writeMicroseconds(RIGHT_HIP_MIDDLE);
    leftHip.writeMicroseconds(LEFT_HIP_MIDDLE);
}

bool running = false;

void loop() {
    int i = 0;

    if (!running) {
        delay(5000);

        // lean left
        for (i = 0; i < MOVE; ++i) {
            rightAnkle.writeMicroseconds(RIGHT_ANKLE_MIDDLE-i*FACTOR);
            leftAnkle.writeMicroseconds(LEFT_ANKLE_MIDDLE-i);
            delay(DELAY);
        }

        // rotate hip forward
        for (i = 0; i < MOVE; ++i) {
            leftHip.writeMicroseconds(LEFT_HIP_MIDDLE+i);
            delay(DELAY);
        }
    
        for (i = 0; i < MOVE; ++i) {
            rightAnkle.writeMicroseconds(RIGHT_ANKLE_MIDDLE - MOVE*FACTOR+i*FACTOR);
            leftAnkle.writeMicroseconds(LEFT_ANKLE_MIDDLE - MOVE+i);
            delay(DELAY);
        }
    
        for (i = 0; i < MOVE; ++i) {
            rightAnkle.writeMicroseconds(RIGHT_ANKLE_MIDDLE+i);
            leftAnkle.writeMicroseconds(LEFT_ANKLE_MIDDLE+i*FACTOR);
            leftHip.writeMicroseconds(LEFT_HIP_MIDDLE+MOVE-i);
            delay(DELAY);
        }
    
        for (i = 0; i < MOVE; ++i) {
            rightHip.writeMicroseconds(RIGHT_HIP_MIDDLE-i);
            leftHip.writeMicroseconds(LEFT_HIP_MIDDLE-i);
            delay(DELAY);
        }
    
        for (i = 0; i < MOVE; ++i) {
            rightAnkle.writeMicroseconds(RIGHT_ANKLE_MIDDLE+MOVE-i);
            leftAnkle.writeMicroseconds(LEFT_ANKLE_MIDDLE+MOVE*FACTOR-i*FACTOR);
            delay(DELAY);
        }
    
        for (i = 0; i < MOVE; ++i) {
            rightAnkle.writeMicroseconds(RIGHT_ANKLE_MIDDLE-i*FACTOR);
            leftAnkle.writeMicroseconds(LEFT_ANKLE_MIDDLE-i);
            delay(DELAY);
        }

        running = true;
    }

    stepForward();
}

void stepForward() {
    int i = 0;
    
    for (i = 0; i < MOVE; ++i) {
        rightHip.writeMicroseconds(RIGHT_HIP_MIDDLE - MOVE + i);
        leftHip.writeMicroseconds(LEFT_HIP_MIDDLE - MOVE + i);
        delay(DELAY);
    }

    for (i = 0; i < MOVE; ++i) {
        rightHip.writeMicroseconds(RIGHT_HIP_MIDDLE + i);
        leftHip.writeMicroseconds(LEFT_HIP_MIDDLE + i);
        rightAnkle.writeMicroseconds(RIGHT_ANKLE_MIDDLE - MOVE * FACTOR + i * FACTOR);
        leftAnkle.writeMicroseconds(LEFT_ANKLE_MIDDLE - MOVE + i);
        delay(DELAY);
    }

    for (i = 0; i < MOVE; ++i) {
        rightAnkle.writeMicroseconds(RIGHT_ANKLE_MIDDLE + i);
        leftAnkle.writeMicroseconds(LEFT_ANKLE_MIDDLE + i * FACTOR);
        delay(DELAY);
    }

    for (i = 0; i < MOVE; ++i) {
        rightHip.writeMicroseconds(RIGHT_HIP_MIDDLE + MOVE - i);
        leftHip.writeMicroseconds(LEFT_HIP_MIDDLE + MOVE - i);
        delay(DELAY);
    }

    for (i = 0; i < MOVE; ++i) {
        rightHip.writeMicroseconds(RIGHT_HIP_MIDDLE - i);
        leftHip.writeMicroseconds(LEFT_HIP_MIDDLE - i);
        rightAnkle.writeMicroseconds(RIGHT_ANKLE_MIDDLE + MOVE - i);
        leftAnkle.writeMicroseconds(LEFT_ANKLE_MIDDLE + MOVE * FACTOR - i * FACTOR);
        delay(DELAY);
    }

    for (i = 0; i < MOVE; ++i) {
        rightAnkle.writeMicroseconds(RIGHT_ANKLE_MIDDLE - i * FACTOR);
        leftAnkle.writeMicroseconds(LEFT_ANKLE_MIDDLE - i);
        delay(DELAY);
    }
}

