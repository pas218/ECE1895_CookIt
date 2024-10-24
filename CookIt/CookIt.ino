// Define Chop-It Inputs;
#define inputChopIt 5

// Rotary Encoder Inputs
 #define inputCLK 0
 #define inputDT 7

  // LED Outputs
 #define ledCW 8
 #define ledCCW 9

 int counter = 0; 
 int currentStateDT;
 int previousStateDT; 

typedef struct
{
     int analogInputNumber;
     float expectVoltage;
}ingredient;

ingredient Burger  = {0, 1};
ingredient Lettuce = {0, 1};
ingredient Cheese  = {0, 1};
ingredient Onion   = {0, 1};

long randNumber; 

void setup() {

  // Set encoder pins as inputs  
  pinMode (inputCLK,INPUT);
  pinMode (inputDT,INPUT);

  // Read the initial state of inputCLK
  // Assign to previousStateCLK variable
  previousStateDT = digitalRead(inputDT);

  pinMode(inputChopIt, INPUT_PULLUP);

  randomSeed(analogRead(0));

  // Setup Serial Monitor
   Serial.begin (9600);
}

void loop() {
  
  delay(2000);

  randNumber = random(0, 2);
  // Chop it
  if (randNumber == 0){
    Serial.print("Chop-It!");
    Serial.println();
    chopIt();
  }
  // Cook it
  else{
    Serial.print("Cook-It!");
    Serial.println();
    cookIt();
  }
}

void chopIt() {

  // Define the sensor value.
  int sensorVal;

  while(true){
    // Read the value of the digital input associated with Chop-It.
    sensorVal = digitalRead(inputChopIt);

    // If the digital input is low (meaning the knife has been used to complete the circuit),
    // turn off the LED to signify that the input has been read and break from the infinite loop.
    if (sensorVal == LOW){
      Serial.print("Chop-It recieved.");
      Serial.println();
      break;
    }
  }

}

void cookIt(){

  

  while(true){

    // Read the current state of inputCLK
    currentStateDT = digitalRead(inputDT);

    // If the previous and the current state of the inputCLK are different then a pulse has occured
    if (currentStateDT != previousStateDT){ 
          
        // If the inputDT state is same as the inputCLK state then 
        // the encoder is rotating counterclockwise
        if (digitalRead(inputCLK) == currentStateDT) { 
          counter++; 
        }
        else {
          counter--;
        }
      } 
      // Update previousStateCLK with the current state
      previousStateDT = currentStateDT;

      if(counter > 7){
          Serial.print("Cook-It recieved.");
          Serial.println();
          break;
      }
    }

    // Reset the counter for the
    counter = 0;
}

void assembleIt(){

}