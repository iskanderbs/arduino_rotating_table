#include <GyverButton.h>
#include <GyverStepper2.h>

#define button A0
#define potentiometer A2

#define led_1 7
#define led_2 8
#define led_3 9
#define led_4 10

// DIR и STEP пины
const int dirPin = 3;
const int stepPin = 2;

// Кол-во шагов у двигателя и множитель потенциомера
const int motorSteps = 400;
const float speedMultiplier = 15.5;

int i = 0;
uint8_t mode = 1;
int32_t zeroPoint = 0;
int potentiometer_value;

bool directionFlag;
bool reverseFlag = true;

GButton motor_button(button);
GStepper2<STEPPER2WIRE> motor(motorSteps, stepPin, dirPin); 

void setup() {
  pinMode(potentiometer, INPUT);
  pinMode(led_1, OUTPUT);
  pinMode(led_2, OUTPUT);
  pinMode(led_3, OUTPUT);
  pinMode(led_4, OUTPUT);

  motor_button.setType(HIGH_PULL);
  motor_button.setDirection(NORM_OPEN);
  
  motor.setAcceleration(50);
}

void loop() {
  // Опрашиваем кнопку и запускаем мотор
  motor_button.tick();
  motor.tick();
  
  // Считываем значение регулятора и задаем скорость вращения мотора
  potentiometer_value = analogRead(potentiometer);
  potentiometer_value = map(potentiometer_value, 1000, 0, 3, 12);
  if (mode == 1) {
    motor.setSpeed(potentiometer_value * speedMultiplier);
  } else {
    motor.setMaxSpeed(potentiometer_value * speedMultiplier);
  }
  
  // При одинарном нажатии кнопки переключаем режим
  if (motor_button.isSingle()) {
    mode++;
    if (mode > 4) {
      mode = 1;
    }
    
    if (mode == 1) {
      motor.setSpeed(potentiometer_value * speedMultiplier);
    } else {
      motor.brake();
      motor.reset();
      motor.setTarget(zeroPoint);
    }
  }

  // При двойном нажатии кнопки изменяем направление вращения
  // только в первом режиме
  if (motor_button.isDouble() && mode == 1) {
    directionFlag = !directionFlag;
    motor.reverse(directionFlag);
  }

  // Зажигаем светодиод текущего режима
  setModeLed(mode);
  
  // В зависимости от режима запускаем соответствующую функцию
  switch (mode) {
    case 1: 
    break;
    case 2: rotateByAngle(360); // Полный оборот и реверс
    break;
    case 3: rotateByAngle(180); // Пол-оборота и реверс
    break;
    case 4: rotateByAngle(90); // Четверть оборота и реверс
    break;
  }
}

void rotateByAngle(float angle) {
  if (!motor.ready()) {
    return;
  }
  
  if (reverseFlag) {
    motor.setTargetDeg(angle);
    reverseFlag = false;
    return;
  }
  
  motor.setTargetDeg(-angle);
  reverseFlag = true;
}

void setModeLed(int ledNum) {
  switch (ledNum) {
    case 1: digitalWrite(led_1, HIGH);
    digitalWrite(led_2, LOW);
    digitalWrite(led_3, LOW);
    digitalWrite(led_4, LOW);
    break;
    case 2: digitalWrite(led_2, HIGH);
    digitalWrite(led_1, LOW);
    digitalWrite(led_3, LOW);
    digitalWrite(led_4, LOW);
    break;
    case 3: digitalWrite(led_3, HIGH);
    digitalWrite(led_1, LOW);
    digitalWrite(led_2, LOW);
    digitalWrite(led_4, LOW);
    break;
    case 4: digitalWrite(led_4, HIGH);
    digitalWrite(led_1, LOW);
    digitalWrite(led_2, LOW);
    digitalWrite(led_3, LOW);
    break;
  }
}