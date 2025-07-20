// ESP32 robot all-terrain control via Serial commands (e.g. from phone app)

#define IN1 14  // Left motor forward
#define IN2 27  // Left motor backward
#define IN3 26  // Right motor forward
#define IN4 25  // Right motor backward

void setup() {
  Serial.begin(9600);  // Bluetooth module or Serial Monitor
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  stopMotors();
  Serial.println("ESP32 Robot Ready. Awaiting command...");
}

void loop() {
  if (Serial.available()) {
    char command = Serial.read();
    Serial.print("Received: "); Serial.println(command);

    switch (command) {
      case 'F':  // Forward
        forward();
        break;
      case 'B':  // Backward
        backward();
        break;
      case 'L':  // Left
        turnLeft();
        break;
      case 'R':  // Right
        turnRight();
        break;
      case 'S':  // Stop
        stopMotors();
        break;
      default:
        stopMotors();
        break;
    }
  }
}

// Motor control functions
void forward() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void backward() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void turnLeft() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void turnRight() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void stopMotors() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}
