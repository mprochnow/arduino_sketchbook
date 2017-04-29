#include <Servo.h>

#define RIGHT_ANKLE_PIN 5
#define LEFT_ANKLE_PIN  4
#define RIGHT_HIP_PIN   3
#define LEFT_HIP_PIN    2

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

    rightAnkle.writeMicroseconds(1060);
    leftAnkle.writeMicroseconds(1300);
    rightHip.writeMicroseconds(1300);
    leftHip.writeMicroseconds(1500);
}

bool running = false;
int d = 2;
int factor = 2;
int factor2 = 3;
int diff = 350;
int rightAnkleMiddle = 1100;
int leftAnkleMiddle = 1300;
int rightHipMiddle = 1300;
int leftHipMiddle = 1500;

void loop() {
    int i = 0;

    if (!running) {
        delay(5000);
        
        for (i = 0; i < diff; ++i) {
            rightAnkle.writeMicroseconds(rightAnkleMiddle-i*factor);
            leftAnkle.writeMicroseconds(leftAnkleMiddle-i);
            delay(d);
        }
    
        for (i = 0; i < diff; ++i) {
            leftHip.writeMicroseconds(leftHipMiddle+i);
            delay(d);
        }
    
        for (i = 0; i < diff; ++i) {
            rightAnkle.writeMicroseconds(rightAnkleMiddle - diff*factor+i*factor);
            leftAnkle.writeMicroseconds(leftAnkleMiddle - diff+i);
            delay(d);
        }
    
        for (i = 0; i < diff; ++i) {
            rightAnkle.writeMicroseconds(rightAnkleMiddle+i);
            leftAnkle.writeMicroseconds(leftAnkleMiddle+i*factor2);
            leftHip.writeMicroseconds(leftHipMiddle+diff-i);
            delay(d);
        }
    
        for (i = 0; i < diff; ++i) {
            rightHip.writeMicroseconds(rightHipMiddle-i);
            leftHip.writeMicroseconds(leftHipMiddle-i);
            delay(d);
        }
    
        for (i = 0; i < diff; ++i) {
            rightAnkle.writeMicroseconds(rightAnkleMiddle+diff-i);
            leftAnkle.writeMicroseconds(leftAnkleMiddle+diff*factor2-i*factor2);
            delay(d);
        }
    
        for (i = 0; i < diff; ++i) {
            rightAnkle.writeMicroseconds(rightAnkleMiddle-i*factor2);
            leftAnkle.writeMicroseconds(leftAnkleMiddle-i);
            delay(d);
        }

        running = true;
    }

    stepForward();
}

void stepForward() {
    int i = 0;
    
    for (i = 0; i < diff; ++i) {
        rightHip.writeMicroseconds(rightHipMiddle - diff + i);
        leftHip.writeMicroseconds(leftHipMiddle - diff + i);
        delay(d);
    }

    for (i = 0; i < diff; ++i) {
        rightHip.writeMicroseconds(rightHipMiddle + i);
        leftHip.writeMicroseconds(leftHipMiddle + i);
        rightAnkle.writeMicroseconds(rightAnkleMiddle - diff * factor2 + i * factor2);
        leftAnkle.writeMicroseconds(leftAnkleMiddle - diff + i);
        delay(d);
    }

    for (i = 0; i < diff; ++i) {
        rightAnkle.writeMicroseconds(rightAnkleMiddle + i);
        leftAnkle.writeMicroseconds(leftAnkleMiddle + i * factor2);
        delay(d);
    }

    for (i = 0; i < diff; ++i) {
        rightHip.writeMicroseconds(rightHipMiddle + diff - i);
        leftHip.writeMicroseconds(leftHipMiddle + diff - i);
        delay(d);
    }

    for (i = 0; i < diff; ++i) {
        rightHip.writeMicroseconds(rightHipMiddle - i);
        leftHip.writeMicroseconds(leftHipMiddle - i);
        rightAnkle.writeMicroseconds(rightAnkleMiddle + diff - i);
        leftAnkle.writeMicroseconds(leftAnkleMiddle + diff * factor2 - i * factor2);
        delay(d);
    }

    for (i = 0; i < diff; ++i) {
        rightAnkle.writeMicroseconds(rightAnkleMiddle - i * factor2);
        leftAnkle.writeMicroseconds(leftAnkleMiddle - i);
        delay(d);
    }
}

