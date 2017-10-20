
#include <Servo.h>

// gimbal system
Servo gimbalServoX;
Servo gimbalServoY;
const int gimbalInitPosX = 90;
const int gimbalInitPosY = 90;
int gimbalPosX = 90;
int gimbalPosY = 90;

// gimbal pins
int gimbalServoPinX = A0;
int gimbalServoPinY = A1;

// gimbal keys
const char gimbalUpKey = 'i';
const char gimbalDownKey = 'k';
const char gimbalLeftKey = 'j';
const char gimbalRightKey = 'l';
const char gimbalNeutralKey = 'o';

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

    // Setup gimbal system
    gimbalServoX.attach(gimbalServoPinX);
    gimbalServoY.attach(gimbalServoPinY);
    resetgimbalPos();

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
            // gimbal
            case gimbalUpKey:
                movegimbalUp();
                break;
            case gimbalDownKey:
                movegimbalDown();
                break;
            case gimbalLeftKey:
                movegimbalLeft();
                break;
            case gimbalRightKey:
                movegimbalRight();
                break;
            case gimbalNeutralKey:
                resetgimbalPos();
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

void updategimbalPos() {
    gimbalServoX.write(gimbalPosX);
    gimbalServoY.write(gimbalPosY);
}

/**
 * Resets the gimbal to it's initial position
 */
void resetgimbalPos() {
    gimbalPosX = gimbalInitPosX;
    gimbalPosY = gimbalInitPosY;
    updategimbalPos();
}

void movegimbalUp() {
    gimbalPosX += 5;
    updategimbalPos();
}

void movegimbalDown() {
    gimbalPosX -= 5;
    updategimbalPos();
}

void movegimbalLeft() {
    gimbalPosY -= 5;
    updategimbalPos();
}

void movegimbalRight() {
    gimbalPosY += 5;
    updategimbalPos();
}

bool containsChar(char haystack[], char needle) {
    for (int i = 0; i <= sizeof(haystack); i++) {
        if (haystack[i] == needle) {
            return true;
        }
    }
    return false;
}
