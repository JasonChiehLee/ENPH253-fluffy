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
void processInterrupts() {
  
  // testing stop switch
  if (!digitalRead(TEST_STOP_BUTTON))
  {
    // stop the bot
    hardStop();
    // show that we've stopped
    LCD.clear();
    LCD.home();
    LCD.print("Stopped");
    delay(2000);
  }

  // check for intersection - not implemented in mech yet!
  int intersectionQRDRight = analogRead(INTERSECTION_QRD_RIGHT);
  int intersectionQRDLeft = analogRead(INTERSECTION_QRD_LEFT);
  if (0)//(intersectionQRDRight > QRD_GROUND_THRESHOLD) || (intersectionQRDLeft > QRD_GROUND_THRESHOLD))
  {
    LCD.clear();
    LCD.home();
    LCD.print("At intersection");
    intersectionHandler(intersectionQRDRight, intersectionQRDLeft);
    delay(5000);
  }
}

