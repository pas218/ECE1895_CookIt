// plateIt Class
#include <Adafruit_NeoPixel.h>
#include "ADS1X15.h"
#ifdef __AVR__
#include <avr/power.h>  // Required for 16 MHz Adafruit Trinket
#endif                  // Done


// Ingredient coding system
#define numIngredients 5
#define pattyID 0
#define cheeseID 1
#define tomatoID 2
#define onionID 3
#define lettuceID 4
#define bottomBunID 5
#define topBunID 6
#define maxBurgerSize 7

class PlateIt {
private:

  ADS1115 ADS = ADS1115(0x48);
  //strip_order = Adafruit_NeoPixel(neoPixelLEDCount, neoPixelLED0, NEO_GRB + NEO_KHZ800);
  //strip_player = Adafruit_NeoPixel(neoPixelLEDCount, neoPixelLED1, NEO_GRB + NEO_KHZ800);
  Adafruit_NeoPixel strip_player = Adafruit_NeoPixel(8, 1, NEO_GRB + NEO_KHZ800);   // player
  Adafruit_NeoPixel strip_order = Adafruit_NeoPixel(8, 2, NEO_GRB + NEO_KHZ800);    // order
  

  uint32_t BunRGB;
  uint32_t PattyRGB;
  uint32_t LettuceRGB;
  uint32_t OnionRGB;
  uint32_t CheeseRGB;
  uint32_t TomatoRGB;
  uint32_t BlankRGB;

  int resPin0;
  int resPin1;
  int resPin2;
  int resPin3;

  int Patty;
  int Cheese;
  int Tomato;
  int Onions;
  int Lettuce;
  int BottomBun;
  int TopBun;
  int Bell;

  int pattyCount;
  int cheeseCount;
  int tomatoCount;
  int onionCount;
  int lettuceCount;
  int bottomBunCount;
  int topBunCount;

  int topBunInput;
  int bottomBunInput;
  int bellInput;

  int randNumber;
  bool orderCorrect;


  int order[maxBurgerSize];
  int previousIngredients[numIngredients];
  int playerOrder[maxBurgerSize];
  int ing;
  bool winFlag;
  int len;


  bool checkPatty();
  bool checkCheese();
  bool checkTomato();
  bool checkOnion();
  bool checkLettuce();
  bool checkTopBun();
  bool checkBottomBun();
  bool checkBell();

public:
  PlateIt(int plateItBottomBun, int plateItTopBun, int plateItBell, int neoPixelLED0, int neoPixelLED1, int neoPixelLEDCount);
  void initialize();
  void generateNewBurger(int burgerSizeTotal);  //This burger size includes the buns.
  int plateItNormal();
  bool disassembleBurger();
  bool compareOrderToPlayer();
  void orderDisplay(int arr[], int len);
  void resetPlateIt();
  int arraySize(int arr[], int arrSize);
};

PlateIt::PlateIt(int plateItBottomBun, int plateItTopBun, int plateItBell, int neoPixelLED0, int neoPixelLED1, int neoPixelLEDCount) {


  BunRGB = strip_order.Color(255, 50, 0);
  PattyRGB = strip_order.Color(255, 255, 255);
  LettuceRGB = strip_order.Color(30, 255, 0);
  OnionRGB = strip_order.Color(200, 0, 255);
  CheeseRGB = strip_order.Color(255, 200, 0);
  TomatoRGB = strip_order.Color(255, 0, 0);
  BlankRGB = strip_order.Color(0, 0, 0);

  resPin0 = A0;
  resPin1 = A1;
  resPin2 = A2;
  resPin3 = A3;

  Patty = 0;
  Cheese = 0;
  Tomato = 0;
  Onions = 0;
  Lettuce = 0;
  BottomBun = 0;
  TopBun = 0;

  pattyCount = 0;
  cheeseCount = 0;
  tomatoCount = 0;
  onionCount = 0;
  lettuceCount = 0;
  bottomBunCount = 0;
  topBunCount = 0;

  bottomBunInput = plateItBottomBun;
  topBunInput = plateItTopBun;
  bellInput = plateItBell;

  orderCorrect = false;


  ing = 0;
  winFlag = 1;
  len = 7;


  for (int i = 0; i < maxBurgerSize; i++) {
    playerOrder[i] = -1;
  }
}

void PlateIt::initialize() {
  // Initialize Plate-It NeoPixels

  strip_order.begin();
  for (int i = 0; i < maxBurgerSize; i++) {
    strip_order.Color(0, 0, 0);
  }
  strip_order.show();  // Initialize all pixels to 'off'
  strip_order.setBrightness(50);

  strip_player.begin();
  for (int i = 0; i < maxBurgerSize; i++) {
    strip_player.Color(0, 0, 0);
  }
  strip_player.show();  // Initialize all pixels to 'off'
  strip_player.setBrightness(50);

  for (int i = 0; i < maxBurgerSize-2; i++){
    previousIngredients[i] = -1;
  }

  ADS.begin();
  ADS.setGain(0);
}


void PlateIt::generateNewBurger(int burgerSizeTotal){


  len = burgerSizeTotal;

  // First clear previous order
  for (int i = 0; i < maxBurgerSize; i++){
    order[i] = 0;
  }

  //Set bottom bun
  order[0] = bottomBunID;

  int currentSizeIngredients = 0;
  bool increment = true;
  // Loop through size-2 because the top and bottom bun have set positions.
  while (currentSizeIngredients < burgerSizeTotal-2){
    
    // Generate random nubmer to select ingredient.
    randNumber = random(0, numIngredients);
    
    increment = true;
    // If this is not the first ingredient, then make sure it has not been placed before.
    // If this ingredient has been placed before, then it will not be placed again and
    // another ingredient will be chosen.
    // Must also adhere to a couple of rules: 1) lettuce and cheese cannot be adjacent to each other
    // 2) The following combintation is not allowed: lettuce - onion - cheese
    if (currentSizeIngredients != 0){
      
      // Check for previous ingredients
      for (int i = 0; i < currentSizeIngredients; i++){
        if (randNumber == previousIngredients[i]){
          increment = false;
          break;
        }
      }

      // Check for forbidden placements:
      // 1) lettuce - cheese
      // 2) cheese - lettuce
      // 3) lettuc - onion - cheese
      // If the order length is maximum (maxBurgerSize), and on of the forbidden combinations occurs,
      // then have to push back the currentSizeIngredients counter to allow for recalculation
      if ((randNumber == lettuceID)){
        if (previousIngredients[currentSizeIngredients-1] == cheeseID){
          increment = false;

          // Force algorithm to recalculate the burger order.
          if ((burgerSizeTotal == maxBurgerSize) && (currentSizeIngredients == numIngredients-1)){
            currentSizeIngredients = 0;
          }
        }
      }
      else if ((randNumber == cheeseID)){
        if (previousIngredients[currentSizeIngredients-1] == lettuceID){
          increment = false;

          // Force algorithm to recalculate the burger order.
          if ((burgerSizeTotal == maxBurgerSize) && (currentSizeIngredients == numIngredients-1)){
            currentSizeIngredients = 0;
          }
        }
        else if ((currentSizeIngredients >= 2) && (previousIngredients[currentSizeIngredients-1] == onionID) && (previousIngredients[currentSizeIngredients-2] == lettuceID)){
          increment = false;

          // Force algorithm to recalculate the burger order.
          if ((burgerSizeTotal == maxBurgerSize) && (currentSizeIngredients == numIngredients-1)){
            currentSizeIngredients = 0;
          }
        }
      }

      
    }
    
    // Decide whether or not to increment to the next ingredient.
    if (increment == true){
      previousIngredients[currentSizeIngredients] = randNumber;
      order[1+currentSizeIngredients] = randNumber;
      currentSizeIngredients++;
    }
  }

  order[burgerSizeTotal-1] = topBunID;


  orderDisplay(order, burgerSizeTotal);
} 


int PlateIt::plateItNormal() {

  int returnVal = 0;
  bool checker;

  checker = checkPatty();
  if (checker == 1) {
    returnVal = 1;
  }
  checker = checkCheese();
  if (checker == 1) {
    returnVal = 1;
  }
  checker = checkTomato();
  if (checker == 1) {
    returnVal = 1;
  }
  checker = checkOnion();
  if (checker == 1) {
    returnVal = 1;
  }
  checker = checkTopBun();
  if (checker == 1) {
    returnVal = 1;
  }
  checker = checkBottomBun();
  if (checker == 1) {
    returnVal = 1;
  }
  checker = checkLettuce();
  if (checker == 1) {
    returnVal = 1;
  }
  checker = checkBell();
  if (checker == 1) {
    returnVal = 2;
  }

  return returnVal;
}


bool PlateIt::disassembleBurger(){

  bool returnValue = false;
  checkPatty();
  checkCheese();
  checkTomato();
  checkOnion();
  checkTopBun();
  checkBottomBun();
  checkLettuce();

  delay(50);

  if (ing == 0){
    returnValue = true;
  }
  else{
    returnValue = false;
  }

  return returnValue;
}


bool PlateIt::compareOrderToPlayer(){

  orderCorrect = true;

  for (int i = 0; i < len; i++){
    if (order[i] != playerOrder[i]){
      orderCorrect = false;
      break;
    }
  }
  
}

bool PlateIt::checkPatty() {

  bool readInput = 0;
  Patty = analogRead(resPin0);
  if (Patty >= 400 && Patty < 525 && pattyCount == 0) {
    playerOrder[ing] = 0;
    strip_player.setPixelColor(ing, PattyRGB);
    strip_player.show();
    ing++;
    pattyCount++;
    readInput = 1;
  } else if (Patty >= 200 && Patty < 350 && pattyCount == 1) {
    playerOrder[ing] = 0;
    strip_player.setPixelColor(ing, PattyRGB);
    strip_player.show();
    ing++;
    pattyCount++;
    readInput = 1;
  } else if ((Patty == 1023 && pattyCount == 1) || (Patty >= 400 && Patty < 525 && pattyCount == 2)) {
    ing--;
    pattyCount--;
    playerOrder[ing] = -1;
    strip_player.setPixelColor(ing, BlankRGB);
    strip_player.show();
    readInput = 1;
  }

  return readInput;
}

bool PlateIt::checkCheese() {
  bool readInput = 0;
  // Cheese
  Cheese = analogRead(resPin1);
  if (Cheese >= 400 && Cheese < 525 && cheeseCount == 0) {
    playerOrder[ing] = 1;
    strip_player.setPixelColor(ing, CheeseRGB);
    strip_player.show();
    ing++;
    cheeseCount++;
    readInput = 1;
  } else if (Cheese >= 200 && Cheese < 350 && cheeseCount == 1) {
    playerOrder[ing] = 1;
    strip_player.setPixelColor(ing, CheeseRGB);
    strip_player.show();
    ing++;
    cheeseCount++;
    readInput = 1;
  } else if ((Cheese == 1023 && cheeseCount == 1) || (Cheese >= 400 && Cheese < 525 && cheeseCount == 2)) {
    ing--;
    cheeseCount--;
    playerOrder[ing] = -1;
    strip_player.setPixelColor(ing, BlankRGB);
    strip_player.show();
    readInput = 1;
  }

  return readInput;
}

bool PlateIt::checkTomato() {
  bool readInput = 0;
  // Tomato
  Tomato = analogRead(resPin2);
  if (Tomato >= 400 && Tomato < 525 && tomatoCount == 0) {
    playerOrder[ing] = 2;
    strip_player.setPixelColor(ing, TomatoRGB);
    strip_player.show();
    ing++;
    tomatoCount++;
    readInput = 1;
  } else if (Tomato >= 200 && Tomato < 350 && tomatoCount == 1) {
    playerOrder[ing] = 2;
    strip_player.setPixelColor(ing, TomatoRGB);
    strip_player.show();
    ing++;
    tomatoCount++;
    readInput = 1;
  } else if ((Tomato == 1023 && tomatoCount == 1) || (Tomato >= 400 && Tomato < 525 && tomatoCount == 2)) {
    ing--;
    tomatoCount--;
    playerOrder[ing] = -1;
    strip_player.setPixelColor(ing, BlankRGB);
    strip_player.show();
    readInput = 1;
  }

  return readInput;
}

bool PlateIt::checkOnion() {
  bool readInput = 0;
  // Onions
  Onions = analogRead(resPin3);
  if (Onions >= 400 && Onions < 525 && onionCount == 0) {
    playerOrder[ing] = 3;
    strip_player.setPixelColor(ing, OnionRGB);
    strip_player.show();
    ing++;
    onionCount++;
    readInput = 1;
  } else if (Onions >= 200 && Onions < 350 && onionCount == 1) {
    playerOrder[ing] = 3;
    strip_player.setPixelColor(ing, OnionRGB);
    strip_player.show();
    ing++;
    onionCount++;
    readInput = 1;
  } else if ((Onions == 1023 && onionCount == 1) || (Onions >= 400 && Onions < 525 && onionCount == 2)) {
    ing--;
    onionCount--;
    playerOrder[ing] = -1;
    strip_player.setPixelColor(ing, BlankRGB);
    strip_player.show();
    readInput = 1;
  }

  return readInput;
}


bool PlateIt::checkLettuce() {
  bool readInput = 0;
  // Lettuce
  Lettuce = ADS.readADC(0);
  if (Lettuce >= 13300 && Lettuce < 13600 && lettuceCount == 0) {
    playerOrder[ing] = 4;
    strip_player.setPixelColor(ing, LettuceRGB);
    strip_player.show();
    ing++;
    lettuceCount++;
    readInput = 1;
  } else if (Lettuce >= 8800 && Lettuce < 9100 && lettuceCount == 1) {
    playerOrder[ing] = 4;
    strip_player.setPixelColor(ing, LettuceRGB);
    strip_player.show();
    ing++;
    lettuceCount++;
    readInput = 1;
  } else if ((Lettuce > 26000 && lettuceCount == 1) || (Lettuce >= 13300 && Lettuce < 13600 && lettuceCount == 2)) {
    ing--;
    lettuceCount--;
    playerOrder[ing] = -1;
    strip_player.setPixelColor(ing, BlankRGB);
    strip_player.show();
    readInput = 1;
  }

  return readInput;
}


bool PlateIt::checkBottomBun() {
  // BottomBun
  int readInput = 0;
  BottomBun = digitalRead(bottomBunInput);
  if (BottomBun == LOW && bottomBunCount == 0) {
    playerOrder[ing] = 5;
    strip_player.setPixelColor(ing, BunRGB);
    strip_player.show();
    ing++;
    bottomBunCount++;
    readInput = 1;
  } else if (BottomBun == HIGH && bottomBunCount != 0) {
    ing--;
    bottomBunCount--;
    playerOrder[ing] = -1;
    strip_player.setPixelColor(ing, BlankRGB);
    strip_player.show();
    readInput = 1;
  }

  return readInput;
}


bool PlateIt::checkTopBun() {
  // TopBun
  int readInput = 0;
  TopBun = digitalRead(topBunInput);
  if (TopBun == LOW && topBunCount == 0) {
    playerOrder[ing] = 6;
    strip_player.setPixelColor(ing, BunRGB);
    strip_player.show();
    ing++;
    topBunCount++;
    readInput = 1;
  } else if (TopBun == HIGH && topBunCount != 0) {
    ing--;
    topBunCount--;
    playerOrder[ing] = -1;
    strip_player.setPixelColor(ing, BlankRGB);
    strip_player.show();
    readInput = 1;
  }
  return readInput;
}

bool PlateIt::checkBell() {
  // Bell
  int readInput = 0;
  Bell = digitalRead(bellInput);
  if (Bell == LOW) {
    readInput = 1;
  }
  return readInput;
}


void PlateIt::orderDisplay(int arr[], int len) {

  for (int i = 0; i < maxBurgerSize; i++) {
    strip_order.Color(0, 0, 0);
  }
  strip_order.show();

  for (int i = 0; i < len; i++) {
    int ing = arr[i];
    switch (ing) {
      // Patty
      case 0:
        strip_order.setPixelColor(i, PattyRGB);
      
        break;

      // Cheese
      case 1:
        strip_order.setPixelColor(i, CheeseRGB);
        break;

      // Tomato
      case 2:
        strip_order.setPixelColor(i, TomatoRGB);
        break;

      // Onion
      case 3:
        strip_order.setPixelColor(i, OnionRGB);
        break;

      // Lettuce
      case 4:
        strip_order.setPixelColor(i, LettuceRGB);
        break;

      // BottomBun
      case 5:
        strip_order.setPixelColor(i, BunRGB);
        break;

      // TopBun
      case 6:
        strip_order.setPixelColor(i, BunRGB);
        break;
    }
    strip_order.show();
  }
}

void PlateIt::resetPlateIt(){
  
  for (int i = 0; i < maxBurgerSize; i++) {
    strip_player.Color(0, 0, 0);
  }
  strip_player.show();

}

int PlateIt::arraySize(int arr[], int arrSize) {
  int len = arrSize / sizeof(arr[0]);
  return len;
}
