#include <phys253.h>
#include <LiquidCrystal.h>
void setup() {
  // put your setup code here, to run once:
#include <phys253setup.txt>
  Serial.begin(9600);

}

void loop() {
  LCD.clear();
  LCD.home();
  LCD.print(map(knob(6), 0, 1024, 0, 180));
  LCD.print(" ");
  LCD.print(map(knob(7), 0, 1024, 0, 180));
  delay(5);
  if (startbutton())
  {
    RCServo0.write(5);
    RCServo1.write(map(knob(6), 0, 1024, 0, 180));
    RCServo2.write(map(knob(7), 0, 1024, 0, 180));
  }
}
