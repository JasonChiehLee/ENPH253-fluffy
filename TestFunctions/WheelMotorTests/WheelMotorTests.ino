#include <phys253.h>
#include <LiquidCrystal.h>

void setup() {
  //Setup
#include <phys253setup.txt>
  Serial.begin(9600);
}

#define LEFT_MOTOR 0
#define RIGHT_MOTOR 1

int leftMotorSpeed;
int rightMotorSpeed;

void loop() {
  LCD.clear();
  LCD.home();

  leftMotorSpeed = knob(6)-512;
  rightMotorSpeed = knob(7)-512;

  motor.speed(LEFT_MOTOR,leftMotorSpeed);
  motor.speed(RIGHT_MOTOR,rightMotorSpeed);

  LCD.print(leftMotorSpeed);
  LCD.print(" ");
  LCD.print(rightMotorSpeed);

  delay(250);

}
