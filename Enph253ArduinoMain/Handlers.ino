/*
   function - intersectionHandler

   Checks which directions are available while halting, and calls necessary function to compute optimal direction to turn.
   Proceeds to call
*/
void intersectionHandler()
{
  boolean rightTurn = false, leftTurn = false, straightThrough = false;
  /*
  // stop the bot
  hardStop();
  delay(1000);//HARD_STOP_WAIT_TIME);
  
  // keep going through intersection using PID, checking for turn availability
  // @TODO: how do we look at T intersections?
  while (digitalRead(INTERSECTION_QRD_RIGHT) || digitalRead(INTERSECTION_QRD_LEFT))
  {
    tapeFollow(false, TAPE_FOLLOWING_REDUCED_SPEED);
    ((digitalRead(INTERSECTION_QRD_RIGHT) == ON) || rightTurn) ? rightTurn = true : rightTurn = false;
    ((digitalRead(INTERSECTION_QRD_LEFT) == ON) || leftTurn) ? leftTurn = true : leftTurn = false;
    if (doneSearching)
    {
      directionalScan(false);
    }
    else
    {
      directionalScan(true);
    }
    // if we run off the tape, end our little inching forward
    if (digitalRead(TAPE_FOLLOWING_QRD_RIGHT) == OFF && digitalRead(TAPE_FOLLOWING_QRD_LEFT) == OFF)
    {
      break;
    }
  }

  // check if we can go straight
  (digitalRead(TAPE_FOLLOWING_QRD_RIGHT) == ON || digitalRead(TAPE_FOLLOWING_QRD_LEFT) == ON) ? straightThrough = true : straightThrough = false;
  
  */
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
  if (!straightThrough)
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
      ((digitalRead(INTERSECTION_QRD_RIGHT) == ON) || rightTurn) ? rightTurn = true : rightTurn = false;
      ((digitalRead(INTERSECTION_QRD_LEFT) == ON) || leftTurn) ? leftTurn = true : leftTurn = false;
      delay(INTERSECTION_WIGGLE_TIME >> 4);
    }
    hardStop();
  }

  if (!straightThrough)
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
      ((digitalRead(INTERSECTION_QRD_RIGHT) == ON) || rightTurn) ? rightTurn = true : rightTurn = false;
      ((digitalRead(INTERSECTION_QRD_LEFT) == ON) || leftTurn) ? leftTurn = true : leftTurn = false;
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
    //delay(INTERSECTION_WAIT_TIME);
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
    //delay(INTERSECTION_WAIT_TIME);

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
    //delay(INTERSECTION_WAIT_TIME);

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

  // set new previous turn according to decision made here
  if (travelDirection == LEFT)
  {
    setPreviousTurn(LEFT);
    lastTurn = LEFT;
  }
  else if (travelDirection == RIGHT)
  {
    setPreviousTurn(RIGHT);
    lastTurn = RIGHT;
  }
  else // forward
  {
    lastTurn = FORWARD;
    if (leftTurn && !rightTurn)
    {
      setPreviousTurn(LEFT);
    }
    else if (rightTurn && !leftTurn)
    {
      setPreviousTurn(RIGHT);
    }
    else
    {
      setPreviousTurn(FORWARD);
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
  centreAlign(dollSide);
  LCD.clear();
  LCD.home();
  LCD.print("DOLL");
  delay(DOLL_PICKUP_WAIT);
  if (dollSide == RIGHT)
  {
    LCD.print(" right side");
    //delay(1000);
    passengerAquire(rightPickUp);
  }
  else if (dollSide == LEFT)
  {
    LCD.print(" left side");
    //delay(1000);
    passengerAquire(leftPickUp);
  }
  //setLoadStatus(TRUE);
  //setTravelAngle(90);
}

/*
   function - dropoffSide

   Handles drop-off, depending on input side direction
*/
void dropoffHandler(direction_e dropoffSide)
{
  LCD.home();
  LCD.clear();
  LCD.print("DROPOFF");
  hardStop();

  if (dropoffSide == RIGHT)
  {
    hardStop();
    motor.speed(MOTOR_RIGHT_WHEEL, PULLOVER_SPEED >> 1);
    motor.speed(MOTOR_LEFT_WHEEL, PULLOVER_SPEED >> 1);
    unload();
  }
  else // left side dropoff
  {
    // need to move forward, and turn around 
    tapeFollowForTime(DROPOFF_TURN_AROUND_WAIT, TAPE_FOLLOWING_DEFAULT_SPEED);
    hardStop();

    // Turn around, and unload
    xPointTurn();
    motor.speed(MOTOR_CONVEYOR, -CONVEYOR_SPEED);
    tapeFollowForTime(DROPOFF_CONVEYOR_WAIT_TIME, TAPE_FOLLOWING_REDUCED_SPEED);
    motor.stop(MOTOR_CONVEYOR);
  }

  doneSearching = false;
  dollCount = 0;
}

