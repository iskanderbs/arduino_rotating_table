// Пины DIR, STEP
#define pinDir 5
#define pinStep 6

void setup() {
  pinMode(pinDir, OUTPUT);
  pinMode(pinStep, OUTPUT);
  setMotorDirection(false);
}

void loop() {
  doMotorStep();
}

void setMotorDirection(bool clockwise) {
  delay(500);
  if (clockwise) {
    digitalWrite(pinDir, LOW);
  } else {
    digitalWrite(pinDir, HIGH);
  }
}

void doMotorStep() {
  digitalWrite(pinStep, HIGH);
  delayMicroseconds(200);
  digitalWrite(pinStep, LOW);
  delayMicroseconds(200);
}
