#include "ADS1X15.h"
#include "mp3tf16p.h"
#include "chopIt.h"
#include "cookIt.h"
#include "plateIt.h"
#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif
// Welcome to Cook-It!


///// Digital Pin assignments /////

// Chop-It related.
#define chopItInput 5
#define requiredNumChops 4

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

ChopIt chopItInstance   = ChopIt(chopItInput, requiredNumChops);
CookIt cookItInstance   = CookIt(cookItButton, cookItEncoderDT, cookItEncoderClk);
PlateIt plateItInstance = PlateIt(plateItBottomBun, plateItTopBun, plateItBell, plateItNeoPixelLED0, plateItNeoPixelLED1, plateItNeoPixelLEDCount);

int right = 0;
int leaveLoop = 0;
int returnNumber = 0


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

  pinMode(5, INPUT_PULLUP);
  pinMode(cookItButton, INPUT_PULLUP);

  randomSeed(analogRead(0));
  plateItInstance.initialize();
  
}

void loop() {
  


  mp3.playTrackNumber(1, 20);


  while(true)
  {
    delay(2000);

    //randNumber = random(0, 4);
    randNumber = 0;
    leaveLoop = 0;

    // Chop it
    if (randNumber == 0){
      mp3.playTrackNumber(5, 15);
      returnNumber = 0;
      delay(100);
      while (leaveLoop == 0){
        /*
        returnNumber = chopItInstance.runChopIt();
        if (returnNumber == 1){
          right = 1;
          leaveLoop = 1;
          break;
        }*/
        delay(50);
        returnNumber = cookItInstance.runCookIt();
        if (returnNumber == 1){
            cookItInstance.resetCookIt();
            right = 0;
            leaveLoop = 1;
            break;
        }
        /*
        returnNumber = plateItInstance.plateItNormal();
        if (returnNumber != 0){
          right = 0;
          leaveLoop = 1;
          break;
        }*/
      }
      delay(150);
      if (right == 1){
        
        mp3.playTrackNumber(8, 15);
      }
      else{
        mp3.playTrackNumber(9, 15);
      }
    }
    // Cook it
    else if (randNumber == 1){
      mp3.playTrackNumber(6, 15);
      cookItInstance.runCookIt();
      mp3.playTrackNumber(8, 15);
    }
    // Plate It
    else{
      mp3.playTrackNumber(7, 15);
      returnNumber = plateItInstance.plateItNormal();
      // Ingredient placed
      if (returnNumber == 1)
      {
        mp3.playTrackNumber(8, 15);
      }
      // Bell rang
      else if (returnNumber == 2)
      {
        mp3.playTrackNumber(1, 15);
      } 
    }

    //chopItInstance.resetChopIt();
  }
}

