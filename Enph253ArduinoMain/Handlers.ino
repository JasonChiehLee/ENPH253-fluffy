/*
   function - intersectionHandler

   Checks which directions are available while halting, and calls necessary function to compute optimal direction to turn.
   Proceeds to call
*/
void intersectionHandler()
{
  boolean rightTurn = false, leftTurn = false, straightThrough = false;

  for (int t = 0; t < HARD_STOP_WAIT_TIME; t += HARD_STOP_WAIT_TIME >> 4)
  {
    ((analogRead(INTERSECTION_QRD_RIGHT) > QRD_GROUND_THRESHOLD) || rightTurn) ? rightTurn = true : rightTurn = false;
    ((analogRead(INTERSECTION_QRD_LEFT) > QRD_GROUND_THRESHOLD) || leftTurn) ? leftTurn = true : leftTurn = false;
    delay(HARD_STOP_WAIT_TIME >> 4);
  }

  // Halt the bot
  hardStop();

  // Let the stop happen, and check which directions we can turn
  for (int t = 0; t < HARD_STOP_WAIT_TIME; t += HARD_STOP_WAIT_TIME >> 4)
  {
    ((analogRead(INTERSECTION_QRD_RIGHT) > QRD_GROUND_THRESHOLD) || rightTurn) ? rightTurn = true : rightTurn = false;
    ((analogRead(INTERSECTION_QRD_LEFT) > QRD_GROUND_THRESHOLD) || leftTurn) ? leftTurn = true : leftTurn = false;
    delay(HARD_STOP_WAIT_TIME >> 4);
  }

  (analogRead(TAPE_FOLLOWING_QRD_RIGHT) > QRD_GROUND_THRESHOLD || analogRead(TAPE_FOLLOWING_QRD_LEFT) > QRD_GROUND_THRESHOLD) ? straightThrough = true : straightThrough = false;

  // Compute optimal direction
  // Temporarily prioritizing left-straight-right until sensors are online
  direction_e nextDirection = FORWARD;

  nextDirection = determineDirection(rightTurn, leftTurn, straightThrough);
  if (nextDirection == LEFT)
  {
    LCD.clear();
    LCD.home();
    LCD.print("LEFT");
  }
  else if (nextDirection == FORWARD)
  {
    LCD.clear();
    LCD.home();
    LCD.print("FORWARD");

  }
  else if (nextDirection == RIGHT)
  {
    LCD.clear();
    LCD.home();
    LCD.print("RIGHT");
  }
  else
  {
    LCD.clear();
    LCD.home();
    LCD.print("KILL YOURSELF");
  }

  // Move to that direction
  intersectionTurn(nextDirection);
}

/*
   function - determineDirection

   Determines direction to turn depending on available paths.
*/
direction_e determineDirection(boolean rightTurn, boolean leftTurn, boolean straightThrough)
{
  direction_e travelDirection = FORWARD;
  if (travelAngle < THRESHOLDANGLE1)
  {
    if (leftTurn == true)
    {
      travelDirection = LEFT;
    }
    else if (straightThrough = true)
    {
      travelDirection = FORWARD;
    }
    else if (rightTurn == true)
    {
      travelDirection = RIGHT;
    }
    else
    {
      xPointTurn(5);
    }
  }
  else if (travelAngle > THRESHOLDANGLE2)
  {
    if (rightTurn == true)
    {
      travelDirection = RIGHT;
    }
    else if (straightThrough = true)
    {
      travelDirection = FORWARD;
    }
    else if (leftTurn == true)
    {
      travelDirection = LEFT;
    }
    else
    {
      xPointTurn(5);
    }
  }
  else if (travelAngle > THRESHOLDANGLE1 && travelAngle < THRESHOLDANGLE2)
  {
    if (straightThrough == true)
    {
      travelDirection = FORWARD;
    }
    else if (rightTurn = true)
    {
      travelDirection = RIGHT;
    }
    else if (leftTurn == true)
    {
      travelDirection = LEFT;
    }
    else
    {
      xPointTurn(5);
    }
  }
  return travelDirection;
}

/*
   function - dollHandler

   Carries out aqcuisition actions when called
*/
void dollHandler(direction_e dollSide, armPosition_t pickUp, armPosition_t dropOff)
{
  centreAlign(dollSide);
  passengerAquire(pickUp, dropOff);
  setLoadStatus(TRUE);
}
