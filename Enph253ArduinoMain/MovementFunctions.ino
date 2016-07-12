# define TAPE_FOLLOWING_TRESHOLD QRD_GROUND_THRESHOLD
# define TAPE_FOLLOWING_CORRECTION 3
# define HARD_STOP_REVERSE_SPEED  -100 //between -255 and 255
# define HARD_STOP_REVERSE_TIME 100 //ms
# define INTERSECTION_TURN_SPEED 150
# define INTERSECTION_TURN_WAIT_TICK 5
/*
   function - followLine

   Checks for interrupts, if processInterrupt falls through with no interrupt conditions detected, proceeds through one loop
   of PID control to make the bot follow the line.
*/
int tapeFollowing_lastError = 0;
int currentMotorSpeed = 150;
int tapeFollowing_loopCount = 0; // for testing/debug ~ remove if we're good here
int tapeFollowing_ticksSinceLastError = 0; // m in sample code
int tapeFollowing_lastDifferentError = 0; // recerr in sample code
int tapeFollowing_errorHistory = 0; // q in sample code

void tapeFollow()
{ // Left is negative, right is positive!

  // Check for interrupts
  processInterrupts();

  // @TODO: move these to # defines after calibrating?
  int proportionalGain = knob(6) >> 2;
  int derivativeGain = knob(7) >> 2;

  int rightQRD = analogRead(TAPE_FOLLOWING_QRD_RIGHT);
  int leftQRD = analogRead(TAPE_FOLLOWING_QRD_LEFT);

  int currentError = 0;

  // Proportionality
  if (leftQRD > TAPE_FOLLOWING_TRESHOLD && rightQRD > TAPE_FOLLOWING_TRESHOLD)
    currentError = 0;
  else if (leftQRD > TAPE_FOLLOWING_TRESHOLD && rightQRD <= TAPE_FOLLOWING_TRESHOLD)
    currentError = 1;
  else if (leftQRD <= TAPE_FOLLOWING_TRESHOLD && rightQRD > TAPE_FOLLOWING_TRESHOLD)
    currentError = -1;
  else if (leftQRD <= TAPE_FOLLOWING_TRESHOLD && rightQRD <= TAPE_FOLLOWING_TRESHOLD)
  {
    // Both off, check history and correct
    if (tapeFollowing_lastError > 0)
      currentError = TAPE_FOLLOWING_CORRECTION;
    else if (tapeFollowing_lastError <= 0)
      currentError = -TAPE_FOLLOWING_CORRECTION;
    // @TODO: case where last error is 0?
  }

  // Derivative
  if (currentError != tapeFollowing_lastError)
  {
    tapeFollowing_lastDifferentError = tapeFollowing_lastError;
    tapeFollowing_errorHistory = tapeFollowing_ticksSinceLastError;//What is Q?
    tapeFollowing_ticksSinceLastError = 1;
  }

  // Add adjustments
  int pAdjust = proportionalGain * currentError;
  int dAdjust = derivativeGain * (int)(float)(currentError - tapeFollowing_lastDifferentError)
                / (float)(tapeFollowing_ticksSinceLastError + tapeFollowing_errorHistory);
  int totalAdjustment = pAdjust + dAdjust;

#if 1 // FOR DEBUG/TEST, DISABLE FOR ACTUAL RUNNING
  if (tapeFollowing_loopCount == 30)
  {
    LCD.clear();
    LCD.setCursor(0, 0);
    LCD.print(rightQRD);

    LCD.setCursor(5, 0);
    LCD.print(leftQRD);

    LCD.setCursor(10, 0);
    LCD.print(proportionalGain);

    LCD.setCursor(0, 1);
    LCD.print(derivativeGain);

    LCD.setCursor(5, 1);
    LCD.print(pAdjust);

    LCD.setCursor(10, 1);
    LCD.print(dAdjust);

    tapeFollowing_loopCount = 0;
  }
#endif

  // Increment tick counters and write adjustments to motors, update last error
  tapeFollowing_loopCount += 1;
  tapeFollowing_ticksSinceLastError += 1;
  motor.speed(MOTOR_RIGHT_WHEEL, currentMotorSpeed + totalAdjustment);
  motor.speed(MOTOR_LEFT_WHEEL, currentMotorSpeed - totalAdjustment);
  tapeFollowing_lastError = currentError;

}
/*
   function - hardStop

   Brings the bot to a halt as fast as possible by putting motors in reverse for a set amount of time
*/
void hardStop() {

  motor.speed(MOTOR_RIGHT_WHEEL, HARD_STOP_REVERSE_SPEED);
  motor.speed(MOTOR_LEFT_WHEEL, HARD_STOP_REVERSE_SPEED);
  delay(HARD_STOP_REVERSE_TIME);
  motor.stop(MOTOR_RIGHT_WHEEL);
  motor.stop(MOTOR_LEFT_WHEEL);

}

/*
   function - intersectionTurn

   Turns the robot in the direction_e specified in the call to this functions, ends once tape has been found
   Assumes that we can turn in that direction!
*/
void intersectionTurn(direction_e turnDirection) {
  if (turnDirection == LEFT)
  {
    motor.speed(MOTOR_RIGHT_WHEEL, INTERSECTION_TURN_SPEED);
    motor.stop(MOTOR_LEFT_WHEEL);
    // wait until both QRDs are off
    while ((analogRead(TAPE_FOLLOWING_QRD_LEFT) > TAPE_FOLLOWING_TRESHOLD) || (analogRead(TAPE_FOLLOWING_QRD_RIGHT) > TAPE_FOLLOWING_TRESHOLD))
    {
      delay(INTERSECTION_TURN_WAIT_TICK);
    }
    // wait until at least one QRD is on again
    while ((analogRead(TAPE_FOLLOWING_QRD_RIGHT) < TAPE_FOLLOWING_TRESHOLD) && (analogRead(TAPE_FOLLOWING_QRD_LEFT) < TAPE_FOLLOWING_TRESHOLD))
    {
      delay(INTERSECTION_TURN_WAIT_TICK);
    }
    motor.stop(MOTOR_RIGHT_WHEEL);
  }
  else if (turnDirection == RIGHT)
  {
    motor.stop(MOTOR_RIGHT_WHEEL);
    motor.speed(MOTOR_LEFT_WHEEL, INTERSECTION_TURN_SPEED);
    // wait until both QRDs are off
    while ((analogRead(TAPE_FOLLOWING_QRD_LEFT) > TAPE_FOLLOWING_TRESHOLD) || (analogRead(TAPE_FOLLOWING_QRD_RIGHT) > TAPE_FOLLOWING_TRESHOLD))
    {
      delay(INTERSECTION_TURN_WAIT_TICK);
    }
    // wait until both QRDs are on again
    while (analogRead(TAPE_FOLLOWING_QRD_RIGHT) < TAPE_FOLLOWING_TRESHOLD && analogRead(TAPE_FOLLOWING_QRD_LEFT) < TAPE_FOLLOWING_TRESHOLD)
    {
      delay(INTERSECTION_TURN_WAIT_TICK);
    }
    motor.stop(MOTOR_LEFT_WHEEL);
  }
  // if the plan is to go straight, no action needed
}

