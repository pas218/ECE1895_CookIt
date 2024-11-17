// Chop It class

class ChopIt
{
  private:

    // State variables.
    int previousValue;
    int chopItInput;
    int returnVal;

  public:

    ChopIt(int chopItInput);
    int runChopIt();

    void resetChopIt();
};

ChopIt::ChopIt(int chopItInput)
{
  returnVal = 0;
  previousValue = HIGH;
}

int ChopIt::runChopIt()
{

  // Increase the number of chops if the knife went from "unchopped" to "chopped."
  if ((digitalRead(5) == LOW) && (previousValue == HIGH)){

      returnVal++;; 
  }

  previousValue = digitalRead(5);

  return returnVal;

}

void ChopIt::resetChopIt()
{
  returnVal = 0;
}