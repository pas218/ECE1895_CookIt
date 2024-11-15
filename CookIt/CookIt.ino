#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "ADS1X15.h"
#include "mp3tf16p.h"
#include "chopIt.h"
#include "cookIt.h"
#include "plateIt.h"
#define i2c_Address 0x3C
#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif
// Welcome to Cook-It!

#define WIRE Wire
///// ADAFRUIT Screen Assignments /////
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels


///// Digital Pin assignments /////

// Chop-It related inputs.
#define chopItInput 5
#define requiredNumChops 4

// Cook-It Related (rotarty encoder) inputs.
#define cookItButton 6
#define cookItEncoderDT 7
#define cookItEncoderClk 8
#define requiredCookItMoves 10

// Plate-It related inputs.
#define plateItNeoPixelLED0 2       // Order
#define plateItNeoPixelLED1 1       // Player
#define plateItNeoPixelLEDCount 8
#define plateItBottomBun 3
#define plateItTopBun 4
#define plateItmp3_bell 12

//Sound effects
#define mp3_bell 1
#define mp3_introChord 3
#define mp3_callChopIt 5
#define mp3_callCookIt 6
#define mp3_callPlateIt 7
#define mp3_sayGoodJob 8
#define mp3_sayYouSuck 9

// Cook-It Initializations
int counter = 0;
int currentStateDT;
int previousStateDT;

// Timestamps for keeping track of time.
MP3Player mp3(10,11);


long randNumber; 

ChopIt chopItInstance   = ChopIt(chopItInput, requiredNumChops);
CookIt cookItInstance   = CookIt(cookItButton, cookItEncoderDT, cookItEncoderClk);
PlateIt plateItInstance = PlateIt(plateItBottomBun, plateItTopBun, plateItmp3_bell, plateItNeoPixelLED0, plateItNeoPixelLED1, plateItNeoPixelLEDCount);


// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display = Adafruit_SSD1306(128, 32, &WIRE);


bool right = false;
int leaveLoop = 0;
int returnNumber = 0;
int totalScore = 0;



unsigned long startTime = 0;
unsigned long endTime = 0;
unsigned long timePerFunction = 10000;
bool tooLong = false;


void setup() {


  ///// SCREEN INITIALIZATIONS /////
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C); // Address 0x3C for 128x32
  display.display();
  delay(1000);

  // Clear the buffer.
  display.clearDisplay();
  display.display();

  // text display tests
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.display(); // actually display all of the above
  ///// END SCREEN INITIALIZATIONS /////


  #if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
  clock_prescale_set(clock_div_1);
  #endif


  ///// DEFINE CHOP-IT INPUTS /////
  pinMode(chopItInput, INPUT);

  ///// DEFINE COOK-iT INPUTS /////
  pinMode (cookItEncoderClk,INPUT);
  pinMode (cookItEncoderDT,INPUT);
  pinMode(cookItButton, INPUT);

  ///// DEFINE PLATE-IT INPUTS /////
  pinMode(plateItBottomBun, INPUT);
  pinMode(plateItTopBun, INPUT);
  pinMode(plateItmp3_bell, INPUT);

  ///// MP3 INITIALIZATION
  mp3.initialize();

  ///// GENERATE RANDOM SEED /////
  randomSeed(analogRead(0));

  ///// INITIALIZE PLATE-IT /////
  plateItInstance.initialize();
  
  
}

void loop() {
  

  // Play introductory chord.
  mp3.playTrackNumber(mp3_introChord, 20);
  delay(100);

  while(true)
  {
    delay(500);

    ///// DISPLAY SCORE /////
    // Clear the buffer.
    display.clearDisplay();
    display.display();
    display.setCursor(0,0);
    display.print(totalScore);
    display.setCursor(0,0);
    display.display(); // actually display all of the above

    randNumber = random(0, 2);
    //randNumber =  2;

    leaveLoop = 0;
    right = false;
    returnNumber = 0;
    tooLong = false;

    // Get starting time to compare time passed later.
    startTime = millis();

    //////////////////// CHOP IT ////////////////////
    if (randNumber == 0){

      
      mp3.playTrackNumber(mp3_callChopIt, 15);
      delay(100);

      while (leaveLoop == 0){
        
        returnNumber = chopItInstance.runChopIt();
        if (returnNumber >= requiredNumChops){
          right = true;
          leaveLoop = 1;
        }
        delay(50);

        endTime = millis();
        if ((endTime-startTime) >= timePerFunction){
          right = false;
          leaveLoop = 1;
        }
    }

      if (right == true){
        
        mp3.playTrackNumber(mp3_sayGoodJob, 15);
      }
      else{
        mp3.playTrackNumber(mp3_sayYouSuck, 15);
      }
      
      delay(1000);
    }


    //////////////////// COOK IT ////////////////////
    else if (randNumber == 1){
      
      mp3.playTrackNumber(mp3_callCookIt, 15);
      delay(100);

      // First check for button input
      while (leaveLoop == 0){
        returnNumber = cookItInstance.runCookItButton();
        if (returnNumber >= 1){
          leaveLoop = 1;
        }
        delay(50);
        endTime = millis();
        if ((endTime-startTime) >= timePerFunction){
          right = false;
          leaveLoop = 1;
          tooLong = true;
          
        }
      }

      // Second check for encoder input
      leaveLoop = 0;
      while ((leaveLoop == 0) && (tooLong == false)){
        returnNumber = cookItInstance.runCookItEncoder();
        if (returnNumber >= requiredCookItMoves){
          leaveLoop = 1;
          right = true;
        }
        endTime = millis();
        if ((endTime-startTime) >= timePerFunction){
          right = false;
          leaveLoop = 1;
          tooLong = true;
          
        }
      }


      if (right == true){
        mp3.playTrackNumber(mp3_sayGoodJob, 15);
      }
      else{
        mp3.playTrackNumber(mp3_sayYouSuck, 15);
      }
      delay(1000);
      
    }
    //////////////////// PLATE IT ////////////////////
    else{

      mp3.playTrackNumber(mp3_callPlateIt, 15);
      delay(100);

      while (leaveLoop == 0){
        returnNumber = plateItInstance.plateItNormal();
        // Ingredient placed
        if (returnNumber != 0)
        {
          right = true;
          leaveLoop = 1;
        }

        endTime = millis();
        if ((endTime-startTime) >= timePerFunction){
          right = false;
          leaveLoop = 1;
        }
      }

      if (right == false){
        mp3.playTrackNumber(mp3_sayYouSuck, 15);
      }
      //regular ingredient places
      else if (returnNumber == 1){
        mp3.playTrackNumber(mp3_sayGoodJob, 15);
      }
      // mp3_bell rang
      else if (returnNumber == 2)
      {
        mp3.playTrackNumber(mp3_bell, 15);
      }
      delay(1000);  
    }

    cookItInstance.resetCookIt();
    chopItInstance.resetChopIt();
    
    // Increase total score.
    totalScore++;
  }
  //// END GAME WHILE LOOP //
  
}

