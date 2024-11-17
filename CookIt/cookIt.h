// Cook It class


class CookIt
{
  private:
    
    // State variables. 
    int buttonVal;
    int numTurns;

    // Variables to keep track of encoder.
    int currentStateDT;
    int previousStateDT;

    // Digital pin input assignemnt.
    int cookItButtonInput;
    int cookItEncoderDTInput;
    int cookItEncoderClkInput;

    // Return variables.
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

  // Assign digital inputs.
  cookItButtonInput = cookItButton;
  cookItEncoderDTInput = cookItEncoderDT;
  cookItEncoderClkInput = cookItEncoderClk;

  // Zero all return variables.
  cookItEncoderReturn = 0;
  cookItButtonReturn = 0;

}

// Function to detect that the Cook-it button has been pressed
// If pressed, return 1
// return 0 otherwise
int CookIt::runCookItButton()
{
  cookItButtonReturn = 0;

  buttonVal = digitalRead(cookItButtonInput);
  if(buttonVal == LOW){
    cookItButtonReturn = 1;
  }

  return cookItButtonReturn;
}

// Function to detect the number of turns made by Cook-It encoder.
// Returns the current number of clockwise "ticks" detected since last reset.
// Counter clockwise ticks do not register.
int CookIt::runCookItEncoder()
{

  // Read the current state of inputDT
  currentStateDT = digitalRead(cookItEncoderDTInput);

  // If the previous and the current state of the inputDT are different then a pulse has occured
  if (currentStateDT != previousStateDT){ 
      
    
    // If the inputDT state is same as the inputCLK state then 
    // the encoder is rotating clockwise
    if (digitalRead(cookItEncoderClkInput) == currentStateDT) { 
      cookItEncoderReturn++;
    }
  } 
  // Update previousStateDT with the current state
  previousStateDT = currentStateDT;


  return cookItEncoderReturn;
}

//Reset.
void CookIt::resetCookIt(){
  cookItButtonReturn = 0;
  cookItEncoderReturn = 0;
}