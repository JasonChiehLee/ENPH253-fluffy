#include <phys253.h>
#include <LiquidCrystal.h>

# define QSD1 0
# define QSD2 2
# define QSD3 1
# define QSD4 3
# define QSD5 4
# define QSD6 6
# define QSD7 5

void setup() {
  //Setup
#include <phys253setup.txt>
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  LCD.clear();
  LCD.home();
  LCD.print(analogRead(QSD1));
  LCD.print(" ");
  LCD.print(analogRead(QSD2));
  LCD.print(" ");
  LCD.print(analogRead(QSD3));
  LCD.print(" ");
  LCD.print(analogRead(QSD4));
  LCD.print(" ");
  LCD.setCursor(0, 1);
  LCD.print(analogRead(QSD5));
  LCD.print(" ");
  LCD.print(analogRead(QSD6));
  LCD.print(" ");
  LCD.print(analogRead(QSD7));

  delay(250);
}
