/*
   function - processInterrupts

   Checks for intterupts - conditions where actions other than following tape, and calls handling functions accordingly
                         - all functions must terminate nicely once their action is finished
   Currently checks for:
      - testing stop switch - TEST_STOP_BUTTON goes low
          -> hard stop motors
      - intersection detection - INTERSECTION_QRD_RIGHT or INTERSECTION_QRD_LEFT is above threshold
          -> calls intersectionHandler()

   PLEASE ADD ANY NEW INTERRUPTS TO THIS LIST
*/
void processInterrupts()
{
  byte travelAngle = 0;
  if (dropStatus == TRUE)
  {
    dropOffScan(); // also sets travel angle
  }
  else
  {
    travelAngle = intersectionScan();

    // check load status
    armPosition_t dropOff = rightDropOff;
    if (loadStatus == TRUE)
    {
      dropOff = leftDropOff;
    }

    // check for adjacent dolls
    if (0)//analogRead(SIDE_QSD_RIGHT) > QSD_SIDE_THRESHOLD)
    {
      LCD.clear();
      LCD.home();
      LCD.print("DOLL AQUISITION");
      dollHandler(RIGHT, rightPickUp, dropOff);
    }
    else if (0)//analogRead(SIDE_QSD_LEFT) > QSD_SIDE_THRESHOLD)
    {
      LCD.clear();
      LCD.home();
      LCD.print("DOLL AQUISITION");
      dollHandler(LEFT, leftPickUp, dropOff);
    }
  }
  // for collisions
  if (0)//digitalRead(FRONT_RIGHT_BUMPER_SWITCH) == ON || digitalRead(FRONT_LEFT_BUMPER_SWITCH))
  {
    motor.stop(MOTOR_RIGHT_WHEEL);
    motor.stop(MOTOR_LEFT_WHEEL);
    delay(BACKUP_WAIT_TICK);
    motor.speed(MOTOR_RIGHT_WHEEL, -UTURN_SPEED);
    motor.speed(MOTOR_LEFT_WHEEL, -UTURN_SPEED);
    delay(BACKUP_WAIT_TICK);
    xPointTurn(5);
  }

  // check for intersection
  int intersectionQRDRight = digitalRead(INTERSECTION_QRD_RIGHT);
  int intersectionQRDLeft = digitalRead(INTERSECTION_QRD_LEFT);
  if ((intersectionQRDRight == ON) || (intersectionQRDLeft == ON))
  {
    intersectionHandler();
  }

}
