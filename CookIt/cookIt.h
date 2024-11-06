// Cook It class


class CookIt
{
  private:
    
    int buttonVal;
    int numTurns;
    int currentStateDT;
    int previousStateDT;

    int cookItButtonInput;
    int cookItEncoderDTInput;
    int cookItEncoderClkInput;

  public:

    CookIt(int cookItButton, int cookItEncoderDT, int cookItEncoderClk);
    int runCookIt();
};


CookIt::CookIt(int cookItButton, int cookItEncoderDT, int cookItEncoderClk)
{
  // Read the initial state of inputDT
  // Assign to previousStateDT variable
  previousStateDT = digitalRead(cookItEncoderDT);
  numTurns = 0;

  cookItButtonInput = cookItButton;
  cookItEncoderDTInput = cookItEncoderDT;
  cookItEncoderClkInput = cookItEncoderClk;
}

int CookIt::runCookIt()
{
  bool continueToTurn = 0;

  while(continueToTurn == 0){
    buttonVal = digitalRead(cookItButtonInput);
    if(buttonVal == LOW){
      continueToTurn = 1;
    }
  }
    
  while(continueToTurn == 1){


    // Read the current state of inputCLK
    currentStateDT = digitalRead(cookItEncoderDTInput);

    // If the previous and the current state of the inputCLK are different then a pulse has occured
    if (currentStateDT != previousStateDT){ 
          
        
        // If the inputDT state is same as the inputCLK state then 
        // the encoder is rotating counterclockwise
        if (digitalRead(cookItEncoderClkInput) == currentStateDT) { 
          numTurns++;
        }
      } 
      // Update previousStateCLK with the current state
      previousStateDT = currentStateDT;

      if(numTurns > 3){
          continueToTurn = 0;
      }
    }

    // Reset the counter for the
    numTurns = 0;
}