/*
   TINAH Template Program - UBC Engineering Physics 253
   (nakane, 2015 Jan 2)  - Updated for use in Arduino IDE (1.0.6)
*/

#include <phys253.h>
#include <LiquidCrystal.h>
extern "C" {
  //Include .h files here if they are in C
}

//DIGITAL INPUTS
#define TEST_STOP_BUTTON 0 // FOR TESTING

//ANALOG INPUTS
#define TAPE_FOLLOWING_QRD_RIGHT  0
#define TAPE_FOLLOWING_QRD_LEFT 2

//MOTOR OUTPUTS
#define MOTOR_RIGHT_WHEEL 0
#define MOTOR_LEFT_WHEEL 3

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

  followLine();
}

