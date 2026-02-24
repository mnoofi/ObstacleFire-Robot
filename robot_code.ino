#include <Servo.h>

// ================= FLAME SENSORS =================
#define FLAME_LEFT   A0
#define FLAME_CENTER A1
#define FLAME_RIGHT  A2
#define FIRE_THRESHOLD 900

// ================= ULTRASONIC =================
#define TRIG_PIN A3
#define ECHO_PIN A4
#define OBSTACLE_DISTANCE 20

// ================= L298N =================
#define IN1 5
#define IN2 6
#define IN3 9
#define IN4 10

// ================= SPEED =================
#define MOVE_ON   50
#define MOVE_OFF  90

// ================= WATER PUMP =================
#define RELAY_PIN 7
#define PUMP_ON   LOW
#define PUMP_OFF  HIGH

// ================= SERVO =================
#define SERVO_PIN 3
Servo hoseServo;

// ================= FLAME SMOOTH =================
int readFlameSmooth(int pin) {
  int sum = 0;
  for (int i = 0; i < 3; i++) {
    sum += analogRead(pin);
    delay(2);
  }
  return sum / 3;
}

// ================= SERVO SHAKE =================
void shakeServo(int centerPos) {
  hoseServo.write(centerPos - 10);
  delay(120);
  hoseServo.write(centerPos + 10);
  delay(120);
}

// ================= SETUP =================
void setup() {
  Serial.begin(9600);

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, PUMP_OFF);

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  hoseServo.attach(SERVO_PIN);
  hoseServo.write(90);
}

// ================= LOOP =================
void loop() {

  int flameL = readFlameSmooth(FLAME_LEFT);
  int flameC = readFlameSmooth(FLAME_CENTER);
  int flameR = readFlameSmooth(FLAME_RIGHT);

  bool leftFire   = flameL < FIRE_THRESHOLD;
  bool centerFire = flameC < FIRE_THRESHOLD;
  bool rightFire  = flameR < FIRE_THRESHOLD;

  // ================= FIRE MODE =================
  if (leftFire || centerFire || rightFire) {

    stopMotors();
    digitalWrite(RELAY_PIN, PUMP_ON);

    // 🔥 نار يمين
    if (rightFire) {
      hoseServo.write(40);        // يمين صح
      shakeServo(40);             // هزّة حوالين اليمين
    }
    // 🔥 نار شمال
    else if (leftFire) {
      hoseServo.write(140);       // شمال صح
      shakeServo(140);            // هزّة حوالين الشمال
    }
    // 🔥 نار في النص
    else {
      hoseServo.write(90);
      shakeServo(90);
    }

    return;
  }

  // ================= NO FIRE =================
  digitalWrite(RELAY_PIN, PUMP_OFF);
  hoseServo.write(90);

  if (getDistance() < OBSTACLE_DISTANCE) {
    avoidObstacle();
  } else {
    moveForwardSlow();
  }
}

// ================= ULTRASONIC =================
int getDistance() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  long duration = pulseIn(ECHO_PIN, HIGH, 30000);
  if (duration == 0) return 100;
  return duration * 0.034 / 2;
}

// ================= MOVEMENT =================
void moveForwardSlow() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  delay(MOVE_ON);

  stopMotors();
  delay(MOVE_OFF);
}

void stopMotors() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

// ================= OBSTACLE AVOID =================
void avoidObstacle() {
  stopMotors();
  delay(200);

  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  delay(300);

  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  delay(400);

  stopMotors();
}