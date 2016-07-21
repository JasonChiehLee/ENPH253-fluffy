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
  LCD.print(digitalRead(12));

  LCD.setCursor(8,0);
  LCD.print(digitalRead(13));

  LCD.setCursor(0,1);
  LCD.print(digitalRead(11));

  LCD.setCursor(11,1);
  LCD.print(digitalRead(10));

  delay(100);

}
