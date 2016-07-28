/*
   function - intersectionHandler

   Checks which directions are available while halting, and calls necessary function to compute optimal direction to turn.
   Proceeds to call
*/
void intersectionHandler()
{
  boolean rightTurn = false, leftTurn = false, straightThrough = false;

  // run past the intersection, checking for left and right turn availability
  for (int t = 0; t < HARD_STOP_WAIT_TIME; t += HARD_STOP_WAIT_TIME >> 4)
  {
    ((digitalRead(INTERSECTION_QRD_RIGHT) == ON) || rightTurn) ? rightTurn = true : rightTurn = false;
    ((digitalRead(INTERSECTION_QRD_LEFT) == ON) || leftTurn) ? leftTurn = true : leftTurn = false;
    delay(HARD_STOP_WAIT_TIME >> 4);
  }

  // Halt the bot
  hardStop();

  // Let the stop happen, and check which directions we can turn
  for (int t = 0; t < HARD_STOP_WAIT_TIME; t += HARD_STOP_WAIT_TIME >> 4)
  {
    ((digitalRead(INTERSECTION_QRD_RIGHT) == ON) || rightTurn) ? rightTurn = true : rightTurn = false;
    ((digitalRead(INTERSECTION_QRD_LEFT) == ON) || leftTurn) ? leftTurn = true : leftTurn = false;
    delay(HARD_STOP_WAIT_TIME >> 4);
  }

  (digitalRead(TAPE_FOLLOWING_QRD_RIGHT) == ON || digitalRead(TAPE_FOLLOWING_QRD_LEFT) == ON) ? straightThrough = true : straightThrough = false;

  // If we can't move forward, wiggle a bit to make sure we can't move forward (for curved intersections)
  if (!straightThrough && (leftTurn == rightTurn))
  {
    // go left
    motor.speed(MOTOR_RIGHT_WHEEL, INTERSECTION_TURN_SPEED >> 1);
    motor.speed(MOTOR_LEFT_WHEEL, -INTERSECTION_TURN_SPEED >> 1);
    for (int t = 0; t < INTERSECTION_WIGGLE_TIME; t += INTERSECTION_WIGGLE_TIME >> 4)
    {
      if (digitalRead(TAPE_FOLLOWING_QRD_RIGHT) == ON || digitalRead(TAPE_FOLLOWING_QRD_LEFT) == ON)
      {
        straightThrough = true;
        break;
      }
      delay(INTERSECTION_WIGGLE_TIME >> 4);
    }
    hardStop();
  }
  
  if (!straightThrough && (leftTurn == rightTurn))
  {
    // go right
    motor.speed(MOTOR_RIGHT_WHEEL, -INTERSECTION_TURN_SPEED >> 1);
    motor.speed(MOTOR_LEFT_WHEEL, INTERSECTION_TURN_SPEED >> 1);
    for (int t = 0; t < INTERSECTION_WIGGLE_TIME * 2; t += INTERSECTION_WIGGLE_TIME >> 4)
    {
      if (digitalRead(TAPE_FOLLOWING_QRD_RIGHT) == ON || digitalRead(TAPE_FOLLOWING_QRD_LEFT) == ON)
      {
        straightThrough = true;
        break;
      }
      delay(INTERSECTION_WIGGLE_TIME >> 4);
    }
    hardStop();
  }

  // Compute optimal direction
  // Temporarily prioritizing left-straight-right until sensors are online
  direction_e nextDirection = FORWARD;

  LCD.clear();
  LCD.home();
  LCD.print(leftTurn);
  LCD.print(" ");
  LCD.print(straightThrough);
  LCD.print(" ");
  LCD.print(rightTurn);

  nextDirection = determineDirection(rightTurn, leftTurn, straightThrough);

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
    LCD.print(" leftpref");
    LCD.setCursor(0, 1);
    LCD.print(travelAngle);
    delay(INTERSECTION_WAIT_TIME);
    if (leftTurn == true)
    {
      travelDirection = LEFT;
    }
    else if (straightThrough == true)
    {
      travelDirection = FORWARD;
    }
    else if (rightTurn == true)
    {
      travelDirection = RIGHT;
    }
    else
    {
      delay(INTERSECTION_WAIT_TIME);
      xPointTurn();
    }
  }
  else if (travelAngle > THRESHOLDANGLE2)
  {
    LCD.print(" rightpref");
    LCD.setCursor(0, 1);
    LCD.print(travelAngle);
    delay(INTERSECTION_WAIT_TIME);

    if (rightTurn == true)
    {
      travelDirection = RIGHT;
    }
    else if (straightThrough == true)
    {
      travelDirection = FORWARD;
    }
    else if (leftTurn == true)
    {
      travelDirection = LEFT;
    }
    else
    {
      delay(INTERSECTION_WAIT_TIME);
      xPointTurn();
    }
  }
  else if (travelAngle > THRESHOLDANGLE1 && travelAngle < THRESHOLDANGLE2)
  {
    LCD.print(" straightpref");
    LCD.setCursor(0, 1);
    LCD.print(travelAngle);
    delay(INTERSECTION_WAIT_TIME);

    if (straightThrough == true)
    {
      travelDirection = FORWARD;
    }
    else if (rightTurn == true)
    {
      travelDirection = RIGHT;
    }
    else if (leftTurn == true)
    {
      travelDirection = LEFT;
    }
    else
    {
      delay(INTERSECTION_WAIT_TIME);
      xPointTurn();
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
  hardStop();
  //centreAlign(dollSide);
  LCD.clear();
  LCD.home();
  LCD.print("DOLL");
  delay(DOLL_PICKUP_WAIT);
  if (dollSide == RIGHT)
  {
    LCD.print(" right side");
    delay(1000);
  }
  else if (dollSide == LEFT)
  {
    LCD.print(" left side");
    delay(1000);
  }
  //passengerAquire(pickUp, dropOff);
  //setLoadStatus(TRUE);
  setTravelAngle(90);
}
