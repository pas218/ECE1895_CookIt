#include "ADS1X15.h"
#include "mp3tf16p.h"
#include "plateIt.h"
#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif
// Welcome to Cook-It!


///// Digital Pin assignments /////

// Chop-It related.
#define chopItInput 5

// Cook-It Related (rotarty encoder).
#define cookItButton 6
#define cookItEncoderDT 7
#define cookItEncoderClk 8

// Plate-IT related
#define plateItNeoPixelLED0 0
#define plateItNeoPixelLED1 1
#define plateItNeoPixelLEDCount 8
#define plateItBottomBun 2
#define plateItTopBun 3
#define plateItBell 4

// Cook-It Initializations
 int counter = 0; 
 int currentStateDT;
 int previousStateDT; 

 MP3Player mp3(10,11);

long randNumber; 

PlateIt plateItInstance = PlateIt(plateItBottomBun, plateItTopBun, plateItBell, plateItNeoPixelLED0, plateItNeoPixelLED1, plateItNeoPixelLEDCount);

void setup() {

  mp3.initialize();

  #if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
  clock_prescale_set(clock_div_1);
  #endif

  pinMode(plateItBottomBun, INPUT);
  pinMode(plateItTopBun, INPUT);
  pinMode(plateItBell, INPUT);

  // Set encoder pins as inputs  
  pinMode (cookItEncoderClk,INPUT);
  pinMode (cookItEncoderDT,INPUT);

  // Read the initial state of inputCLK
  // Assign to previousStateCLK variable
  previousStateDT = digitalRead(cookItEncoderDT);

  pinMode(chopItInput, INPUT_PULLUP);
  pinMode(cookItButton, INPUT_PULLUP);

  randomSeed(millis());
  plateItInstance.initialize();
  
}

void loop() {
  
 int returnNumber;
mp3.playTrackNumber(3, 20);
while(true)
{
  delay(2000);

  
  randNumber = random(0, 4);
  
  // Chop it
  if (randNumber == 0){
    mp3.playTrackNumber(5, 20);
    chopIt();
    mp3.playTrackNumber(8, 20);
  }
  // Cook it
  else if (randNumber == 1){
    mp3.playTrackNumber(6, 20);
    cookIt();
    mp3.playTrackNumber(8, 20);
  }
  else{
    mp3.playTrackNumber(7, 20);
    returnNumber = plateItInstance.plateItNormal();
    if (returnNumber == 1)
    {
      mp3.playTrackNumber(8, 20);
    }
    else if (returnNumber == 2)
    {
      mp3.playTrackNumber(1, 20);
    }
    
  }

}
  
}

void chopIt() {

  // Define the sensor value.
  int sensorVal;
  
  // Number of Chops needed.
  int countChops = 0;

  while(countChops < 4){
    // Read the value of the digital input associated with Chop-It.
    sensorVal = digitalRead(chopItInput);

    // If the digital input is low (meaning the knife has been used to complete the circuit),
    // turn off the LED to signify that the input has been read and break from the infinite loop.
    if (sensorVal == LOW){
      countChops++;
      delay(750);
    }
  }

}

void cookIt(){

  // Define the button value.
  int buttonVal;
  bool continueToTurn = 0;

  while(continueToTurn == 0){
    buttonVal = digitalRead(cookItButton);
    if(buttonVal == LOW){
      continueToTurn = 1;
    }
  }
    
  while(continueToTurn == 1){

    
    
    //Serial.print("Inside.");
    //Serial.println();
    //break;

    

    // Read the current state of inputCLK
    currentStateDT = digitalRead(cookItEncoderDT);

    // If the previous and the current state of the inputCLK are different then a pulse has occured
    if (currentStateDT != previousStateDT){ 
          
        
        // If the inputDT state is same as the inputCLK state then 
        // the encoder is rotating counterclockwise
        if (digitalRead(cookItEncoderClk) == currentStateDT) { 
          counter++;
        }
      } 
      // Update previousStateCLK with the current state
      previousStateDT = currentStateDT;

      if(counter > 3){
          continueToTurn = 0;
      }
    }

    // Reset the counter for the
    counter = 0;
}
/*
void plateIt()
{
 bool roundStarts = true;
 while (roundStarts)
 {
 // Get Size of Order
 len = arraySize(order, sizeof(order));
 // Display Order on Neopixels on Digital Pin 6

 orderDisplay(order, len);
 mp3.playTrackNumber(3, 25);

 // End Once Bell is Pressed
 while(digitalRead(4) == LOW)
 {
 // Patty
 Patty = analogRead(resPin0);
 if (Patty >=400 && Patty <525 && pattyCount == 0)
 {
 playerOrder[ing] = 0;
 strip_player.setPixelColor(ing, PattyRGB);
 strip_player.show();
 ing++;
 pattyCount++;
 }
 else if (Patty >= 200 && Patty <350 && pattyCount == 1)
 {
 playerOrder[ing] = 0;
 strip_player.setPixelColor(ing, PattyRGB);
 strip_player.show();
 ing++;
 pattyCount++;
 }
 else if((Patty == 1023 && pattyCount == 1) || (Patty >=400 && Patty <525 && pattyCount == 2))
 {
 ing--;
 pattyCount--;
 playerOrder[ing] = -1;
 strip_player.setPixelColor(ing, BlankRGB);
 strip_player.show();
 }

 // Cheese
 Cheese = analogRead(resPin1);
 if (Cheese >=400 && Cheese <525 && cheeseCount == 0)
 {
 playerOrder[ing] = 1;
 strip_player.setPixelColor(ing, CheeseRGB);
 strip_player.show();
 ing++;
 cheeseCount++;
 }
 else if (Cheese >= 200 && Cheese <350 && cheeseCount == 1)
 {
 playerOrder[ing] = 1;
 strip_player.setPixelColor(ing, CheeseRGB);
 strip_player.show();
 ing++;
 cheeseCount++;
 }
 else if((Cheese == 1023 && cheeseCount == 1) || (Cheese >=400 && Cheese <525 && cheeseCount == 2))
 {
 ing--;
 cheeseCount--;
 playerOrder[ing] = -1;
 strip_player.setPixelColor(ing, BlankRGB);
 strip_player.show();
 }

 // Tomato
 Tomato = analogRead(resPin2);
 if (Tomato >=400 && Tomato <525 && tomatoCount == 0)
 {
 playerOrder[ing] = 2;
 strip_player.setPixelColor(ing, TomatoRGB);
 strip_player.show();
 ing++;
 tomatoCount++;
 }
 else if (Tomato >= 200 && Tomato <350 && tomatoCount == 1)
 {
 playerOrder[ing] = 2;
 strip_player.setPixelColor(ing, TomatoRGB);
 strip_player.show();
 ing++;
 tomatoCount++;
 }
 else if((Tomato == 1023 && tomatoCount == 1) || (Tomato >=400 && Tomato <525 && tomatoCount == 2))
 {
 ing--;
 tomatoCount--;
 playerOrder[ing] = -1;
 strip_player.setPixelColor(ing, BlankRGB);
 strip_player.show();
 }

 // Onions
 Onions = analogRead(resPin3);
 if (Onions >=400 && Onions <525 && onionCount == 0)
 {
 playerOrder[ing] = 3;
 strip_player.setPixelColor(ing, OnionRGB);
 strip_player.show();
 ing++;
 onionCount++;
 }
 else if (Onions >= 200 && Onions <350 && onionCount == 1)
 {
 playerOrder[ing] = 3;
 strip_player.setPixelColor(ing, OnionRGB);
 strip_player.show();
 ing++;
 onionCount++;
 }
 else if((Onions == 1023 && onionCount == 1) || (Onions >=400 && Onions <525 && onionCount == 2))
 {
 ing--;
 onionCount--;
 playerOrder[ing] = -1;
 strip_player.setPixelColor(ing, BlankRGB);
 strip_player.show();
 }

 // Lettuce
 Lettuce = ADS.readADC(0);
 if (Lettuce >=13300 && Lettuce <13600 && lettuceCount == 0)
 {
 playerOrder[ing] = 4;
 strip_player.setPixelColor(ing, LettuceRGB);
 strip_player.show();
 ing++;
 lettuceCount++;
 }
 else if (Lettuce >= 8800 && Lettuce <9100 && lettuceCount == 1)
 {
 playerOrder[ing] = 4;
 strip_player.setPixelColor(ing, LettuceRGB);
 strip_player.show();
 ing++;
 lettuceCount++;
 }
 else if((Lettuce > 26000 && lettuceCount == 1) || (Lettuce >=13300 && Lettuce <13600 && lettuceCount == 2))
 {
 ing--;
 lettuceCount--;
 playerOrder[ing] = -1;
 strip_player.setPixelColor(ing, BlankRGB);
 strip_player.show();
 }

 // BottomBun
 BottomBun = digitalRead(2);
 if (BottomBun == HIGH && bottomBunCount == 0)
 {
 playerOrder[ing] = 5;
 strip_player.setPixelColor(ing, BunRGB);
 strip_player.show();
 ing++;
 bottomBunCount++;
 }
 else if(BottomBun == LOW && bottomBunCount != 0)
 {
 ing--;
 bottomBunCount--;
 playerOrder[ing] = -1;
 strip_player.setPixelColor(ing, BlankRGB);
 strip_player.show();
 }

 // TopBun
 TopBun = digitalRead(3);
 if (TopBun == HIGH && topBunCount == 0)
 {
 playerOrder[ing] = 6;
 strip_player.setPixelColor(ing, BunRGB);
 strip_player.show();
 ing++;
 topBunCount++;
 }
 else if(TopBun == LOW && topBunCount != 0)
 {
 ing--;
 topBunCount--;
 playerOrder[ing] = -1;
 strip_player.setPixelColor(ing, BlankRGB);
 strip_player.show();
 }
 delay(50);
 }

 mp3.playTrackNumber(1, 25);
 delay(500);
 for (int i=0; i<len; i++)
 {
 if (playerOrder[i] == order[i])
 winFlag = 1;
 else
 {
 winFlag = 0;
 break;
 }
 }

 // Consitional Win or Lose
 //  if (winFlag == 1)

 //  else

 //  delay(1000);
 break;
 }

 // Force Player to Disassemble
 bool empty = 0;
 while (empty == 0)
 {
  // Patty
 Patty = analogRead(resPin0);
 if (Patty >=400 && Patty <525 && pattyCount == 0)
 {
 playerOrder[ing] = 0;
 strip_player.setPixelColor(ing, PattyRGB);
 strip_player.show();
 ing++;
 pattyCount++;
 }
 else if (Patty >= 200 && Patty <350 && pattyCount == 1)
 {
 playerOrder[ing] = 0;
 strip_player.setPixelColor(ing, PattyRGB);
 strip_player.show();
 ing++;
 pattyCount++;
 }
 else if((Patty == 1023 && pattyCount == 1) || (Patty >=400 && Patty <525 && pattyCount == 2))
 {
 ing--;
 pattyCount--;
 playerOrder[ing] = -1;
 strip_player.setPixelColor(ing, BlankRGB);
 strip_player.show();
 }

 // Cheese
 Cheese = analogRead(resPin1);
 if (Cheese >=400 && Cheese <525 && cheeseCount == 0)
 {
 playerOrder[ing] = 1;
 strip_player.setPixelColor(ing, CheeseRGB);
 strip_player.show();
 ing++;
 cheeseCount++;
 }
 else if (Cheese >= 200 && Cheese <350 && cheeseCount == 1)
 {
 playerOrder[ing] = 1;
 strip_player.setPixelColor(ing, CheeseRGB);
 strip_player.show();
 ing++;
 cheeseCount++;
 }
 else if((Cheese == 1023 && cheeseCount == 1) || (Cheese >=400 && Cheese <525 && cheeseCount == 2))
 {
 ing--;
 cheeseCount--;
 playerOrder[ing] = -1;
 strip_player.setPixelColor(ing, BlankRGB);
 strip_player.show();
 }

 // Tomato
 Tomato = analogRead(resPin2);
 if (Tomato >=400 && Tomato <525 && tomatoCount == 0)
 {
 playerOrder[ing] = 2;
 strip_player.setPixelColor(ing, TomatoRGB);
 strip_player.show();
 ing++;
 tomatoCount++;
 }
 else if (Tomato >= 200 && Tomato <350 && tomatoCount == 1)
 {
 playerOrder[ing] = 2;
 strip_player.setPixelColor(ing, TomatoRGB);
 strip_player.show();
 ing++;
 tomatoCount++;
 }
 else if((Tomato == 1023 && tomatoCount == 1) || (Tomato >=400 && Tomato <525 && tomatoCount == 2))
 {
 ing--;
 tomatoCount--;
 playerOrder[ing] = -1;
 strip_player.setPixelColor(ing, BlankRGB);
 strip_player.show();
 }

 // Onions
 Onions = analogRead(resPin3);
 if (Onions >=400 && Onions <525 && onionCount == 0)
 {
 playerOrder[ing] = 3;
 strip_player.setPixelColor(ing, OnionRGB);
 strip_player.show();
 ing++;
 onionCount++;
 }
 else if (Onions >= 200 && Onions <350 && onionCount == 1)
 {
 playerOrder[ing] = 3;
 strip_player.setPixelColor(ing, OnionRGB);
 strip_player.show();
 ing++;
 onionCount++;
 }
 else if((Onions == 1023 && onionCount == 1) || (Onions >=400 && Onions <525 && onionCount == 2))
 {
 ing--;
 onionCount--;
 playerOrder[ing] = -1;
 strip_player.setPixelColor(ing, BlankRGB);
 strip_player.show();
 }

 // Lettuce
 Lettuce = ADS.readADC(0);
 if (Lettuce >=13300 && Lettuce <13600 && lettuceCount == 0)
 {
 playerOrder[ing] = 4;
 strip_player.setPixelColor(ing, LettuceRGB);
 strip_player.show();
 ing++;
 lettuceCount++;
 }
 else if (Lettuce >= 8800 && Lettuce <9100 && lettuceCount == 1)
 {
 playerOrder[ing] = 4;
 strip_player.setPixelColor(ing, LettuceRGB);
 strip_player.show();
 ing++;
 lettuceCount++;
 }
 else if((Lettuce > 26000 && lettuceCount == 1) || (Lettuce >=13300 && Lettuce <13600 && lettuceCount == 2))
 {
 ing--;
 lettuceCount--;
 playerOrder[ing] = -1;
 strip_player.setPixelColor(ing, BlankRGB);
 strip_player.show();
 }

 // BottomBun
 BottomBun = digitalRead(2);
 if (BottomBun == HIGH && bottomBunCount == 0)
 {
 playerOrder[ing] = 5;
 strip_player.setPixelColor(ing, BunRGB);
 strip_player.show();
 ing++;
 bottomBunCount++;
 }
 else if(BottomBun == LOW && bottomBunCount != 0)
 {
 ing--;
 bottomBunCount--;
 playerOrder[ing] = -1;
 strip_player.setPixelColor(ing, BlankRGB);
 strip_player.show();
 }

 // TopBun
 TopBun = digitalRead(3);
 if (TopBun == HIGH && topBunCount == 0)
 {
 playerOrder[ing] = 6;
 strip_player.setPixelColor(ing, BunRGB);
 strip_player.show();
 ing++;
 topBunCount++;
 }
 else if(TopBun == LOW && topBunCount != 0)
 {
 ing--;
 topBunCount--;
 playerOrder[ing] = -1;
 strip_player.setPixelColor(ing, BlankRGB);
 strip_player.show();
 }

 if (ing == 0)
 empty = 1;
 delay(50);
 }

 // Reset Parameters
 pattyCount = 0;
 cheeseCount = 0;
 tomatoCount = 0;
 onionCount = 0;
 lettuceCount = 0;
 bottomBunCount = 0;
 topBunCount = 0;
 winFlag = 1;
 strip_order.clear();
 strip_order.show();
 strip_player.clear();
 strip_player.show();
 delay(1000);
}

int arraySize(int arr[], int arrSize)
{
 int len = arrSize / sizeof(arr[0]);
 return len;
}

void orderDisplay(int arr[], int len)
{
 for (int i=0; i<len; i++)
 {
 int ing = arr[i];
 switch (ing)
 {
 // Patty
 case 0:
 strip_order.setPixelColor(i, PattyRGB);
 strip_order.show();
 break;

 // Cheese
 case 1:
 strip_order.setPixelColor(i, CheeseRGB);
 strip_order.show();
 break;

 // Tomato
 case 2:
 strip_order.setPixelColor(i, TomatoRGB);
 strip_order.show();
 break;

 // Onion
 case 3:
 strip_order.setPixelColor(i, OnionRGB);
 strip_order.show();
 break;

 // Lettuce
 case 4:
 strip_order.setPixelColor(i, LettuceRGB);
 strip_order.show();
 break;

 // BottomBun
 case 5:
 strip_order.setPixelColor(i, BunRGB);
 strip_order.show();
 break;

 // TopBun
 case 6:
 strip_order.setPixelColor(i, BunRGB);
 strip_order.show();
 break;
 }
 delay(1000);
 }
}*/
