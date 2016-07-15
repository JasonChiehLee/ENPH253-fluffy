/*
   function - followLine

   Checks for interrupts, if processInterrupt falls through with no interrupt conditions detected, proceeds through one loop
   of PID control to make the bot follow the line.
*/
int currentMotorSpeedRight = TAPE_FOLLOWING_DEFAULT_SPEED;
int currentMotorSpeedLeft = TAPE_FOLLOWING_DEFAULT_SPEED;
int tapeFollowing_lastError = 0;
int tapeFollowing_loopCount = 0; // for testing/debug ~ remove if we're good here
int tapeFollowing_ticksSinceLastError = 0; // m in sample code
int tapeFollowing_lastDifferentError = 0; // recerr in sample code
int tapeFollowing_errorHistory = 0; // q in sample code

void tapeFollow()
{ // Left is negative, right is positive!

  // Check for interrupts
  processInterrupts();

  // Reset motor speeds
  currentMotorSpeedRight = TAPE_FOLLOWING_DEFAULT_SPEED;
  currentMotorSpeedLeft = TAPE_FOLLOWING_DEFAULT_SPEED;

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

  // Update motor speeds
  currentMotorSpeedRight = currentMotorSpeedRight + totalAdjustment;
  currentMotorSpeedLeft = currentMotorSpeedLeft - totalAdjustment;

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
    LCD.print(currentMotorSpeedLeft);

    LCD.setCursor(10, 1);
    LCD.print(currentMotorSpeedRight);

    tapeFollowing_loopCount = 0;
  }
#endif

  // Increment tick counters and write adjustments to motors, update last error
  tapeFollowing_loopCount += 1;
  tapeFollowing_ticksSinceLastError += 1;
  motor.speed(MOTOR_RIGHT_WHEEL, currentMotorSpeedRight);
  motor.speed(MOTOR_LEFT_WHEEL, currentMotorSpeedLeft);
  tapeFollowing_lastError = currentError;

}
/*
   function - hardStop

   Stops the wheels
*/
void hardStop() {
  
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
    /*
    while ((analogRead(TAPE_FOLLOWING_QRD_LEFT) > TAPE_FOLLOWING_TRESHOLD) || (analogRead(TAPE_FOLLOWING_QRD_RIGHT) > TAPE_FOLLOWING_TRESHOLD))
    {
      delay(INTERSECTION_TURN_WAIT_TICK);
    }
    */
    delay(INTERSECTION_TURN_TIME);
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
    /*
    while ((analogRead(TAPE_FOLLOWING_QRD_LEFT) > TAPE_FOLLOWING_TRESHOLD) || (analogRead(TAPE_FOLLOWING_QRD_RIGHT) > TAPE_FOLLOWING_TRESHOLD))
    {
      delay(INTERSECTION_TURN_WAIT_TICK);
    }
    */
    delay(INTERSECTION_TURN_TIME);
    // wait until both QRDs are on again
    while (analogRead(TAPE_FOLLOWING_QRD_RIGHT) < TAPE_FOLLOWING_TRESHOLD && analogRead(TAPE_FOLLOWING_QRD_LEFT) < TAPE_FOLLOWING_TRESHOLD)
    {
      delay(INTERSECTION_TURN_WAIT_TICK);
    }
    motor.stop(MOTOR_LEFT_WHEEL);
  }
  else // FORWARD
  {
    motor.speed(MOTOR_RIGHT_WHEEL, currentMotorSpeedRight);
    motor.speed(MOTOR_LEFT_WHEEL, currentMotorSpeedLeft);
  }
}

/*
   function - pullover

   Pull over in a specified direction and uses the front, ground switches to find edge of surface!
*/
void pullOver(direction_e pulloverDirection) {
  int count = 0;
  byte firstPowerMotor;
  byte secondPowerMotor;
  
  if (pulloverDirection == RIGHT)
  {
    firstPowerMotor = MOTOR_LEFT_WHEEL;
    secondPowerMotor = MOTOR_RIGHT_WHEEL;
  }
  else
  {
    firstPowerMotor = MOTOR_RIGHT_WHEEL;
    secondPowerMotor = MOTOR_LEFT_WHEEL;
  }

  motor.speed(firstPowerMotor, PULLOVER_SPEED);
  motor.stop(secondPowerMotor);
  while (digitalRead(FRONT_RIGHT_GROUND_SWITCH) == OFF && digitalRead(FRONT_LEFT_GROUND_SWITCH) == OFF)
  {
    delay(MOTOR_WRITE_WAIT_TICK);
    count++;
  }
  
  motor.stop(firstPowerMotor);
  motor.speed(secondPowerMotor, PULLOVER_SPEED);
  while (count != 0)
  {
    delay(MOTOR_WRITE_WAIT_TICK);
    count--;
  }
}

/*
   function - xPointTurn

   Performs (3 or 5) point turn depending on whether a cliff is present!
*/
void xPointTurn(direction_e lastPickUpDirection, int numPoints) {
  byte powerMotor;
  byte pivotMotor;
  if (lastPickUpDirection == RIGHT)
  {
    direction_e firstTurn = LEFT;
    powerMotor = MOTOR_RIGHT_WHEEL;
    pivotMotor = MOTOR_LEFT_WHEEL;
  }
  else
  {
    direction_e firstTurn = RIGHT;
    powerMotor = MOTOR_LEFT_WHEEL;
    pivotMotor = MOTOR_RIGHT_WHEEL;
  }

  
  if (numPoints == 3)
  //Three-point turn
  {
    motor.speed(powerMotor, UTURN_SPEED);
    motor.speed(pivotMotor, -UTURN_SPEED >> 1);
    while (digitalRead(FRONT_RIGHT_GROUND_SWITCH) == OFF && digitalRead(FRONT_LEFT_GROUND_SWITCH) == OFF)
    {
      delay(MOTOR_WRITE_WAIT_TICK);
    }
    
    motor.speed(powerMotor, UTURN_SPEED >> 1);
    motor.speed(pivotMotor, -UTURN_SPEED);
    while (digitalRead(BACK_RIGHT_BUMPER_SWITCH) == ON && digitalRead(BACK_LEFT_BUMPER_SWITCH) == ON)
    {
      delay(MOTOR_WRITE_WAIT_TICK);
    }
    
    motor.speed(powerMotor, UTURN_SPEED);
    motor.speed(pivotMotor, -UTURN_SPEED >> 1);
    while (analogRead(TAPE_FOLLOWING_QRD_RIGHT) < TAPE_FOLLOWING_TRESHOLD && analogRead(TAPE_FOLLOWING_QRD_LEFT) < TAPE_FOLLOWING_TRESHOLD)
    {
      delay(MOTOR_WRITE_WAIT_TICK);
    }
  }
  else
  //Five-point turn
  {
    motor.speed(powerMotor, UTURN_SPEED);
    motor.speed(pivotMotor, -UTURN_SPEED >> 1);
    while (digitalRead(FRONT_RIGHT_BUMPER_SWITCH) == ON && digitalRead(FRONT_LEFT_BUMPER_SWITCH) == ON)
    {
      delay(MOTOR_WRITE_WAIT_TICK);
    }

    motor.speed(powerMotor, UTURN_SPEED >> 1);
    motor.speed(pivotMotor, -UTURN_SPEED);
    while (digitalRead(BACK_RIGHT_BUMPER_SWITCH) == ON && digitalRead(BACK_LEFT_BUMPER_SWITCH) == ON)
    {
      delay(MOTOR_WRITE_WAIT_TICK);
    }

    motor.speed(powerMotor, UTURN_SPEED);
    motor.speed(pivotMotor, -UTURN_SPEED/2);
    while (digitalRead(FRONT_RIGHT_BUMPER_SWITCH) == ON && digitalRead(FRONT_LEFT_BUMPER_SWITCH) == ON)
    {
      delay(MOTOR_WRITE_WAIT_TICK);
    }

    motor.speed(powerMotor, UTURN_SPEED >> 1);
    motor.speed(pivotMotor, -UTURN_SPEED);
    while (digitalRead(BACK_RIGHT_BUMPER_SWITCH) == ON && digitalRead(BACK_LEFT_BUMPER_SWITCH) == ON)
    {
      delay(MOTOR_WRITE_WAIT_TICK);
    }

    motor.speed(powerMotor, UTURN_SPEED);
    motor.speed(pivotMotor, -UTURN_SPEED >> 1);
    while (analogRead(TAPE_FOLLOWING_QRD_RIGHT) < TAPE_FOLLOWING_TRESHOLD && analogRead(TAPE_FOLLOWING_QRD_LEFT) < TAPE_FOLLOWING_TRESHOLD)
    {
      delay(MOTOR_WRITE_WAIT_TICK);
    }
  }
}
