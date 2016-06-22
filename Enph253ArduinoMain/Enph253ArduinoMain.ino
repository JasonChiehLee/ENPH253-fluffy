/*
 * TINAH Template Program - UBC Engineering Physics 253
 * (nakane, 2015 Jan 2)  - Updated for use in Arduino IDE (1.0.6)
 */
 
#include <phys253.h>          
#include <LiquidCrystal.h>  
extern "C" {
//Include .h files here if they are in C
}

void setup()
{
  //Setup
    #include <phys253setup.txt>
    Serial.begin(9600);
}

void loop()
{
  //Main code, repeated
  LCD.clear();
  LCD.home();

  LCD.print("Code loaded!");
  
  delay(500);
}

