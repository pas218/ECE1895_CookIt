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



/////// Welcome to Cook-It! /////


///// ADAFRUIT Screen Assignments /////
#define WIRE Wire
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


// Needed classes.
MP3Player mp3(10,11);  //MP3 Player
ChopIt chopItInstance   = ChopIt(chopItInput, requiredNumChops);
CookIt cookItInstance   = CookIt(cookItButton, cookItEncoderDT, cookItEncoderClk);
PlateIt plateItInstance = PlateIt(plateItBottomBun, plateItTopBun, plateItmp3_bell, plateItNeoPixelLED0, plateItNeoPixelLED1, plateItNeoPixelLEDCount);
Adafruit_SSD1306 display = Adafruit_SSD1306(128, 32, &WIRE);  // OLED


// Used for generating random numbers.
long randNumber; 


// Variables for program control. 
bool right = false;
int leaveLoop = 0;
int returnNumber = 0;
int totalScore = 0;


// Variables for keeping track of time (mostly using the millis() function).
unsigned long startTime = 0;
unsigned long endTime = 0;
unsigned long timePerFunction = 10000;   // This is the amound of time in miliseconds that a player is allowed to take to complete an input.
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
  display.display();     // This will end up being blank.
  ///// END SCREEN INITIALIZATIONS /////

  // Not 100% sure what this is but I think we need it.
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
  

  // Play introductory chord (there needs to be a delay after every MP3)
  mp3.playTrackNumber(mp3_introChord, 20);
  delay(100);

  // Enter while loop to start the game.
  while(true)
  {
    delay(500);

    // Display score.
    display.clearDisplay();
    display.display();
    display.setCursor(0,0);
    display.print(totalScore);
    display.setCursor(0,0);
    display.display(); 

    // 0 = Chop-It, 1 = Cook-it, 2 = Plate-it
    randNumber = random(0, 2);
    //randNumber =  2;

    // Initialize all control variables to base values.
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

      // Check for Chop-It input and time spent waiting for input.
      // Exits if either 1) the correct Chop-It input is recieved or
      // 2) The user takes too long to "Chop-It"
      while (leaveLoop == 0){
        
        // Run chop it. The return value will be the current number of chops that have been done.
        returnNumber = chopItInstance.runChopIt();
        if (returnNumber >= requiredNumChops){
          right = true;
          leaveLoop = 1;
        }
        delay(50);

        // Measure the current time and determing if the user is taking too long to complete the action.
        endTime = millis();
        if ((endTime-startTime) >= timePerFunction){
          right = false;
          leaveLoop = 1;
        }
      }


      // After exiting the while loop, determine which sound effect to play, depending
      // on how the player performed previously.
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


      // First check for button input from the encoder. The program will leave this loop under 2 conditions:
      // 1) The player successfully press the encoder button or
      // 2) The player has spent too long doing an input
      while (leaveLoop == 0){
        // Read if the button has been pressed. Return 1 if there is a press, 0 otherwise.
        returnNumber = cookItInstance.runCookItButton();
        if (returnNumber >= 1){
          leaveLoop = 1;
        }
        delay(50);
        // Measure the current time and determing if the user is taking too long to complete the action.
        endTime = millis();
        if ((endTime-startTime) >= timePerFunction){
          right = false;
          leaveLoop = 1;
          tooLong = true;
          
        }
      }

      // Second check for encoder "rotary" input. The program will leave this loop under 2 conditions:
      // 1) The player successfully turned the encoder clockwise the proper amount of turns.
      // 2) The player has spent too long doing an input
      leaveLoop = 0;
      while ((leaveLoop == 0) && (tooLong == false)){
        // Check the number of "ticks" that have been made. The return value will be the number of ticks.
        returnNumber = cookItInstance.runCookItEncoder();
        if (returnNumber >= requiredCookItMoves){
          leaveLoop = 1;
          right = true;
        }

        // Measure the current time and determing if the user is taking too long to complete the action.
        endTime = millis();
        if ((endTime-startTime) >= timePerFunction){
          right = false;
          leaveLoop = 1;
          tooLong = true;
        }
      }


      // After exiting the while loop, determine which sound effect to play, 
      // depending on how the player performed previously.
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

       // Check for Plate-It input and time spent waiting for input. Exits if:
      // 1) The player has made 1 input on Plate-It (EX: place lettuce, remove burger, or ring bell)
      // 2) The user takes too long to "Chop-It"
      while (leaveLoop == 0){
        // Run playe it. The return number will be 1 if a general input has occured (EX: burger placed).
        // The return will be 2 if the bell has been rung. Otherwise the return will be 0.
        returnNumber = plateItInstance.plateItNormal();
        if (returnNumber != 0)
        {
          right = true;
          leaveLoop = 1;
        }

        // Measure the current time and determing if the user is taking too long to complete the action.
        endTime = millis();
        if ((endTime-startTime) >= timePerFunction){
          right = false;
          leaveLoop = 1;
        }
      }

      // After exiting the while loop, determine which sound effect to play, 
      // depending on how the player performed previously.
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


    // Reset all internal values of Cook-It and Chop-It.
    cookItInstance.resetCookIt();
    chopItInstance.resetChopIt();
    
    // Increase total score.
    totalScore++;
  }
  //// END GAME WHILE LOOP //
  
}

