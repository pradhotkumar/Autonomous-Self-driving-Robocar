#include <Servo.h>

// -------- PIN DEFINITIONS --------
const int ENA = 5;  const int ENB = 6;
const int IN1 = 7;  const int IN2 = 8;
const int IN3 = 9;  const int IN4 = 4;
const int trigPin = 2; const int echoPin = 3;
const int irLeftPin = A0; const int irRightPin = A1;
const int buzzerPin = 12; const int servoPin = 13;

// NEW: 4 Independent LED Pins
const int whiteLedL = 10; // Front Left Headlight
const int whiteLedR = 11; // Front Right Headlight
const int redLedL = A2;   // Rear Left Taillight
const int redLedR = A3;   // Rear Right Taillight

Servo head;
int distance = 0;

// -------- TIMING VARIABLES --------
unsigned long previousMillis = 0;
bool warningState = false;

void setup() {
  Serial.begin(115200);
  pinMode(ENA, OUTPUT); pinMode(ENB, OUTPUT);
  pinMode(IN1, OUTPUT); pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT); pinMode(IN4, OUTPUT);
  pinMode(trigPin, OUTPUT); pinMode(echoPin, INPUT);
  
  pinMode(irLeftPin, INPUT); 
  pinMode(irRightPin, INPUT);
  pinMode(buzzerPin, OUTPUT); 
  
  // Initialize all 4 LEDs
  pinMode(whiteLedL, OUTPUT); pinMode(whiteLedR, OUTPUT);
  pinMode(redLedL, OUTPUT);   pinMode(redLedR, OUTPUT);

  head.attach(servoPin);
  head.write(90);

  startupFanfare();
}

void loop() {
  distance = getDistance();
  int irL = digitalRead(irLeftPin);
  int irR = digitalRead(irRightPin);

  // -------- 1. INSTANT IR REFLEXES --------
  if (irL == LOW && irR == LOW) {
    emergencyBrake();
    policeStrobe(6); // Flash alternately
    reverse(200); delay(800); 
    turnRight(); delay(800);  
    stopCar();
    return;
  }
  else if (irL == LOW) {
    emergencyBrake();
    tone(buzzerPin, 2000, 200); 
    reverse(200); delay(300); 
    turnRight(); delay(500);  
    stopCar();
    return;
  }
  else if (irR == LOW) {
    emergencyBrake();
    tone(buzzerPin, 2000, 200); 
    reverse(200); delay(300); 
    turnLeft(); delay(500);   
    stopCar();
    return;
  }

  // -------- 2. ULTRASONIC STRICT STOP (< 16cm) --------
  else if (distance < 16) { 
    emergencyBrake();
    smartAvoid(); 
    return; 
  }

  // -------- 3. WARNING ZONE: DYNAMIC BRAKING & FLICKER (16cm to 50cm) --------
  else if (distance >= 16 && distance <= 50) {
    int dynamicSpeed = map(distance, 16, 50, 100, 180);
    forward(dynamicSpeed);

    // Keep headlights on
    digitalWrite(whiteLedL, HIGH); digitalWrite(whiteLedR, HIGH);

    int blinkRate = map(distance, 16, 50, 40, 400); 
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= blinkRate) {
      previousMillis = currentMillis;
      warningState = !warningState; 
      
      // Flicker BOTH rear red lights like a hazard warning
      digitalWrite(redLedL, warningState ? HIGH : LOW);
      digitalWrite(redLedR, warningState ? HIGH : LOW);
      
      if(warningState) tone(buzzerPin, 3000, 15); 
    }
  }
  
  // -------- 4. SAFE ZONE: FULL SPEED (> 50cm) --------
  else {
    forward(220); 
    // Headlights ON, Taillights OFF
    setLights(HIGH, HIGH, LOW, LOW);
  }
}

// -------- LIGHT CONTROL HELPER --------
void setLights(bool wL, bool wR, bool rL, bool rR) {
  digitalWrite(whiteLedL, wL);
  digitalWrite(whiteLedR, wR);
  digitalWrite(redLedL, rL);
  digitalWrite(redLedR, rR);
}

// -------- AUDIO-VISUAL STYLES --------

void startupFanfare() {
  int melody[] = {523, 659, 784, 1046};
  for (int i = 0; i < 4; i++) {
    tone(buzzerPin, melody[i], 100);
    setLights(HIGH, LOW, HIGH, LOW); // Left side on
    delay(100);
    setLights(LOW, HIGH, LOW, HIGH); // Right side on
    delay(100);
  }
  setLights(LOW, LOW, LOW, LOW); // All off
  noTone(buzzerPin);
}

void policeStrobe(int count) {
  for (int i = 0; i < count; i++) {
    tone(buzzerPin, 2500, 50);
    setLights(HIGH, LOW, HIGH, LOW); // Left flash
    delay(60);
    setLights(LOW, HIGH, LOW, HIGH); // Right flash
    delay(60);
  }
  setLights(LOW, LOW, LOW, LOW);
}

// -------- STRICT MOVEMENT LOGIC --------

void emergencyBrake() {
  // TRUE BRAKE LIGHTS: Red ON, White OFF
  setLights(LOW, LOW, HIGH, HIGH);
  
  reverse(255); 
  delay(100); 
  stopCar();
}

void smartAvoid() {
  delay(100); 
  
  int leftDist = scanLeft();
  int rightDist = scanRight();
  head.write(90); 
  delay(200);

  if (leftDist > rightDist && leftDist > 30) {
    turnLeft(); delay(600);
  } 
  else if (rightDist > 30) {
    turnRight(); delay(600);
  } 
  else {
    policeStrobe(8);
    // Backing up lights
    setLights(LOW, LOW, HIGH, HIGH);
    reverse(200); delay(800);
    turnRight(); delay(800); 
  }
  stopCar();
}

// -------- SENSORS & MOTORS --------

int getDistance() {
  digitalWrite(trigPin, LOW); delayMicroseconds(2);
  digitalWrite(trigPin, HIGH); delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  long duration = pulseIn(echoPin, HIGH, 15000); 
  if (duration == 0) return 300;
  return duration * 0.034 / 2;
}

int scanLeft() { head.write(160); delay(300); return getDistance(); }
int scanRight() { head.write(20); delay(300); return getDistance(); }

void forward(int s) {
  digitalWrite(IN1, LOW); digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);
  analogWrite(ENA, s); analogWrite(ENB, s);
}

void reverse(int s) {
  digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW); digitalWrite(IN4, HIGH);
  analogWrite(ENA, s); analogWrite(ENB, s);
}

void turnLeft() {
  digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);
  analogWrite(ENA, 220); analogWrite(ENB, 220);
  // TURN SIGNAL: Left lights ON
  setLights(HIGH, LOW, HIGH, LOW);
}

void turnRight() {
  digitalWrite(IN1, LOW); digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW); digitalWrite(IN4, HIGH);
  analogWrite(ENA, 220); analogWrite(ENB, 220);
  // TURN SIGNAL: Right lights ON
  setLights(LOW, HIGH, LOW, HIGH);
}

void stopCar() { analogWrite(ENA, 0); analogWrite(ENB, 0); }
