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
    int cookItButtonReturn;
    int cookItEncoderReturn;

  public:

    CookIt(int cookItButton, int cookItEncoderDT, int cookItEncoderClk);
    int cookItButton();
    int runCookItButton();
    int runCookItEncoder();
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

  cookItEncoderReturn = 0;
  continueToTurn = 0;
  cookItButtonReturn = 0;

}

int CookIt::runCookItButton()
{
  cookItButtonReturn = 0;

  buttonVal = digitalRead(cookItButtonInput);
  if(buttonVal == LOW){
    cookItButtonReturn = 1;
  }

  return cookItButtonReturn;
}

int CookIt::runCookItEncoder()
{

 

  // Read the current state of inputCLK
  currentStateDT = digitalRead(cookItEncoderDTInput);

  // If the previous and the current state of the inputCLK are different then a pulse has occured
  if (currentStateDT != previousStateDT){ 
      
    
    // If the inputDT state is same as the inputCLK state then 
    // the encoder is rotating counterclockwise
    if (digitalRead(cookItEncoderClkInput) == currentStateDT) { 
      cookItEncoderReturn++;
    }
  } 
  // Update previousStateCLK with the current state
  previousStateDT = currentStateDT;


  return cookItEncoderReturn;
}

void CookIt::resetCookIt(){
  cookItButtonReturn = 0;
  cookItEncoderReturn = 0;
}