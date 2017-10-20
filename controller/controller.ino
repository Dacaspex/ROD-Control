
#include <Servo.h>

/*
 * Control, servos, pins, properties
 */

/* Basket system */
Servo basketServo;
int basketServoPin = A4;
// The servo stops rotating if set to this value, otherwise it starts moving
int const basketStopAngle = 90;
int const basketForwardAngle = 100;
int const basketBackwardAngle = 80;

/* Pully system */
Servo pullyServo1;
Servo pullyServo2;
int pullyServo1Pin = A2;
int pullyServo2Pin = A3;
int pullyServo1Pos = 90;
int pullyServo2Pos = 90;
// We're dealing with 180 servos
int const maxPullyAngle = 180;
int const minPullyAngle = 0;

/* gimbal system */
Servo gimbalServoX;
Servo gimbalServoY;
int gimbalServoPinX = A0;
int gimbalServoPinY = A1;
// Initial positions
const int gimbalInitPosX = 90;
const int gimbalInitPosY = 90;
// The current angles
int gimbalPosX = 90;
int gimbalPosY = 90;

/* Movement system */
// Max speed is the full speed
int maxSpeed = 80;
// Slow speed for approaching victims
int slowSpeed = 20;
// Inner wheel rotates at lower speed in order to turn
int turningMaxSpeed = 20;
// Also at a lower velocity
int turningSlowSpeed = 5;
// Driving mode; 1: Full speed 2: Turtle speed
int drivingMode = 1;
// Are we driving?
bool isDriving = false;
// Movement pins (PWM)
// Left wheel, forward
int leftPWM1 = 3;
// Left wheel, backward
int leftPWM2 = 5;
// Right wheel, forward
int rightPWM1 = 6;
// Right wheel, you get the idea
int rightPWM2 = 11;

/*
 * Keys
 */

// Basket keys
const char basketLeftKey = 'z';
const char basketStopKey = 'x';
const char basketRightKey = 'c';

// Pully keys
const char pullUpKey = 'm';
const char pullDownKey = 'n';

// gimbal keys
const char gimbalUpKey = 'i';
const char gimbalDownKey = 'k';
const char gimbalLeftKey = 'j';
const char gimbalRightKey = 'l';
const char gimbalNeutralKey = 'o';

// Controll keys
const char forwardKey = 'w';
const char backwardKey = 's';
const char leftKey = 'a';
const char rightKey = 'd';
const char stopKey = 'q';
const char changeDrivingModeKey = 'e';
char movingKeys[4] = {
    forwardKey,
    backwardKey,
    leftKey,
    rightKey
};

/*
 * Misc
 */
// Timing
unsigned long keyDelay = 100;
unsigned long lastKeyPress = 0;

// LED
int ledPin = 13;

/**
 * Initialises program. Ran only once at startup
 */
void setup() {

    // Initialise LED on the Arduino
    pinMode(ledPin, OUTPUT);

    // Setup gimbal system, attach servos to the pins
    gimbalServoX.attach(gimbalServoPinX);
    gimbalServoY.attach(gimbalServoPinY);
    resetgimbalPos();

    // Setup movement system (PWM)
    stopDriving();

    // Pully system, attach servos, set initial angle
    pullyServo1.attach(pullyServo1Pin);
    pullyServo2.attach(pullyServo2Pin);
    pullyServo1.write(pullyServo1Pos);
    pullyServo2.write(pullyServo2Pos);

    // Basket, attach servo
    basketServo.attach(basketServoPin);

    // Setup serial, for sending and receiving data (commands, feedback)
    Serial1.begin(115200);
    Serial.begin(9600);

    // Clear the input buffer
    while (Serial1.available()) {
        Serial1.read();
    }

    digitalWrite(ledPin, LOW);
}

/**
 * Main program loop
 */
void loop() {

    // Check if we should stop driving
    if (isDriving && (lastKeyPress + keyDelay <= millis())) {
        isDriving = false;
        stopDriving();
    }

    // Check if there are any commands to process
    if (Serial1.available() > 0) {

        char command = Serial1.read();

        // Execute action according to which key was pressed
        switch (command) {
            // Pully
            case pullUpKey:
                pullyServo1Pos += 2;
                pullyServo2Pos += 2;
                pullyServo1.write(pullyServo1Pos);
                pullyServo2.write(pullyServo2Pos);
                break;

            case pullDownKey:
                pullyServo1Pos -= 2;
                pullyServo2Pos -= 2;
                pullyServo1.write(pullyServo1Pos);
                pullyServo2.write(pullyServo2Pos);
                break;

            // Basket
            case basketLeftKey:
                basketServo.write(basketForwardAngle);
                break;

            case basketStopKey:
                basketServo.write(basketStopAngle);
                break;

            case basketRightKey:
                basketServo.write(basketBackwardAngle);
                break;

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
            case stopKey:
                stopDriving();
                break;
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
            case changeDrivingModeKey:
                changeDrivingMode();
                break;
        }

        if (containsChar(movingKeys, command)) {
            isDriving = true;
            lastKeyPress = millis();
        }
    }
}

void changeDrivingMode() {
    if (drivingMode == 1) {
        drivingMode = 2;
    } else {
        drivingMode = 1;
    }
}

void stopDriving() {
    analogWrite(leftPWM1, 0);
    analogWrite(leftPWM2, 0);
    analogWrite(rightPWM1, 0);
    analogWrite(rightPWM2, 0);
}

void moveForward() {
    analogWrite(leftPWM2, 0);
    analogWrite(rightPWM2, 0);
    if (drivingMode == 1) {
        analogWrite(leftPWM1, maxSpeed);
        analogWrite(rightPWM1, maxSpeed);
    } else {
        analogWrite(leftPWM1, slowSpeed);
        analogWrite(rightPWM1, slowSpeed);
    }
}

void moveBackward() {
    analogWrite(leftPWM1, 0);
    analogWrite(rightPWM1, 0);
    if (drivingMode == 1) {
        analogWrite(leftPWM2, maxSpeed);
        analogWrite(rightPWM2, maxSpeed);
    } else {
        analogWrite(leftPWM2, slowSpeed);
        analogWrite(rightPWM2, slowSpeed);
    }
}

void moveLeft() {
    analogWrite(leftPWM2, 0);
    analogWrite(rightPWM2, 0);
    if (drivingMode == 1) {
        analogWrite(leftPWM1, turningMaxSpeed);
        analogWrite(rightPWM1, maxSpeed);
    } else {
        analogWrite(leftPWM1, turningSlowSpeed);
        analogWrite(rightPWM1, slowSpeed);
    }
}

void moveRight() {
    analogWrite(leftPWM2, 0);
    analogWrite(rightPWM2, 0);
    if (drivingMode == 1) {
        analogWrite(leftPWM1, maxSpeed);
        analogWrite(rightPWM1, turningSpeed);
    } else {
        analogWrite(leftPWM1, slowSpeed);
        analogWrite(rightPWM1, turningSlowSpeed);
    }
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

/**
 * Checks whether a command is a movement key
 *
 * @param  haystack Movement keys
 * @param  needle   Command
 * @return          True if the command is a movement key,
 *                  False otherwise
 */
bool containsChar(char haystack[], char needle) {
    for (int i = 0; i < 4; i++) {
        char item = haystack[i];
        if (haystack[i] == needle) {
            return true;
        }
    }
    return false;
}
