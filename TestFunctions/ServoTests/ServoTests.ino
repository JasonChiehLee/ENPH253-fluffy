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
    intersectionScan(0,950);
  }
  while (stopbutton())
  {
    RCServo0.write(0);
  }
}

# define INTERSECTION_SCAN_ANGLE_PARTITION 10
# define INTERSECTION_SCAN_PARTITION_DELAY 50
/*
 * function - intersectionScan
 * 
 * Scans at an intersection using the specified analog input, if there are signals above the threshold, returns the angle for the highest signal.
 * If there are no signals above the threshold, returns 0.
 * 
 * Assumes the scanning servo is connected to port 0
 */
byte intersectionScan(byte analogInputPin, int threshold)
{
  LCD.clear();
  LCD.home();
  RCServo0.write(0);

  byte bestAngle = 0;
  int currentVal;
  int bestVal = 0;

  for (int angle = 1; angle < 180; angle += INTERSECTION_SCAN_ANGLE_PARTITION)
  {
    RCServo0.write(angle);
    currentVal = analogRead(analogInputPin);
    if (currentVal > threshold && currentVal > bestVal)
    {
      bestVal = currentVal;
      bestAngle = angle; // coold read and convert the pwm from the servo if needed
    }
    delay(INTERSECTION_SCAN_PARTITION_DELAY);
    LCD.clear();
    LCD.home();
    LCD.print(RCServo0.read());
  }
  return bestAngle;
}

