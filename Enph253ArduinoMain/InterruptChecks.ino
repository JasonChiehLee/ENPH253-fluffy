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

  // check for adjacent dolls
  if (analogRead(SIDE_QSD_RIGHT) > QSD_SIDE_THRESHOLD || analogRead(SIDE_QSD_LEFT) > QSD_SIDE_THRESHOLD)
  {
    dollHandler();
  }

/*
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
*/

  // check for intersection
  int intersectionQRDRight = analogRead(INTERSECTION_QRD_RIGHT);
  int intersectionQRDLeft = analogRead(INTERSECTION_QRD_LEFT);
  if ((intersectionQRDRight > QRD_GROUND_THRESHOLD) || (intersectionQRDLeft > QRD_GROUND_THRESHOLD))
  {
    intersectionHandler();
  }

}

