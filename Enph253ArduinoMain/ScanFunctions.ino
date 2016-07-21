/*
   function - intersectionScan

   Scans at an intersection using the specified analog input, sets the angle for the highest signal.
   If there are no signals above the threshold, sets the angle as 90.
*/
byte intersectionScan()
{

  byte bestAngle = 90;

/*
  int one = analogRead(FRONT_QSD1);
  int two = analogRead(FRONT_QSD2);
  int three = analogRead(FRONT_QSD3);
  int four = analogRead(FRONT_QSD4);
  int five = analogRead(FRONT_QSD5);

  if (one > two && one > three && one > four && one > five)
  {
    bestAngle = ONE_ANGLE;
  }
  else if (two > one && two > three && two > four && two > five)
  {
    bestAngle = TWO_ANGLE;
  }
  else if (three > one && three > two && three > four && three > five)
  {
    bestAngle = THREE_ANGLE;
  }
  else if (four > one && four > two && four > three && four > five)
  {
    bestAngle = FOUR_ANGLE;
  }
  else if (five > one && five > two && five > three && five > four)
  {
    bestAngle = FIVE_ANGLE;
  }
  */
  
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
  int largestSignal = 0;

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

