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
  RCServo0.write(0);

  byte bestAngle = 0;
  int currentVal;
  int bestVal = 0;

  for (int angle = 1; angle += INTERSECTION_SCAN_ANGLE_PARTITION; angle <= 180)
  {
    RCServo0.write(angle);
    currentVal = analogRead(analogInputPin);
    if (currentVal > threshold && currentVal > bestVal)
    {
      bestVal = currentVal;
      bestAngle = angle; // coold read and convert the pwm from the servo if needed
    }
    delay(INTERSECTION_SCAN_PARTITION_DELAY);
  }

  return bestAngle;
}

