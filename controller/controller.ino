
#include <Servo.h>

// Gimball system
Servo gimballServoX;
Servo gimballServoY;
const int gimballInitPosX = 90;
const int gimballInitPosY = 90;
int gimballPosX = 90;
int gimballPosY = 90;
int gimballServoPinX = A0;
int gimballServoPinY = A1;

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
