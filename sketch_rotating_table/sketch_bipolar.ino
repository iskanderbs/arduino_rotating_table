#include <GyverButton.h>
#include <AccelStepper.h>

#define button A0

#define led_1 7
#define led_2 8
#define led_3 9
#define led_4 10

#define motorInterfaceType 1

// DIR и STEP пины
const int dirPin = 3;
const int stepPin = 2;

// Кол-во шагов у двигателя и скорость (шагов/сек)
const int motorSteps = 200;
const int speed = 200;

int i = 0;
uint8_t mode = 1;
bool directionFlag = true;
bool reverseFlag = true;
float moveSteps;

GButton motor_button(button);
AccelStepper motor(motorInterfaceType, stepPin, dirPin);

void setup() {
  pinMode(led_1, OUTPUT);
  pinMode(led_2, OUTPUT);
  pinMode(led_3, OUTPUT);
  pinMode(led_4, OUTPUT);

  motor_button.setType(HIGH_PULL);
  motor_button.setDirection(NORM_OPEN);
  
  motor.setAcceleration(speed / 5);
  motor.setMaxSpeed(speed * 10);
  motor.setSpeed(speed);
}

void loop() {
  // Опрашиваем кнопку
  motor_button.tick();

  // При одинарном нажатии кнопки переключаем режим
  if (motor_button.isSingle()) {
    mode++;
    if (mode > 4) {
      mode = 1;
    }
  }

  // При двойном нажатии кнопки изменяем направление вращения
  // только в первом режиме
  if (motor_button.isDouble() && mode == 1) {
    directionFlag = !directionFlag;
    
    if (directionFlag) {
      motor.setSpeed(speed);
    } else {
      motor.setSpeed(-speed);
    }
  }

  // Зажигаем светодиод текущего режима
  setModeLed(mode);
  
  if (mode == 1) {
    motor.runSpeed();
  } else {
    motor.run();
  }
  
  // В зависимости от режима запускаем соответствующую функцию
  switch (mode) {
    case 1: 
    break;
    case 2: rotateBySteps(motorSteps); // Полный оборот и реверс
    break;
    case 3: rotateBySteps(motorSteps / 2); // Пол-оборота и реверс
    break;
    case 4: rotateBySteps(motorSteps / 4); // Четверть оборота и реверс
    break;
  }
}

void rotateBySteps(float steps) {
  if (motor.isRunning()) {
    return;
  }
  
  motor.setCurrentPosition(0);
  
  if (reverseFlag) {
    motor.moveTo(steps);
    reverseFlag = false;
    return;
  }
  
  motor.moveTo(-steps);
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
