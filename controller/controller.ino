
#include <Servo.h>

// Basket
Servo basketServo;
int basketServoPin = A4;

// Pullies
Servo pullyServo1;
Servo pullyServo2;
int pullyServo1Pin = A2;
int pullyServo2Pin = A3;
int pullyServo1Pos = 90;
int pullyServo2Pos = 90;

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
bool isDriving = false;

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
unsigned long keyDelay = 300;
unsigned long lastKeyPress = 0;

// LED
int ledPin = 13;

/**
 * Initialises program. Ran only once at startup
 */
void setup() {

    pinMode(ledPin, OUTPUT);

    // Setup gimbal system
    gimbalServoX.attach(gimbalServoPinX);
    gimbalServoY.attach(gimbalServoPinY);
    resetgimbalPos();

    // Setup movement system (PWM)
    analogWrite(leftPWM1, 0);
    analogWrite(leftPWM2, 0);
    analogWrite(rightPWM1, 0);
    analogWrite(rightPWM2, 0);

    // Pully system
    pullyServo1.attach(pullyServo1Pin);
    pullyServo2.attach(pullyServo2Pin);
    pullyServo1.write(pullyServo1Pos);
    pullyServo2.write(pullyServo2Pos);

    // Basket
    basketServo.attach(basketServoPin);

    // Setup serial
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

    if (isDriving && (lastKeyPress + keyDelay <= millis())) {
        Serial.println("test4");
        isDriving = false;
        stopMovement();
    }

    // Check if there are any commands to process
    if (Serial1.available() > 0) {
        char command = Serial1.read();

        // Execute action according to which key was pressed
        switch (command) {
            // Pully
            case 'm':
                pullyServo1Pos += 2;
                pullyServo2Pos += 2;
                pullyServo1.write(pullyServo1Pos);
                pullyServo2.write(pullyServo2Pos);
                break;

            case 'n':
                pullyServo1Pos -= 2;
                pullyServo2Pos -= 2;
                pullyServo1.write(pullyServo1Pos);
                pullyServo2.write(pullyServo2Pos);
                break;

            case 'z':
                basketServo.write(100);
                break;

            case 'x':
                basketServo.write(90);
                break;

            case 'c':
                basketServo.write(80);
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
            case 'q':
                stopMovement();
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
                Serial.println("test 1");
                moveRight();
                Serial.println("Test 2");
                break;
        }

        Serial.println("test: " + containsChar(movingKeys, command));

        if (containsChar(movingKeys, command)) {
            Serial.println("test5");
            isDriving = true;
            lastKeyPress = millis();
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
    analogWrite(leftPWM1, turningSpeed);
    analogWrite(leftPWM2, 0);
    analogWrite(rightPWM1, maxSpeed);
    analogWrite(rightPWM2, 0);
}

void moveRight() {
    analogWrite(leftPWM1, maxSpeed);
    analogWrite(leftPWM2, 0);
    analogWrite(rightPWM1, turningSpeed);
    analogWrite(rightPWM2, 0);
    Serial.write("test 3");
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
    for (int i = 0; i < 4; i++) {
        char item = haystack[i];
        if (haystack[i] == needle) {
            return true;
        }
    }
    return false;
}
