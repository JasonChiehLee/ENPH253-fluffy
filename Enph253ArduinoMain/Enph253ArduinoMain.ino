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

void setup()
{
  //Setup
#include <phys253setup.txt>
  Serial.begin(9600);
}

void loop()
{
  //Main code, repeated
  //LCD.clear();
  //LCD.home();

  //passengerAquire(rightPickUp, rightDropOff);
  //delay(1000000);
 
  tapeFollow();
 
}
