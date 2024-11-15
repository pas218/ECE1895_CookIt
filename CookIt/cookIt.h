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

    bool continueToTurn;
    int returnVal;

  public:

    CookIt(int cookItButton, int cookItEncoderDT, int cookItEncoderClk);
    int runCookIt();
    void resetCookIt();
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

  returnVal = 0;
  continueToTurn = 0;

}

int CookIt::runCookIt()
{

  
  buttonVal = digitalRead(cookItButtonInput);
  if(buttonVal == LOW){
    continueToTurn = 1;
    returnVal = 1;
  }
    
    if (continueToTurn == 1){

      for(int h = 0; h < 200; h++){
      // Read the current state of inputCLK
      currentStateDT = digitalRead(cookItEncoderDTInput);

      // If the previous and the current state of the inputCLK are different then a pulse has occured
      if (currentStateDT != previousStateDT){ 
          
        
        // If the inputDT state is same as the inputCLK state then 
        // the encoder is rotating counterclockwise
        if (digitalRead(cookItEncoderClkInput) == currentStateDT) { 
          returnVal++;
        }
      } 
      // Update previousStateCLK with the current state
      previousStateDT = currentStateDT;
    }
    

 }

  return returnVal;
}

void CookIt::resetCookIt(){
  continueToTurn = 0;
  returnVal = 0;