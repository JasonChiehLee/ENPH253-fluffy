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
  int proportionalGain = PROPORTIONAL_GAIN; //knob(6) >> 2;
  int derivativeGain = DERIVATIVE_GAIN; //knob(7) >> 2;

  int rightQRD = digitalRead(TAPE_FOLLOWING_QRD_RIGHT);
  int leftQRD = digitalRead(TAPE_FOLLOWING_QRD_LEFT);

  int currentError = 0;

  // Proportionality
  if (leftQRD == ON && rightQRD == ON)
    currentError = 0;
  else if (leftQRD == ON && rightQRD == OFF)
    currentError = 1;
  else if (leftQRD == OFF && rightQRD == ON)
    currentError = -1;
  else if (leftQRD == OFF && rightQRD == OFF)
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
    LCD.print(analogRead(FRONT_QSD1));

    LCD.setCursor(5, 0);
    LCD.print(analogRead(FRONT_QSD2));

    LCD.setCursor(10, 0);
    LCD.print(analogRead(FRONT_QSD3));

    LCD.setCursor(0, 1);
    LCD.print(analogRead(FRONT_QSD4));

    LCD.setCursor(5, 1);
    LCD.print(analogRead(FRONT_QSD5));

    LCD.setCursor(10, 1);
    LCD.print(analogRead(SIDE_QSD_LEFT));
    LCD.print(" ");
    LCD.print(analogRead(SIDE_QSD_RIGHT));

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
    LCD.print(" turn left");
    motor.speed(MOTOR_RIGHT_WHEEL, INTERSECTION_TURN_SPEED);
    motor.stop(MOTOR_LEFT_WHEEL);
    // wait until both QRDs are off - excellent hard coding
    delay(INTERSECTION_TURN_TIME);
    // wait until at least one QRD is on again
    while ((digitalRead(TAPE_FOLLOWING_QRD_RIGHT) == OFF) && (digitalRead(TAPE_FOLLOWING_QRD_LEFT) == OFF))
    {
      delay(INTERSECTION_TURN_WAIT_TICK);
    }
    motor.stop(MOTOR_RIGHT_WHEEL);
    setPreviousTurn(LEFT);
  }
  else if (turnDirection == RIGHT)
  {
    LCD.print(" turn right");
    motor.stop(MOTOR_RIGHT_WHEEL);
    motor.speed(MOTOR_LEFT_WHEEL, INTERSECTION_TURN_SPEED);
    // wait until both QRDs are off - excellent ard coding
    delay(INTERSECTION_TURN_TIME);
    // wait until both QRDs are on again
    while (digitalRead(TAPE_FOLLOWING_QRD_RIGHT) == OFF && digitalRead(TAPE_FOLLOWING_QRD_LEFT) == OFF)
    {
      delay(INTERSECTION_TURN_WAIT_TICK);
    }
    motor.stop(MOTOR_LEFT_WHEEL);
    setPreviousTurn(RIGHT);
  }
  else // FORWARD
  {
    LCD.print(" forward");
    motor.speed(MOTOR_RIGHT_WHEEL, currentMotorSpeedRight);
    motor.speed(MOTOR_LEFT_WHEEL, currentMotorSpeedLeft);
  }
  setTravelAngle(90);
}

/*
   function - pullOver

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
  while (digitalRead(FRONT_RIGHT_GROUND_SWITCH) == PRESS_YES && digitalRead(FRONT_LEFT_GROUND_SWITCH) == PRESS_YES)
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

   Turns the robot around, making a 1 or 2 point turn depending on the situation
*/
void xPointTurn() {
  byte powerMotor;
  byte pivotMotor;
  if (previousTurn == RIGHT)
  {
    direction_e firstTurn = LEFT;
    powerMotor = MOTOR_RIGHT_WHEEL;
    pivotMotor = MOTOR_LEFT_WHEEL;
    previousTurn = LEFT;
  }
  else
  {
    direction_e firstTurn = RIGHT;
    powerMotor = MOTOR_LEFT_WHEEL;
    pivotMotor = MOTOR_RIGHT_WHEEL;
    previousTurn = RIGHT;
  }



  //Back-up, until back bumper is hit
  LCD.clear();
  LCD.home();
  LCD.print("Backing up");
  motor.speed(powerMotor, UTURN_SPEED >> 1);
  motor.speed(pivotMotor, -UTURN_SPEED);

  delay(UTURN_TAPE_IGNORE_TIME);
  while (!(digitalRead(BACK_LEFT_BUMPER_SWITCH) == PRESS_YES || digitalRead(BACK_RIGHT_BUMPER_SWITCH) == PRESS_YES))
  {
    if (digitalRead(TAPE_FOLLOWING_QRD_LEFT) || digitalRead(TAPE_FOLLOWING_QRD_RIGHT))
    {
      return;
    }
    if (digitalRead(FRONT_LEFT_BUMPER_SWITCH) == PRESS_YES || digitalRead(FRONT_RIGHT_BUMPER_SWITCH) == PRESS_YES)
    {
      LCD.print("Full reverse");
      motor.speed(powerMotor, -UTURN_SPEED);
      motor.speed(pivotMotor, -UTURN_SPEED);
    }
    delay(MOTOR_WRITE_WAIT_TICK);
  }

  LCD.clear();
  LCD.home();
  LCD.print("Forward");
  motor.speed(powerMotor, UTURN_SPEED);
  motor.speed(pivotMotor, -UTURN_SPEED >> 1);
  while (!(digitalRead(TAPE_FOLLOWING_QRD_LEFT) || digitalRead(TAPE_FOLLOWING_QRD_RIGHT)))
  {
    delay(MOTOR_WRITE_WAIT_TICK);
  }
}

/*
   function - centreAlign

   Centres robot adjacent to doll, using the side sensor corresponding to the pickUp direction variable
*/
void centreAlign (direction_e pickUp)
{
  LCD.clear();
  LCD.home();
  LCD.print("Aligning");
  byte signalSensor;

  // check which sensor to use, based on input direction
  if (pickUp == RIGHT)
  {
    signalSensor = SIDE_QSD_RIGHT;
  }
  else
  {
    signalSensor = SIDE_QSD_LEFT;
  }

  // forward until we reach a max
  int lastSignal = analogReadAvg(signalSensor);
  motor.speed(MOTOR_RIGHT_WHEEL, CENTRE_SPEED);
  motor.speed(MOTOR_LEFT_WHEEL, CENTRE_SPEED);
  int currentSignal = analogReadAvg(signalSensor);
  while (currentSignal >= lastSignal - 2)
  {
    delay(MOTOR_WRITE_WAIT_TICK);
    Serial.println(currentSignal);
    lastSignal = currentSignal;
    currentSignal = analogReadAvg(signalSensor);
  }
  motor.stop(MOTOR_RIGHT_WHEEL);
  motor.stop(MOTOR_LEFT_WHEEL);

  delay(3000);

  // @TODO do we need a backing up section?

}

