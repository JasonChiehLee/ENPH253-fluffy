#include <phys253.h>
#include <LiquidCrystal.h>

int testVal;
int target;

void setup() {
  //Setup
#include <phys253setup.txt>
  Serial.begin(9600);
}
void loop() {
  while (startbutton())
  {
    /*
      for (int i = 0; i < 180; i++)
      {
      RCServo0.write(i);
      if (RCServo0.read() > 500 && RCServo0.read() <= 1000)
      {
        testVal = RCServo0.read();
        target = i;
      }
      delay(7);
      }
    */
    RCServo0.write(0);
    delay(500);
    RCServo0.write(180);
    if (RCServo0.read() >= 1345)
    {
      RCServo0.write(90);
    }
    LCD.clear();
    LCD.home();
    LCD.print(target);
    LCD.print(" ");
    LCD.print(testVal);
    LCD.print(" ");
    LCD.print(RCServo0.read());
    LCD.print(" ");
    LCD.print(map(RCServo0.read(), 450, 1350, 0, 180));
  }
  while (stopbutton())
  {
    RCServo0.write(0);
  }
}
