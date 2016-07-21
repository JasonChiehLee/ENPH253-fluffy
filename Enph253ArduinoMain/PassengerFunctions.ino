/*
   function - armCommand

   Moves arm to location for pickup and waits for clawCommand to grab passenger. Moves aquired passenger to dropOff on conveyor belt.
*/
void passengerAquire(armPosition_t pickUp, armPosition_t dropOff)
{
  delay(SERVO_WAIT_TICK);
  RCServo0.write(pickUp.baseRotationAngle);
  delay(SERVO_WAIT_TICK);
  RCServo1.write(pickUp.backPositionAngle);
  delay(SERVO_WAIT_TICK);

  byte positionStep = pickUp.frontPositionAngle >> 3;

  while (positionStep < pickUp.frontPositionAngle && digitalRead(DOLL_SWITCH) == PRESS_NO)
  {
    RCServo2.write(positionStep);
    positionStep += positionStep;
    delay(ARM_POSITION_TICK);
  }

  delay(ARM_WAIT_TICK);

  clawCommand(CLOSE);

  delay(ARM_WAIT_TICK);

  RCServo0.write(dropOff.baseRotationAngle);
  delay(SERVO_WAIT_TICK);
  RCServo1.write(dropOff.backPositionAngle);
  delay(SERVO_WAIT_TICK);
  RCServo2.write(dropOff.frontPositionAngle);

  delay(ARM_WAIT_TICK);

  clawCommand(OPEN);

  delay(ARM_WAIT_TICK);

  RCServo0.write(reset.baseRotationAngle);
  delay(SERVO_WAIT_TICK);
  RCServo1.write(reset.frontPositionAngle);
  delay(SERVO_WAIT_TICK);
  RCServo2.write(reset.backPositionAngle);
  delay(SERVO_WAIT_TICK);
}

/*
   function - clawComamnd

   Open or closes claw.
*/
void clawCommand(clamp_e command)
{
  if (command == CLOSE)
  {
    // close claw
    motor.speed(MOTOR_CLAW, CLAW_SPEED);
    while (digitalRead(CLAW_SWITCH) == PRESS_NO)
    {
      delay(MOTOR_WRITE_WAIT_TICK);
      setClawClampTick(clawClampTick + 1);
    }
  }
  else
  {
    // open claw
    motor.speed(MOTOR_CLAW, -CLAW_SPEED);
    while (clawClampTick != 0)
    {
      delay(MOTOR_WRITE_WAIT_TICK);
      setClawClampTick(clawClampTick - 1);
    }
  }
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


