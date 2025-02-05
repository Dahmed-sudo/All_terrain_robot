// Define motor pins (Assuming L298 motor driver)
const int motorLeftForward = 5;
const int motorLeftBackward = 6;
const int motorRightForward = 9;
const int motorRightBackward = 10;

// Define sensor pins (16 sensors)
int sensors[16] = {A0, A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11, A12, A13, A14, A15};

// Threshold for detecting the line
const int threshold = 500; // Adjust based on the sensor's readings (value for line vs. surface)

// Motor control functions
void moveForward() {
  digitalWrite(motorLeftForward, HIGH);
  digitalWrite(motorLeftBackward, LOW);
  digitalWrite(motorRightForward, HIGH);
  digitalWrite(motorRightBackward, LOW);
}

void moveBackward() {
  digitalWrite(motorLeftForward, LOW);
  digitalWrite(motorLeftBackward, HIGH);
  digitalWrite(motorRightForward, LOW);
  digitalWrite(motorRightBackward, HIGH);
}

void turnLeft() {
  digitalWrite(motorLeftForward, LOW);
  digitalWrite(motorLeftBackward, HIGH);
  digitalWrite(motorRightForward, HIGH);
  digitalWrite(motorRightBackward, LOW);
}

void turnRight() {
  digitalWrite(motorLeftForward, HIGH);
  digitalWrite(motorLeftBackward, LOW);
  digitalWrite(motorRightForward, LOW);
  digitalWrite(motorRightBackward, HIGH);
}

void stopMotors() {
  digitalWrite(motorLeftForward, LOW);
  digitalWrite(motorLeftBackward, LOW);
  digitalWrite(motorRightForward, LOW);
  digitalWrite(motorRightBackward, LOW);
}

void setup() {
  // Initialize motor pins
  pinMode(motorLeftForward, OUTPUT);
  pinMode(motorLeftBackward, OUTPUT);
  pinMode(motorRightForward, OUTPUT);
  pinMode(motorRightBackward, OUTPUT);

  // Initialize sensor pins
  for (int i = 0; i < 16; i++) {
    pinMode(sensors[i], INPUT);
  }
}

void loop() {
  int sensorValues[16];
  int leftSensor = 0;
  int rightSensor = 0;
  int centerSensor = 0;

  // Read values from all sensors
  for (int i = 0; i < 16; i++) {
    sensorValues[i] = analogRead(sensors[i]);
  }

  // Identify which sensors are over the line
  for (int i = 0; i < 16; i++) {
    if (sensorValues[i] < threshold) { // Assuming the line reflects less infrared light
      if (i < 8) leftSensor = 1;
      if (i > 7) rightSensor = 1;
      if (i == 7 || i == 8) centerSensor = 1;
    }
  }

  // Line following logic
  if (leftSensor && rightSensor) {
    moveForward();  // Both sides see the line, move forward
  } else if (leftSensor && !rightSensor) {
    turnLeft();  // Left side sees the line, turn left
  } else if (!leftSensor && rightSensor) {
    turnRight();  // Right side sees the line, turn right
  } else {
    stopMotors();  // No sensors see the line, stop
  }
}
