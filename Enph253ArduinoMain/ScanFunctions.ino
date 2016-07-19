# define INTERSECTION_SCAN_ANGLE_PARTITION 10
# define INTERSECTION_SCAN_PARTITION_DELAY 50
/*
   function - intersectionScan

   Scans at an intersection using the specified analog input, if there are signals above the threshold, returns the angle for the highest signal.
   If there are no signals above the threshold, returns 0.

   Assumes the scanning servo is connected to port 0
*/
byte intersectionScan()
{
  byte bestAngle = 90;
  setTravelAngle(bestAngle);
}

/*
   function - dropOffScan

   Scans at an intersection using the specified analog input, returns the corresponding angle with the largest signal.

   Assumes the scanning servo is connected to port X
*/
void dropOffScan()
{
  byte angle = 0;
  byte varAngle = 0;
  byte largestSignal = 0;

  RCServo0.write(varAngle);

  while (varAngle <= 180)
  {
    RCServo0.write(varAngle);
    if (analogRead(TOP_QSD) > largestSignal)
    {
      largestSignal = analogRead(TOP_QSD);
      angle = varAngle;
    }
    varAngle = varAngle + 20;
  }
  setTravelAngle(varAngle);
}

