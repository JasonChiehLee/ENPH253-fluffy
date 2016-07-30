/*
   function - armCommand

   Moves arm to location for pickup and waits for clawCommand to grab passenger. Moves aquired passenger to dropOff on conveyor belt.
*/
#include <LiquidCrystal.h>
void passengerAquire(armPosition_t pickUp)
{
  LCD.clear();
  LCD.home();
  LCD.print("Passenger Acquire");

  // set movement position (NOTE ORDER)
  delay(SERVO_WAIT_TIME);
  RCServo2.write(movingPosition.frontPositionAngle);
  RCServo1.write(movingPosition.backPositionAngle);

  // move to pickup position (NOTE ORDER)
  delay(SERVO_WAIT_TIME);
  if (pickUp.baseRotationAngle == rightPickUp.baseRotationAngle)
  {
    RCServo0.write(pickUp.baseRotationAngle);
  }
  else
  {
    // less strain on arm to move in small increments
    RCServo0.write(movingPosition.baseRotationAngle + ((pickUp.baseRotationAngle - movingPosition.baseRotationAngle) / 2));
    delay(SERVO_WAIT_TIME);
    RCServo0.write(pickUp.baseRotationAngle);
  }
  delay(SERVO_WAIT_TIME);
  RCServo2.write(pickUpInitLimit1.frontPositionAngle);
  delay(SERVO_WAIT_TIME);
  RCServo1.write(pickUpInit.backPositionAngle);
  RCServo2.write(pickUpInit.frontPositionAngle);
  delay(SERVO_WAIT_TIME + 500);

  // extend front until doll detection
  byte positionStep = (pickUpInit.frontPositionAngle - pickUp.frontPositionAngle) >> 3;
  byte frontPosition = pickUpInit.frontPositionAngle;

  while (digitalRead(DOLL_SWITCH) == PRESS_NO)
  {
    // front switch press = fail

    if (digitalRead(CLAW_CLOSE_SWITCH) == PRESS_YES)
    {
      LCD.setCursor(0, 1);
      LCD.print("Failed Front Switch");
      delay(FAIL_WAIT_TIME);
      delay(SERVO_WAIT_TIME);
      RCServo1.write(reset.backPositionAngle);
      RCServo2.write(reset.frontPositionAngle);
      delay(SERVO_WAIT_TIME);
      RCServo0.write(reset.baseRotationAngle);
      return;
    }
    // extend front position gradually
    else if (frontPosition > pickUp.frontPositionAngle)
    {
      RCServo2.write(frontPosition);
      frontPosition -= positionStep;
      delay(ARM_POSITION_TIME);
    }
    // no doll detection = fail
    else
    {
      LCD.setCursor(0, 1);
      LCD.print("Failed No Doll");
      delay(FAIL_WAIT_TIME);
      delay(SERVO_WAIT_TIME);
      RCServo1.write(reset.backPositionAngle);
      RCServo2.write(reset.frontPositionAngle);
      delay(SERVO_WAIT_TIME);
      RCServo0.write(reset.baseRotationAngle);
      return;
    }
  }

  // close claw
  delay(ARM_WAIT_TIME);
  clawCommand(CLOSE);
  delay(ARM_WAIT_TIME);

  // set movement position (NOTE ORDER)
  delay(SERVO_WAIT_TIME);
  RCServo2.write(movingPosition.frontPositionAngle);
  RCServo1.write(movingPosition.backPositionAngle);

  // move to drop off position (NOTE ORDER)
  delay(SERVO_WAIT_TIME);
  if (pickUp.baseRotationAngle == rightPickUp.baseRotationAngle)
  {
    RCServo0.write(dropOffInit.baseRotationAngle);
  }
  else
  {
    // less strain on arm to move in small increments
    RCServo0.write(pickUp.baseRotationAngle - ((pickUp.baseRotationAngle - dropOffInit.baseRotationAngle) / 2));
    delay(SERVO_WAIT_TIME);
    RCServo0.write(dropOffInit.baseRotationAngle);
  }
  delay(SERVO_WAIT_TIME);
  RCServo1.write(dropOffInit.backPositionAngle);
  delay(SERVO_WAIT_TIME);
  RCServo2.write(dropOffInit.frontPositionAngle);
  delay(SERVO_WAIT_TIME);
  RCServo0.write(dropOff.baseRotationAngle);

  // open claw
  delay(ARM_WAIT_TIME);
  clawCommand(OPEN);
  delay(ARM_WAIT_TIME);

  // back to reset position (NOTE ORDER)
  delay(SERVO_WAIT_TIME);
  RCServo0.write(reset.baseRotationAngle);
  delay(SERVO_WAIT_TIME);
  RCServo2.write(reset.frontPositionAngle);
  delay(SERVO_WAIT_TIME);
  RCServo1.write(reset.backPositionAngle);

  conveyorCommand(STARBOARD);
  motor.stop(MOTOR_CONVEYOR);
  delay(SERVO_WAIT_TIME);
  RCServo0.write(movingPosition.baseRotationAngle);
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
    delay(CLAW_CLAMP_WAIT_TIME);
    /*
      while (digitalRead(CLAW_CLOSE_SWITCH) == PRESS_NO)
      {
      delay(MOTOR_WRITE_WAIT_TIME);
      }
    */
  }
  else
  {
    // open claw
    motor.speed(MOTOR_CLAW, -CLAW_SPEED);
    while (digitalRead(CLAW_OPEN_SWITCH) == PRESS_NO)
    {
      delay(MOTOR_WRITE_WAIT_TIME);
    }
    motor.stop(MOTOR_CLAW);
    delay(CLAW_WAIT_TIME);
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
    delay(CONVEYOR_WAIT_TIME);
  }
  else
  {
    motor.speed(MOTOR_CONVEYOR, -CONVEYOR_SPEED);
    delay(CONVEYOR_WAIT_TIME);
  }
}

/*
   function - servoWrite

   Writes to servo in small increments to avoid strain and minimize torque.
*/
void servoWrite(byte servoNum, int currentAngle, int writeAngle, byte numSteps)
{
  int positionStep = (writeAngle - currentAngle)/(numSteps + 1);
  int currentPosition = currentAngle;
  byte count = 0;
  while (count < (numSteps + 1))
  {
    if (servoNum == 0)
    {
      RCServo0.write(currentPosition + positionStep);
      delay(ARM_POSITION_TIME);
    }
    else if (servoNum == 1)
    {
      RCServo1.write(currentPosition + positionStep);
      delay(ARM_POSITION_TIME);
    }
    else
    {
      RCServo2.write(currentPosition + positionStep);
      delay(ARM_POSITION_TIME);
    }
    count++;
  }
}

