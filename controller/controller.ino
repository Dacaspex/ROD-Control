
#include <Servo.h>

// Gimball system
Servo gimballServoX;
Servo gimballServoY;
const int gimballInitPosX = 90;
const int gimballInitPosY = 90;
int gimballPosX = 90;
int gimballPosY = 90;

// Gimball pins
int gimballServoPinX = A0;
int gimballServoPinY = A1;

// Movement system
int speed = 255;

// Movement pins (PWM)
int leftPWM1 = 3;
int leftPWM2 = 5;
int rightPWM1 = 6;
int rightPWM2 = 11;

// Controll keys
const char forwardKey = 'w';
const char backwardKey = 's';
const char leftKey = 'a';
const char rightKey = 'd';

const char gimballUpKey = 'i';
const char gimballDownKey = 'k';
const char gimballLeftKey = 'j';
const char gimballRightKey = 'l';
const char gimballNeutralKey = 'o';

void setup() {

    // Setup gimball system
    gimballServoX.attach(gimballServoPinX);
    gimballServoY.attach(gimballServoPinY);
    resetGimballPos();

    // Setup movement system (PWM)
    analogWrite(leftPWM1, 0);
    analogWrite(leftPWM2, 0);
    analogWrite(rightPWM1, 0);
    analogWrite(rightPWM2, 0);

    // Setup serial
    Serial1.begin(115200);

    // Clear the input buffer
    while (Serial1.available()) {
        Serial1.read();
    }

}

void loop() {

    if (Serial1.available() > 0) {
        char command = Serial1.read();

        switch (command) {
            // Gimball
            case gimballUpKey:
                moveGimballUp();
                break;
            case gimballDownKey:
                moveGimballDown();
                break;
            case gimballLeftKey:
                moveGimballLeft();
                break;
            case gimballRightKey:
                moveGimballRight();
                break;
            case gimballNeutralKey:
                resetGimballPos();
                break;

            // Movement
            case forwardKey:
                moveForward();
                break;
            case backwardKey:
                moveBackward();
                break;
            case leftKey:
                moveLeft();
                break;
            case rightKey:
                moveRight();
                break;
        }

    }

}

/**
 * Resets the gimball to it's initial position
 */
void resetGimballPos() {
    gimballPosX = gimballInitPosX;
    gimballPosY = gimballInitPosY;
    gimballServoX.write(gimballPosX);
    gimballServoY.write(gimballPosY);
}

void stop() {
    analogWrite(leftPWM1, 0);
    analogWrite(leftPWM2, 0);
    analogWrite(rightPWM1, 0);
    analogWrite(rightPWM2, 0);
}

void moveForward() {
    analogWrite(leftPWM1, speed);
    analogWrite(leftPWM2, 0);
    analogWrite(rightPWM1, speed);
    analogWrite(rightPWM2, 0);
}

void moveBackward() {
    analogWrite(leftPWM1, 0);
    analogWrite(leftPWM2, speed);
    analogWrite(rightPWM1, 0);
    analogWrite(rightPWM2, speed);
}

void moveLeft() {
    analogWrite(leftPWM1, speed);
    analogWrite(leftPWM2, 0);
    analogWrite(rightPWM1, 0);
    analogWrite(rightPWM2, speed);
}

void moveRight() {
    analogWrite(leftPWM1, 0);
    analogWrite(leftPWM2, speed);
    analogWrite(rightPWM1, speed);
    analogWrite(rightPWM2, 0);
}

void moveGimballUp() {
    gimballPosX += 5;
}

void moveGimballDown() {
    gimballPosX -= 5;
}

void moveGimballLeft() {
    gimballPosY -= 5;
}

void moveGimballRight() {
    gimballPosY += 5;
}
