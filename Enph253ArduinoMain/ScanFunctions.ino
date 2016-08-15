/*
   function - directionScan

   Scans using 3 analog inputs, dependant on the input boolean (true for front, false for top), I know that's a bit hacky, big sorry.
   Sets best angle depending on these readings, does not set if none are above threshold.
*/
void directionalScan(bool front)
{

  byte bestAngle = 90;
  boolean scanSuccess = false;
  int leftQSD = 0, rightQSD = 0, straightQSD = 0, threshold = 1055;

  // read values, depending on front or top input request
  if (front)
  {
    leftQSD = analogReadAvg(FRONT_QSD_LEFT);
    straightQSD = analogReadAvg(FRONT_QSD_STRAIGHT);
    rightQSD = analogReadAvg(FRONT_QSD_RIGHT);
    threshold = QSD_FRONT_THRESHOLD;
  }
  else
  {
    leftQSD = analogReadAvg(TOP_QSD_LEFT);
    straightQSD = analogReadAvg(TOP_QSD_STRAIGHT);
    rightQSD = analogReadAvg(TOP_QSD_RIGHT);
    threshold = QSD_TOP_THRESHOLD;
  }

  // report greatest reading as best angle to go
  if (leftQSD > threshold && leftQSD >= straightQSD && leftQSD >= rightQSD)
  {
    scanSuccess = true;
    bestAngle = LEFT_ANGLE;
  }
  else if (rightQSD > threshold && rightQSD >= straightQSD && rightQSD > leftQSD)
  {
    scanSuccess = true;
    bestAngle = RIGHT_ANGLE;
  }
  else if (straightQSD > threshold && straightQSD > leftQSD && straightQSD > rightQSD)
  {
    scanSuccess = true;
    bestAngle = FORWARD_ANGLE;
  }

  if (scanSuccess)
  {
    turnBiased = true;
    setTravelAngle(bestAngle);
#if SENSOR_DEBUG
    LCD.home();
    LCD.clear();
    LCD.print(bestAngle);
#endif
  }
}


/*
   function - analogReadAvg
*/

int analogReadAvg(byte port)
{
  int retval = 0;

  for (int i = 0; i < 16; i++)
  {
    retval += analogRead(port);
  }

  return retval >> 4;
}

