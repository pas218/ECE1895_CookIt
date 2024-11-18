#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "ADS1X15.h"
#include "mp3tf16p.h"
#include "chopIt.h"
#include "cookIt.h"
#include "plateIt.h"
#include "misc.h"

#ifdef __AVR__
#include <avr/power.h>  // Required for 16 MHz Adafruit Trinket
#endif



/////// Welcome to Cook-It! /////


///// ADAFRUIT Screen Assignments /////
#define WIRE Wire
#define SCREEN_WIDTH 128  // OLED display width, in pixels
#define SCREEN_HEIGHT 64  // OLED display height, in pixels


///// Digital Pin assignments /////

// Chop-It related inputs.
#define chopItInput 5
#define requiredNumChops 4

// Cook-It Related (rotarty encoder) inputs.
#define cookItButton 6
#define cookItEncoderDT 7
#define cookItEncoderClk 8
#define requiredCookItMoves 5

// Plate-It related inputs.
#define plateItNeoPixelLED0 2  // Order
#define plateItNeoPixelLED1 1  // Player
#define plateItNeoPixelLEDCount 8
#define plateItBottomBun 3
#define plateItTopBun 4
#define plateItmp3_bell 12

// Misc Inputs
#define startButtonInput 9


//Sound effects
#define mp3_bell 1
#define mp3_sayPressStartToProceed 2
#define mp3_introChord 3
#define mp3_callChopIt 4
#define mp3_callCookIt 5
#define mp3_callPlateIt 6
#define mp3_disassembleBurger 7
#define mp3_faster 8
#define mp3_lose 9
#define mp3_win 10
#define mp3_sayGoodJob 11
#define mp3_sayYouSuck 12



// Cook-It Initializations
int counter = 0;
int currentStateDT;
int previousStateDT;

// Beginning variables
int startScore = 0;
int startLives = 3;

// Needed classes.
MP3Player mp3(10, 11);  //MP3 Player
ChopIt chopItInstance = ChopIt(chopItInput);
CookIt cookItInstance = CookIt(cookItButton, cookItEncoderDT, cookItEncoderClk);
PlateIt plateItInstance = PlateIt(plateItBottomBun, plateItTopBun, plateItmp3_bell, plateItNeoPixelLED0, plateItNeoPixelLED1, plateItNeoPixelLEDCount);
Misc misc = Misc(startScore, startLives, startButtonInput);
Adafruit_SSD1306 display = Adafruit_SSD1306(128, 32, &WIRE);  // OLED


// Used for generating random numbers.
long randNumber;


// Variables for program control.
int increment = 0;
int leaveLoop = 0;
int returnNumber = 0;
int totalScore = 0;
int totalLives = 0;
bool correctOrder = false;
int burgerSize = 3;
bool playGame = false;
bool returnBool = false;
bool goToFunction = false;
int wrong = false;
int ha = false;
bool skip = false;

// Variables for keeping track of time (mostly using the millis() function).
unsigned long startTime = 0;
unsigned long endTime = 0;
unsigned long timePerFunctionChopIt = 15000;  // This is the amound of time in miliseconds that a player is allowed to take to complete an input.
unsigned long timePerFunctionCookIt = 15000;
unsigned long timePerFunctionPlateIt = 15000;
unsigned long timePerFunctionDisassembleIt = 15000;
bool tooLong = false;


void setup() {


  ///// SCREEN INITIALIZATIONS /////
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // Address 0x3C for 128x32
  display.display();
  delay(1000);

  // Clear the buffer.
  display.clearDisplay();
  display.display();

  // text display tests
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.display();  // This will end up being blank.
///// END SCREEN INITIALIZATIONS /////

// Not 100% sure what this is but I think we need it.
#if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
  clock_prescale_set(clock_div_1);
#endif


  ///// DEFINE CHOP-IT INPUTS /////
  pinMode(chopItInput, INPUT);

  ///// DEFINE COOK-iT INPUTS /////
  pinMode(cookItEncoderClk, INPUT);
  pinMode(cookItEncoderDT, INPUT);
  pinMode(cookItButton, INPUT);

  ///// DEFINE PLATE-IT INPUTS /////
  pinMode(plateItBottomBun, INPUT);
  pinMode(plateItTopBun, INPUT);
  pinMode(plateItmp3_bell, INPUT);

  ///// DEFINE MISC INPUTS /////
  pinMode(startButtonInput, INPUT);

  ///// MP3 INITIALIZATION
  mp3.initialize();

  ///// GENERATE RANDOM SEED /////
  randomSeed(analogRead(0));

  ///// INITIALIZE PLATE-IT /////
  plateItInstance.initialize();
}

void loop() {


  mp3.playTrackNumber(mp3_sayPressStartToProceed, 25);
  //mp3.playTrackNumber(2, 25);
  delay(1000);

  while(!playGame){
    if (misc.startButtonPressed() == true){
      playGame = true;
    }
  }

  // Play introductory chord (there needs to be a delay after every MP3)
  mp3.playTrackNumber(mp3_introChord, 30);
  delay(500);

  misc.resetScore();
  misc.resetLives();

  // Get starting information.
  // Get total score and set burger size.
  totalScore = misc.getScore();
  if ((totalScore >= 0) && (totalScore < 20)) {
    burgerSize = 3;
  } else if ((totalScore >= 20) && (totalScore < 40)) {
    burgerSize = 4;
  } else if ((totalScore >= 40) && (totalScore < 60)) {
    burgerSize = 5;
  } else if ((totalScore >= 60) && (totalScore < 80)) {
    burgerSize = 6;
  } else if (totalScore >= 80) {
    burgerSize = 7;
  }

  
  // Display score.
  totalLives = misc.getLives();
  display.clearDisplay();
  display.display();
  display.setCursor(0, 0);
  display.print("Score: ");
  display.println(totalScore);
  display.print(totalLives);
  display.setCursor(0, 0);
  display.display();

  // Enter while loop to start the game.
  while (playGame) {

    delay(500);

    

    // 0-19 = Chop-It, 20-39 = Cook-it, 40-59 = Plate-it
    randNumber = random(0, 60);
    //randNumber =  50;

    // Initialize all control variables to base values.
    leaveLoop = 0;
    increment = 0;
    returnNumber = 0;
    tooLong = false;
    goToFunction = false;
    returnBool = false;
    wrong = 0;
    ha = 0;
    skip = false;

    // Get starting time to compare time passed later.
    startTime = millis();


    //////////////////// CHOP IT ////////////////////
    if ((randNumber >= 0) && (randNumber < 20)) {

      mp3.playTrackNumber(mp3_callChopIt, 25);
      delay(2000);

      // Check for Chop-It input and time spent waiting for input.
      // Exits if either 1) the correct Chop-It input is recieved or
      // 2) The user takes too long to "Chop-It"
      while (leaveLoop == 0) {
        
        // Read if the button has been pressed. Return 1 if there is a press, 0 otherwise.
        ha = cookItInstance.runCookItButton();
        if (ha >= 1 && leaveLoop == 0) {
          leaveLoop = 1;
          increment = 0;
        }
        delay(50);

        wrong = plateItInstance.plateItNormal();
        if (wrong == 2 && leaveLoop == 0) {
          increment = 0;
          leaveLoop = 1;
        }
        delay(50);

        
        // Run chop it. The return value will be the current number of chops that have been done.
        returnNumber = chopItInstance.runChopIt();
        if (returnNumber >= requiredNumChops && leaveLoop == 0) {
          increment = 1;
          leaveLoop = 1;
        }
        
        
        delay(50);

        // Measure the current time and determing if the user is taking too long to complete the action.
        endTime = millis();
        if ((endTime - startTime) >= timePerFunctionChopIt) {
          increment = 0;
          leaveLoop = 1;
        }
      }


      // After exiting the while loop, determine which sound effect to play, depending
      // on how the player performed previously.
      if (increment != 0) {

        mp3.playTrackNumber(mp3_sayGoodJob, 25);
      } else {
        mp3.playTrackNumber(mp3_sayYouSuck, 25);
      }
    }


    //////////////////// COOK IT ////////////////////
    else if ((randNumber >= 20) && (randNumber < 40)) {

      mp3.playTrackNumber(mp3_callCookIt, 25);
      delay(2000);


      // First check for button input from the encoder. The program will leave this loop under 2 conditions:
      // 1) The player successfully press the encoder button or
      // 2) The player has spent too long doing an input
      while (leaveLoop == 0) {

        // Run chop it. The return value will be the current number of chops that have been done.
        returnNumber = chopItInstance.runChopIt();
        if (returnNumber != 0 && (leaveLoop == 0)) {
          increment = 0;
          leaveLoop = 1;
          skip = true;
        }

        wrong = plateItInstance.plateItNormal();
        if ((wrong == 2) && (leaveLoop == 0)) {
          increment = 0;
          leaveLoop = 1;
          skip = true;
        }

        // Read if the button has been pressed. Return 1 if there is a press, 0 otherwise.
        ha = cookItInstance.runCookItButton();
        if (ha >= 1 && (leaveLoop == 0)) {
          leaveLoop = 1;
        }
        delay(50);
        // Measure the current time and determing if the user is taking too long to complete the action.
        endTime = millis();
        if ((endTime - startTime) >= timePerFunctionCookIt/1.5) {
          increment = 0;
          leaveLoop = 1;
          tooLong = true;
        }
      }

      // Second check for encoder "rotary" input. The program will leave this loop under 2 conditions:
      // 1) The player successfully turned the encoder clockwise the proper amount of turns.
      // 2) The player has spent too long doing an input
      leaveLoop = 0;
      while ((leaveLoop == 0) && (tooLong == false) && (skip == false)) {
        // Check the number of "ticks" that have been made. The return value will be the number of ticks.
        returnNumber = cookItInstance.runCookItEncoder();
        if (returnNumber >= requiredCookItMoves) {
          leaveLoop = 1;
          increment = 1;
        }

        // Measure the current time and determing if the user is taking too long to complete the action.
        endTime = millis();
        if ((endTime - startTime) >= timePerFunctionCookIt/1.5) {
          increment = 0;
          leaveLoop = 1;
          tooLong = true;
        }
      }


      // After exiting the while loop, determine which sound effect to play,
      // depending on how the player performed previously.
      if (increment != 0) {
        mp3.playTrackNumber(mp3_sayGoodJob, 25);
      } else {
        mp3.playTrackNumber(mp3_sayYouSuck, 25);
      }
    }


    //////////////////// PLATE IT ////////////////////
    else {

      mp3.playTrackNumber(mp3_callPlateIt, 25);
      delay(2000);

      plateItInstance.generateNewBurger(burgerSize);

      // Check for Plate-It input and time spent waiting for input. Exits if:
      // 1) The player has made 1 input on Plate-It (EX: place lettuce, remove burger, or ring bell)
      // 2) The user takes too long to "Chop-It"
      while (leaveLoop == 0) {
        // Run plate it. The return number will be 1 if a general input has occured (EX: burger placed).
        // The return will be 2 if the bell has been rung. Otherwise the return will be 0.

        returnNumber = chopItInstance.runChopIt();
        if (returnNumber != 0 && leaveLoop == 0) {
          increment = 0;
          leaveLoop = 1;
        }

        ha = cookItInstance.runCookItButton();
        if (ha >= 1 && leaveLoop == 0) {
          increment = 0;
          leaveLoop = 1;
        }

        wrong = plateItInstance.plateItNormal();
        if (wrong == 2 && leaveLoop == 0) {
          increment = 1;
          leaveLoop = 1;
        }
        delay(50);

        // Measure the current time and determing if the user is taking too long to complete the action.
        endTime = millis();
        if ((endTime - startTime) >= timePerFunctionPlateIt) {
          increment = 0;
          leaveLoop = 1;
        }
      }

      // After exiting the while loop, determine which sound effect to play,
      // depending on how the player performed previously.
      // INCORRECT INPUT
      /*if (increment == 0) {
        mp3.playTrackNumber(mp3_sayYouSuck, 25);
      }*/

      // REGULAR INGREDIENT PLACE
      if (increment == 0) {
        mp3.playTrackNumber(mp3_sayYouSuck, 25);
      }

      // BELL RANG
      else if (increment == 1) {
        mp3.playTrackNumber(mp3_bell, 25);
        delay(1000);
      }

      /*
      if (plateItInstance.forceDisassemble()) {

        mp3.playTrackNumber(mp3_disassembleBurger, 25);
        // Get starting time to compare time passed later.
        startTime = millis();

        // Check for "Disassemble-It" input and time spent waiting for input. Exits if:
        // 1) The player has removed all burger ingredients
        // 2) The user takes too long
        leaveLoop = 0;
        while (!leaveLoop) {
          //returnNumber will be 1 on the condition that all ingredients have been removed from Plate-It;
          returnNumber = plateItInstance.disassembleBurger();
          if (returnNumber != 0) {
            leaveLoop = 1;
          }
          delay(50);

          // Measure the current time and determing if the user is taking too long to complete the action.
          endTime = millis();
          if ((endTime - startTime) >= timePerFunctionDisassembleIt) {
            increment = 0;
            leaveLoop = 1;
          }
        }


        // After exiting the while loop, determine which sound effect to play,
        // depending on how the player performed previously.
        if (increment != 0) {
          mp3.playTrackNumber(mp3_sayGoodJob, 25);
        } else {
          mp3.playTrackNumber(mp3_sayYouSuck, 25);
        }
      }*/
    }
    delay(1500);


    // Reset all of out classes.
    cookItInstance.resetCookIt();
    chopItInstance.resetChopIt();
    plateItInstance.resetPlateIt();

    // Increase total score.
    if (increment == 1) {
      if (misc.getScore() == 99){
        mp3.playTrackNumber(mp3_win, 25);
        delay(7500);
      }
      misc.increaseScore();
    } else {
      misc.decreaseLives();
    }

    // Get total score and set burger size.
    totalScore = misc.getScore();
    if ((totalScore >= 0) && (totalScore < 20)) {
      burgerSize = 3;
    } else if ((totalScore >= 20) && (totalScore < 40)) {
      burgerSize = 4;
    } else if ((totalScore >= 40) && (totalScore < 60)) {
      burgerSize = 5;
    } else if ((totalScore >= 60) && (totalScore < 80)) {
      burgerSize = 6;
    } else if (totalScore >= 80) {
      burgerSize = 7;
    }


    

    
    // Display score.
    totalScore = misc.getScore();
    totalLives = misc.getLives();
    display.clearDisplay();
    display.display();
    display.setCursor(0, 0);
    display.print("Score: ");
    display.println(totalScore);
    display.print(totalLives);
    display.setCursor(0, 0);
    display.display();

    // Check if user is out of lives
    if (misc.getLives() == 99){
      playGame = false;
      mp3.playTrackNumber(mp3_lose, 25);
      delay(3500);
    }


    // Decrease allowed time per function.
    if ((totalScore % 5 == 0) && (totalScore != 0)) {
      mp3.playTrackNumber(mp3_faster, 25);
      delay(1750);
      timePerFunctionChopIt = timePerFunctionChopIt - 50;
      timePerFunctionCookIt = timePerFunctionCookIt - 50;
      timePerFunctionPlateIt = timePerFunctionPlateIt - 50;
      timePerFunctionDisassembleIt = timePerFunctionDisassembleIt - 50;
    } 
  }
  //// END GAME WHILE LOOP //
}
