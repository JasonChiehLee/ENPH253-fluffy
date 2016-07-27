/*
   function - armCommand

   Moves arm to location for pickup and waits for clawCommand to grab passenger. Moves aquired passenger to dropOff on conveyor belt.
*/
#include <LiquidCrystal.h>
void passengerAquire(armPosition_t pickUp, armPosition_t dropOff)
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
  RCServo0.write(pickUp.baseRotationAngle);
  delay(SERVO_WAIT_TIME);
  RCServo1.write(pickUp.backPositionAngle);
  RCServo2.write(pickUp.frontPositionAngle);
  delay(SERVO_WAIT_TIME);

  // extend front until doll detection
  byte positionStep = pickUp.frontPositionAngle >> 3;
  byte frontPosition = pickUpInit.frontPositionAngle;

  while (digitalRead(DOLL_SWITCH) == PRESS_NO)
  {
    // front switch press = fail
    if (digitalRead(CLAW_CLOSE_SWITCH) == PRESS_YES)
    {
      LCD.setCursor(0,1);
      LCD.print("Failed");
      delay(FAIL_WAIT_TIME);
      return;
    }
    // extend front position gradually
    else if (positionStep > pickUp.frontPositionAngle)
    {
    RCServo2.write(frontPosition);
    frontPosition -= positionStep;
    delay(ARM_POSITION_TIME);
    }
    // no doll detection = fail
    else
    {
      LCD.setCursor(0,1);
      LCD.print("Failed");
      delay(FAIL_WAIT_TIME);
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
  RCServo0.write(dropOff.baseRotationAngle);
  delay(SERVO_WAIT_TIME);
  RCServo2.write(dropOff.frontPositionAngle);
  RCServo1.write(dropOff.backPositionAngle);
  delay(SERVO_WAIT_TIME);

  // open claw
  delay(ARM_WAIT_TIME);
  clawCommand(OPEN);
  delay(ARM_WAIT_TIME);

  // back to reset position (NOTE ORDER)
  RCServo0.write(reset.baseRotationAngle);
  delay(SERVO_WAIT_TIME);
  RCServo2.write(reset.backPositionAngle);
  RCServo1.write(reset.frontPositionAngle);
  delay(SERVO_WAIT_TIME);
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
    while (digitalRead(CLAW_CLOSE_SWITCH) == PRESS_NO)
    {
      delay(MOTOR_WRITE_WAIT_TIME);
    }
  }
  else
  {
    // open claw
    motor.speed(MOTOR_CLAW, -CLAW_SPEED);
    while(digitalRead(CLAW_OPEN_SWITCH) == PRESS_NO)
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


