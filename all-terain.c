#define LEFT_MOTOR_RPWM 18
#define LEFT_MOTOR_LPWM 19
#define RIGHT_MOTOR_RPWM 23
#define RIGHT_MOTOR_LPWM 22

// Motor speed (0-255)
int motorSpeed = 200;

void setup() {
  // Initialize motor control pins
  pinMode(LEFT_MOTOR_RPWM, OUTPUT);
  pinMode(LEFT_MOTOR_LPWM, OUTPUT);
  pinMode(RIGHT_MOTOR_RPWM, OUTPUT);
  pinMode(RIGHT_MOTOR_LPWM, OUTPUT);

  Serial.begin(115200);
  Serial.println("All-Terrain Robot Control Initialized");
}

void loop() {
  // Move forward
  moveForward();
  delay(2000);

  // Move backward
  moveBackward();
  delay(2000);

  // Turn left
  turnLeft();
  delay(2000);

  // Turn right
  turnRight();
  delay(2000);

  // Stop the motors
  stopMotors();
  delay(2000);
}

void moveForward() {
  analogWrite(LEFT_MOTOR_RPWM, motorSpeed);
  analogWrite(LEFT_MOTOR_LPWM, 0);
  analogWrite(RIGHT_MOTOR_RPWM, motorSpeed);
  analogWrite(RIGHT_MOTOR_LPWM, 0);
  Serial.println("Moving forward");
}

void moveBackward() {
  analogWrite(LEFT_MOTOR_RPWM, 0);
  analogWrite(LEFT_MOTOR_LPWM, motorSpeed);
  analogWrite(RIGHT_MOTOR_RPWM, 0);
  analogWrite(RIGHT_MOTOR_LPWM, motorSpeed);
  Serial.println("Moving backward");
}

void turnLeft() {
  analogWrite(LEFT_MOTOR_RPWM, 0);
  analogWrite(LEFT_MOTOR_LPWM, motorSpeed);
  analogWrite(RIGHT_MOTOR_RPWM, motorSpeed);
  analogWrite(RIGHT_MOTOR_LPWM, 0);
  Serial.println("Turning left");
}

void turnRight() {
  analogWrite(LEFT_MOTOR_RPWM, motorSpeed);
  analogWrite(LEFT_MOTOR_LPWM, 0);
  analogWrite(RIGHT_MOTOR_RPWM, 0);
  analogWrite(RIGHT_MOTOR_LPWM, motorSpeed);
  Serial.println("Turning right");
}

void stopMotors() {
  analogWrite(LEFT_MOTOR_RPWM, 0);
  analogWrite(LEFT_MOTOR_LPWM, 0);
  analogWrite(RIGHT_MOTOR_RPWM, 0);
  analogWrite(RIGHT_MOTOR_LPWM, 0);
  Serial.println("Stopping motors");
}
