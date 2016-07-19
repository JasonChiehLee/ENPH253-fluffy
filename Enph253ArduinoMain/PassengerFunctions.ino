/*
   function - armCommand

   Moves arm to location for pickup and waits for clawCommand to grab passenger. Moves aquired passenger to dropOff on conveyor belt.
*/
void passengerAquire(armPosition_t pickUp, armPosition_t dropOff)
{
  RCServo0.write(pickUp.baseRotationAngle);
  RCServo1.write(pickUp.frontPositionAngle);

  byte positionStep = pickUp.backPositionAngle >> 3;
  byte tick = 0;

  while (positionStep < pickUp.backPositionAngle)
  {
    RCServo2.write(positionStep);
    positionStep += positionStep;
  }

  delay(ARM_WAIT_TICK);

  tick = clawCommand(CLOSE, tick);

  delay(ARM_WAIT_TICK);

  RCServo0.write(dropOff.baseRotationAngle);
  RCServo1.write(dropOff.frontPositionAngle);
  RCServo2.write(dropOff.backPositionAngle);

  delay(ARM_WAIT_TICK);

  clawCommand(OPEN, tick);

  delay(ARM_WAIT_TICK);

  RCServo0.write(reset.baseRotationAngle);
  RCServo1.write(reset.frontPositionAngle);
  RCServo2.write(reset.backPositionAngle);
}

/*
   function - clawComamnd

   Open or closes claw.
*/
int clawCommand(clamp_e command, byte tick)
{
  int count = 0;
  if (command == CLOSE)
  {
    // close claw
    motor.speed(MOTOR_CLAW, CLAW_SPEED);
    while (digitalRead(CLAW_SWITCH) == ON)
    {
      delay(MOTOR_WRITE_WAIT_TICK);
      count++;
    }
  }
  else
  {
    // open claw
    motor.speed(MOTOR_CLAW, -CLAW_SPEED);
    while (tick != 0)
    {
      delay(MOTOR_WRITE_WAIT_TICK);
      tick--;
    }
  }
  return count;
}

/*
   function - conveyorCommand

   Rotates conveyor belt right or left.
*/
void conveyorCommand(rotation_e command)
{
  if (command == STARBOARD)
  {
    motor.speed(MOTOR_CONVEYOR, CONVEYOR_SPEED);
    delay(CONVEYOR_TIME_TICK);
  }
  else
  {
    motor.speed(MOTOR_CONVEYOR, -CONVEYOR_SPEED);
    delay(CONVEYOR_TIME_TICK);
  }
}


