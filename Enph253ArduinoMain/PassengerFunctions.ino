/*
   function - armCommand

   Moves arm to location for pickup and waits for clawCommand to grab passenger. Moves aquired passenger to dropOff on conveyor belt.
*/
void passengerAquire(armPosition_t pickUp, armPosition_t dropOff)
{
  RCServo0.write(pickUp.baseRotationAngle);
  RCServo1.write(pickUp.frontPositionAngle);

  byte positionStep = pickUp.backPositionAngle >> 3;

  while (positionStep < pickUp.backPositionAngle)
  {
    RCServo2.write(positionStep);
    positionStep += positionStep;
  }

  clawCommand(CLOSE);

  RCServo0.write(dropOff.baseRotationAngle);
  RCServo1.write(dropOff.frontPositionAngle);
  RCServo2.write(dropOff.backPositionAngle);

  clawCommand(OPEN);
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
  }
  else
  {
    // open claw
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
    // rotate right
  }
  else
  {
    // rotate left
  }
}


