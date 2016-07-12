/*
   function - intersectionHandler

   Takes in left and right intersection sensor readings, calls necessary function to compute optimal direction to turn.
   Proceeds to call
*/
void intersectionHandler(int QRDRight, int QRDLeft)
{
  // For now, just halt the bot
  hardStop();

  // Check available directions
  boolean rightTurn, leftTurn, straightThrough;
  (QRDRight > QRD_GROUND_THRESHOLD) ? rightTurn = true : rightTurn = false;
  (QRDLeft > QRD_GROUND_THRESHOLD) ? leftTurn = true : leftTurn = false;
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
  delay(2000);
  intersectionTurn(nextDirection);
}

/*
 * function - dollHandler
 * 
 * Carries out aqcuisition actions when called
 */
void dollHandler()
{
  
}

