
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

// Gimball keys
const char gimballUpKey = 'i';
const char gimballDownKey = 'k';
const char gimballLeftKey = 'j';
const char gimballRightKey = 'l';
const char gimballNeutralKey = 'o';

// Movement system
int maxSpeed = 100;
int slowSpeed = 20;
int turningSpeed = 50;

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
char movingKeys[4] = {
    forwardKey,
    backwardKey,
    leftKey,
    rightKey
};

// Timing
int keyDelay = 200;
int lastKeyPress = 0;

/**
 * Initialises program. Ran only once at startup
 */
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

/**
 * Main program loop
 */
void loop() {

    // Check if there are any commands to process
    if (Serial1.available() > 0) {
        char command = Serial1.read();

        // Execute action according to which key was pressed
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

        // Check if the command was a moving command
        if (containsChar(movingKeys, command)) {
            // If so, reset lastKeyPress time
            lastKeyPress = millis();
        } else {
            // If not, check whether the ROD should stop moving
            if (lastKeyPress + delay >= millis()) {
                stopMovement();
            }
        }
    }

}

void stopMovement() {
    analogWrite(leftPWM1, 0);
    analogWrite(leftPWM2, 0);
    analogWrite(rightPWM1, 0);
    analogWrite(rightPWM2, 0);
}

void moveForward() {
    analogWrite(leftPWM1, maxSpeed);
    analogWrite(leftPWM2, 0);
    analogWrite(rightPWM1, maxSpeed);
    analogWrite(rightPWM2, 0);
}

void moveBackward() {
    analogWrite(leftPWM1, 0);
    analogWrite(leftPWM2, maxSpeed);
    analogWrite(rightPWM1, 0);
    analogWrite(rightPWM2, maxSpeed);
}

void moveLeft() {
    analogWrite(leftPWM1, maxSpeed);
    analogWrite(leftPWM2, 0);
    analogWrite(rightPWM1, 0);
    analogWrite(rightPWM2, maxSpeed);
}

void moveRight() {
    analogWrite(leftPWM1, 0);
    analogWrite(leftPWM2, maxSpeed);
    analogWrite(rightPWM1, maxSpeed);
    analogWrite(rightPWM2, 0);
}

void updateGimballPos() {
    gimballServoX.write(gimballPosX);
    gimballServoY.write(gimballPosY);
}

/**
 * Resets the gimball to it's initial position
 */
void resetGimballPos() {
    gimballPosX = gimballInitPosX;
    gimballPosY = gimballInitPosY;
    updateGimballPos();
}

void moveGimballUp() {
    gimballPosX += 5;
    updateGimballPos();
}

void moveGimballDown() {
    gimballPosX -= 5;
    updateGimballPos();
}

void moveGimballLeft() {
    gimballPosY -= 5;
    updateGimballPos();
}

void moveGimballRight() {
    gimballPosY += 5;
    updateGimballPos();
}

bool containsChar(char haystack[], char needle) {
    for (int i = 0; i <= sizeof(haystack); i++) {
        if (haystack[i] == needle) {
            return true;
        }
    }
    return false;
}
