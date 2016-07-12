#include <phys253.h>
#include <LiquidCrystal.h>

void setup() {
  //Setup
#include <phys253setup.txt>
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  LCD.clear();
  
  LCD.setCursor(3,0);
  LCD.print(analogRead(2));

  LCD.setCursor(8,0);
  LCD.print(analogRead(0));

  LCD.setCursor(0,1);
  LCD.print(analogRead(6));

  LCD.setCursor(11,1);
  LCD.print(analogRead(4));

  delay(100);

}
