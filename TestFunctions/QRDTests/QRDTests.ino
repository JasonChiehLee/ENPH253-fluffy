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

  // Left tape
  LCD.setCursor(3, 0);
  LCD.print(digitalRead(9));

  //Right tape
  LCD.setCursor(8, 0);
  LCD.print(digitalRead(12));

  // Left Int
  LCD.setCursor(0, 1);
  LCD.print(digitalRead(10));

  //Right Int
  LCD.setCursor(11, 1);
  LCD.print(digitalRead(11));

  delay(100);

}
