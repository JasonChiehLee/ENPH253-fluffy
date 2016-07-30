/*
   TINAH Template Program - UBC Engineering Physics 253
   (nakane, 2015 Jan 2)  - Updated for use in Arduino IDE (1.0.6)
   #define FRONT_RIGHT_GROUND_SWITCH 7
  #define FRONT_LEFT_GROUND_SWITCH 6
  #define FRONT_RIGHT_BUMPER_SWITCH 9
  #define FRONT_LEFT_BUMPER_SWITCH 8
  #define BACK_RIGHT_BUMPER_SWITCH 5
  #define BACK_LEFT_BUMPER_SWITCH 4
*/

#include <phys253.h>
#include <LiquidCrystal.h>
#include "Group2Setup.h"

# define DEBUG 1

void setup()
{
  //Setup
#include <phys253setup.txt>
  Serial.begin(9600);
}

void loop()
{
  //Main code, repeated
#if DEBUG
  while (!startbutton())
  {
    LCD.home();
    LCD.clear();
    LCD.print(digitalRead(CLAW_CLOSE_SWITCH));
    LCD.print(" ");
    LCD.print(digitalRead(CLAW_OPEN_SWITCH));
    LCD.print(" ");
    LCD.print(digitalRead(DOLL_SWITCH));
    RCServo0.write(reset.baseRotationAngle);
    delay(SERVO_WAIT_TIME);
    RCServo1.write(reset.backPositionAngle);
    RCServo2.write(reset.frontPositionAngle);
  }
  passengerAquire(rightPickUp);
  //passengerAquire(leftPickUp);
  /*
    LCD.home();
    LCD.clear();

    LCD.print(digitalRead(FRONT_LEFT_BUMPER_SWITCH));
    LCD.print(" ");
    LCD.print(digitalRead(FRONT_RIGHT_BUMPER_SWITCH));
    LCD.print(" ");
    LCD.print(digitalRead(FRONT_LEFT_GROUND_SWITCH));
    LCD.print(" ");
    LCD.print(digitalRead(FRONT_RIGHT_GROUND_SWITCH));
    LCD.print(" ");
    LCD.print(digitalRead(BACK_LEFT_BUMPER_SWITCH));
    LCD.print(" ");
    LCD.print(digitalRead(BACK_RIGHT_BUMPER_SWITCH));
    delay(100);
  */
#else

  tapeFollow();
#endif
}
