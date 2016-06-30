/*
   function - processInterrupts

   Checks for intterupts - conditions where actions other than following tape, and calls handling functions accordingly
   Currently checks for:
      - testing stop switch - TEST_STOP_BUTTON goes low
          -> hard stop motors

   PLEASE ADD ANY NEW INTERRUPTS TO THIS LIST
*/
void processInterrupts() {
  // testing stop switch
  if (!digitalRead(TEST_STOP_BUTTON))
  {
    // stop the bot
    hardStop();
  }
}

