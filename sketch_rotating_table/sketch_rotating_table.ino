#include <CustomStepper.h>
#include <GyverButton.h>

#define button A0
#define potentiometer A2

#define led_1 7
#define led_2 8
#define led_3 9
#define led_4 10

GButton motor_button(button);
CustomStepper motor(2, 3, 4, 5);

uint8_t mode = 1;
bool reverse_flag;
bool direction_flag;
int potentiometer_value;

void setup() {
  pinMode(potentiometer, INPUT);
  pinMode(led_1, OUTPUT);
  pinMode(led_2, OUTPUT);
  pinMode(led_3, OUTPUT);
  pinMode(led_4, OUTPUT);

  motor_button.setDebounce(50);
  motor_button.setTimeout(300);
  motor_button.setClickTimeout(600);
  motor_button.setType(HIGH_PULL);
  motor_button.setDirection(NORM_OPEN);

  motor.setRPM(5);
  motor.setSPR(4075.7728395);
}

void loop() {
  // Опрашиваем кнопку и запускаем мотор
  motor_button.tick();
  motor.run();

  // Считываем значение регулятора и задаем скорость вращения мотора
  potentiometer_value = analogRead(potentiometer);
  potentiometer_value = map(potentiometer_value, 1000, 0, 3, 12);
  motor.setRPM(potentiometer_value);

  // При одинарном нажатии кнопки переключаем режим
  if (motor_button.isSingle()) {
    mode++;
    if (mode > 4) {
      mode = 1;
    }
  }

  // При двойном нажатии кнопки изменяем направление вращения
  if (motor_button.isDouble()) {
    direction_flag = !direction_flag;
  }

  // В зависимости от режима запускаем соответствующую функцию
  switch (mode) {
    case 1: rotating();
    break;
    case 2: rotateAtAngle(360);
    break;
    case 3: rotateAtAngle(180);
    break;
    case 4: rotateAtAngle(90);
    break;
  }
}

// Функция постоянного вращения
void rotating() {
  if (!motor.isDone()) {
    return;
  }

  if (direction_flag) {
    motor.setDirection(CW);
  } else {
    motor.setDirection(CCW);
  }

  motor.rotate(1);
}

// Функция вращения на заданный угол
void rotateAtAngle(float angle) {
  if (!motor.isDone()) {
    return;
  }

  if (reverse_flag) {
    motor.setDirection(CW);
    motor.rotateDegrees(angle);
    reverse_flag = false;
    return;
  }

  motor.setDirection(CCW);
  motor.rotateDegrees(angle);
  reverse_flag = true;
}
