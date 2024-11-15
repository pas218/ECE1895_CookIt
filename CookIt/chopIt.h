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

    //void resetChopIt();
};

ChopIt::ChopIt(int chopItInput, int requiredNumChops)
{
  //numChops = 0;
  //chopItInput = 5;
  //requiredNumChops = requiredNumChops;
}

int ChopIt::runChopIt()
{
  
  int returnVal;
  //while(true){
    // Read the value of the digital input associated with Chop-It.
    //sensorVal = digitalRead(chopItInput);
    //sensorVal = HIGH;
    // If the digital input is low (meaning the knife has been used to complete the circuit),
    // turn off the LED to signify that the input has been read and break from the infinite loop.
    if (digitalRead(5) == LOW){
      //numChops++;
      //delay(750);
      
      //if (numChops >= requiredNumChops){
        //break;
      //}
       returnVal = 1; 
    }
    else{
      returnVal = 0;
    }

  //}

  return returnVal;

}
/*
void ChopIt::resetChopIt()
{
  numChops = 0;
}*/