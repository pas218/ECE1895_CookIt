// Chop It class


class ChopIt
{
  private:

    int numChops;
    int sensorVal;
    int requiredNumChops;

    int chopItInput;

  public:

  ChopIt(int chopItInput, int requiredNumChops);
  int runChopIt();

  void resetChopIt();
};

ChopIt::ChopIt(int chopItInput, int requiredNumChops)
{
  numChops = 0;
  sensorVal = LOW;
  chopItInput = chopItInput;
  requiredNumChops = requiredNumChops;
}

int ChopIt::runChopIt()
{
  
  while(numChops < requiredNumChops){
    // Read the value of the digital input associated with Chop-It.
    sensorVal = digitalRead(chopItInput);

    // If the digital input is low (meaning the knife has been used to complete the circuit),
    // turn off the LED to signify that the input has been read and break from the infinite loop.
    if (sensorVal == LOW){
      numChops++;
      delay(750);
    }

    sensorVal = LOW;
  }

  return 1;

}

void ChopIt::resetChopIt()
{
  numChops = 0;
}