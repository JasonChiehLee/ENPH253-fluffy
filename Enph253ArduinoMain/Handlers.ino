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

  if (leftTurn)
  {
    LCD.clear();
    LCD.home();
    LCD.print("LEFT");
    nextDirection = LEFT;
  }
  else if (straightThrough)
  {
    LCD.clear();
    LCD.home();
    LCD.print("FORWARD");
    nextDirection = FORWARD;
  }
  else if (rightTurn)
  {
    LCD.clear();
    LCD.home();
    LCD.print("RIGHT");
    nextDirection = RIGHT;
  }
  else
  {
    LCD.clear();
    LCD.home();
    LCD.print("FORWARD");
    nextDirection = FORWARD; // should never hit this one
  }

  // Move to that direction
  intersectionTurn(nextDirection);
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
