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
      chopItInstance.runChopIt();
      mp3.playTrackNumber(8, 20);
    }
    // Cook it
    else if (randNumber == 1){
      mp3.playTrackNumber(6, 20);
      cookItInstance.runCookIt();
      mp3.playTrackNumber(8, 20);
    }
    // Plate It
    else{
      mp3.playTrackNumber(7, 20);
      returnNumber = plateItInstance.plateItNormal();
      // Ingredient placed
      if (returnNumber == 1)
      {
        mp3.playTrackNumber(8, 20);
      }
      // Bell rang
      else if (returnNumber == 2)
      {
        mp3.playTrackNumber(1, 20);
      }
      
    }

  }
  
}

