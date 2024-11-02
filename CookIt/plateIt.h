// plateIt Class
#include <Adafruit_NeoPixel.h>
#include "ADS1X15.h"
#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif // Done

class PlateIt
{
private:

  ADS1115 ADS = ADS1115(0x48);
//strip_order = Adafruit_NeoPixel(neoPixelLEDCount, neoPixelLED0, NEO_GRB + NEO_KHZ800);
  //strip_player = Adafruit_NeoPixel(neoPixelLEDCount, neoPixelLED1, NEO_GRB + NEO_KHZ800);
  Adafruit_NeoPixel strip_order = Adafruit_NeoPixel(8, 0, NEO_GRB + NEO_KHZ800);
  Adafruit_NeoPixel strip_player = Adafruit_NeoPixel(8, 1, NEO_GRB + NEO_KHZ800);

  uint32_t BunRGB;
  uint32_t PattyRGB ;
  uint32_t LettuceRGB;
  uint32_t OnionRGB;
  uint32_t CheeseRGB;
  uint32_t TomatoRGB;
  uint32_t BlankRGB ;

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

  int order[7];
  //BottomBun, Patty, Cheese, Lettuce, Onion, TopBun
  int playerOrder[10];
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
    int plateItNormal();
    int plateItTestOne();
    void orderDisplay(int arr[], int len);
    int arraySize(int arr[], int arrSize);
};

PlateIt::PlateIt(int plateItBottomBun, int plateItTopBun, int plateItBell, int neoPixelLED0, int neoPixelLED1, int neoPixelLEDCount)
{


  BunRGB = strip_order.Color(255,50,0);
  PattyRGB = strip_order.Color(255,255,255);
  LettuceRGB = strip_order.Color(30,255,0);
  OnionRGB = strip_order.Color(200,0,255);
  CheeseRGB = strip_order.Color(255,200,0);
  TomatoRGB = strip_order.Color(255,0,0);
  BlankRGB = strip_order.Color(0,0,0);

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

  
  order[0] = 5;
  order[1] = 0;
  order[2] = 1;
  order[3] = 4;
  order[4] = 3;
  order[5] = 2;
  order[6] = 6;

  ing = 0;
  winFlag = 1;
  len = 7;


  for(int i = 0; i < 10; i++)
  {
    playerOrder[i] = -1;
  }

  //ADS1115 ADS(0x48);
}

void PlateIt::initialize()
{
   // Initialize Plate-It NeoPixels
  strip_order.begin();
  for (int i = 0; i < 8; i++)
  {
    strip_order.Color(0,0,0);
  }
  strip_order.show(); // Initialize all pixels to 'off'
  strip_order.setBrightness(50);
  strip_player.begin();
  for (int i = 0; i < 8; i++)
  {
    strip_player.Color(0,0,0);
  }
  strip_player.show(); // Initialize all pixels to 'off'
  strip_player.setBrightness(50);
  orderDisplay(order, len);

  ADS.begin();
  ADS.setGain(0);
}

int PlateIt::plateItTestOne()
{
  bool checker;
  while(true){
    checker = checkPatty();
    if (checker == 1){
      break;
    }
  }

  return 1;
}

int PlateIt::plateItNormal()
{
  int returnVal = 0;
  bool checker;
  while(true)
  {
    checker = checkPatty();
    if (checker == 1){
      returnVal = 1;
      break;
    }
    checker = checkCheese();
    if (checker == 1){
      returnVal = 1;
      break;
    }
    checker = checkTomato();
    if (checker == 1){
      returnVal = 1;
      break;
    }
    checker = checkOnion();
    if (checker == 1){
      returnVal = 1;
      break;
    }
    checker = checkTopBun();
    if (checker == 1){
      returnVal = 1;
      break;
    }
    checker = checkBottomBun();
    if (checker == 1){
      returnVal = 1;
      break;
    }
    checker = checkLettuce();
    if(checker == 1)
    {
      returnVal = 1;
      break;
    }
    checker = checkBell();
    if(checker == 1)
    {
      returnVal = 2;
      break;
    }
  }
  return returnVal;
}
 bool PlateIt::checkPatty(){

  bool readInput = 0;
  Patty = analogRead(resPin0);
  if (Patty >=400 && Patty <525 && pattyCount == 0)
  {
    playerOrder[ing] = 0;
    strip_player.setPixelColor(ing, PattyRGB);
    strip_player.show();
    ing++;
    pattyCount++;
    readInput = 1;
  }
  else if (Patty >= 200 && Patty <350 && pattyCount == 1)
  {
    playerOrder[ing] = 0;
    strip_player.setPixelColor(ing, PattyRGB);
    strip_player.show();
    ing++;
    pattyCount++;
    readInput = 1;
  }
  else if((Patty == 1023 && pattyCount == 1) || (Patty >=400 && Patty <525 && pattyCount == 2))
  {
    ing--;
    pattyCount--;
    playerOrder[ing] = -1;
    strip_player.setPixelColor(ing, BlankRGB);
    strip_player.show();
    readInput = 1;
  }

  return readInput;
}

bool PlateIt::checkCheese()
{
  bool readInput = 0;
  // Cheese
  Cheese = analogRead(resPin1);
  if (Cheese >=400 && Cheese <525 && cheeseCount == 0)
  {
    playerOrder[ing] = 1;
    strip_player.setPixelColor(ing, CheeseRGB);
    strip_player.show();
    ing++;
    cheeseCount++;
    readInput = 1;
  }
  else if (Cheese >= 200 && Cheese <350 && cheeseCount == 1)
  {
    playerOrder[ing] = 1;
    strip_player.setPixelColor(ing, CheeseRGB);
    strip_player.show();
    ing++;
    cheeseCount++;
    readInput = 1;
  }
  else if((Cheese == 1023 && cheeseCount == 1) || (Cheese >=400 && Cheese <525 && cheeseCount == 2))
  {
    ing--;
    cheeseCount--;
    playerOrder[ing] = -1;
    strip_player.setPixelColor(ing, BlankRGB);
    strip_player.show();
    readInput = 1;
  }

  return readInput;
}

bool PlateIt::checkTomato()
{
  bool readInput = 0;
  // Tomato
  Tomato = analogRead(resPin2);
  if (Tomato >=400 && Tomato <525 && tomatoCount == 0)
  {
    playerOrder[ing] = 2;
    strip_player.setPixelColor(ing, TomatoRGB);
    strip_player.show();
    ing++;
    tomatoCount++;
    readInput = 1;
  }
  else if (Tomato >= 200 && Tomato <350 && tomatoCount == 1)
  {
    playerOrder[ing] = 2;
    strip_player.setPixelColor(ing, TomatoRGB);
    strip_player.show();
    ing++;
    tomatoCount++;
    readInput = 1;
  }
  else if((Tomato == 1023 && tomatoCount == 1) || (Tomato >=400 && Tomato <525 && tomatoCount == 2))
  {
    ing--;
    tomatoCount--;
    playerOrder[ing] = -1;
    strip_player.setPixelColor(ing, BlankRGB);
    strip_player.show();
    readInput = 1;
  }

  return readInput;
}

bool PlateIt::checkOnion()
{
  bool readInput = 0;
  // Onions
  Onions = analogRead(resPin3);
  if (Onions >=400 && Onions <525 && onionCount == 0)
  {
    playerOrder[ing] = 3;
    strip_player.setPixelColor(ing, OnionRGB);
    strip_player.show();
    ing++;
    onionCount++;
    readInput = 1;
  }
  else if (Onions >= 200 && Onions <350 && onionCount == 1)
  {
    playerOrder[ing] = 3;
    strip_player.setPixelColor(ing, OnionRGB);
    strip_player.show();
    ing++;
    onionCount++;
    readInput = 1;
  }
  else if((Onions == 1023 && onionCount == 1) || (Onions >=400 && Onions <525 && onionCount == 2))
  {
    ing--;
    onionCount--;
    playerOrder[ing] = -1;
    strip_player.setPixelColor(ing, BlankRGB);
    strip_player.show();
    readInput = 1;
  }

  return readInput;
}


bool PlateIt::checkLettuce()
{
  bool readInput = 0;
  // Lettuce
  Lettuce = ADS.readADC(0);
  if (Lettuce >=13300 && Lettuce <13600 && lettuceCount == 0)
  {
    playerOrder[ing] = 4;
    strip_player.setPixelColor(ing, LettuceRGB);
    strip_player.show();
    ing++;
    lettuceCount++;
    readInput = 1;
  }
  else if (Lettuce >= 8800 && Lettuce <9100 && lettuceCount == 1)
  {
    playerOrder[ing] = 4;
    strip_player.setPixelColor(ing, LettuceRGB);
    strip_player.show();
    ing++;
    lettuceCount++;
    readInput = 1;
  }
  else if((Lettuce > 26000 && lettuceCount == 1) || (Lettuce >=13300 && Lettuce <13600 && lettuceCount == 2))
  {
    ing--;
    lettuceCount--;
    playerOrder[ing] = -1;
    strip_player.setPixelColor(ing, BlankRGB);
    strip_player.show();
    readInput = 1;
  }

  return readInput;
}


bool PlateIt::checkBottomBun()
{
  // BottomBun
  int readInput = 0;
  BottomBun = digitalRead(bottomBunInput);
  if (BottomBun == HIGH && bottomBunCount == 0)
  {
    playerOrder[ing] = 5;
    strip_player.setPixelColor(ing, BunRGB);
    strip_player.show();
    ing++;
    bottomBunCount++;
    readInput = 1;
  }
  else if(BottomBun == LOW && bottomBunCount != 0)
  {
    ing--;
    bottomBunCount--;
    playerOrder[ing] = -1;
    strip_player.setPixelColor(ing, BlankRGB);
    strip_player.show();
    readInput = 1;
  }

  return readInput;
}


bool PlateIt::checkTopBun()
{
  // TopBun
  int readInput = 0;
  TopBun = digitalRead(topBunInput);
  if (TopBun == HIGH && topBunCount == 0)
  {
    playerOrder[ing] = 6;
    strip_player.setPixelColor(ing, BunRGB);
    strip_player.show();
    ing++;
    topBunCount++;
    readInput = 1;
  }
  else if(TopBun == LOW && topBunCount != 0)
  {
    ing--;
    topBunCount--;
    playerOrder[ing] = -1;
    strip_player.setPixelColor(ing, BlankRGB);
    strip_player.show();
    readInput = 1;
  }
  return readInput;
}

bool PlateIt::checkBell()
{
  // Bell
  int readInput = 0;
  Bell = digitalRead(bellInput);
  if (Bell == HIGH)
  {
    readInput = 1;
  }
  return readInput;
}


void PlateIt::orderDisplay(int arr[], int len)
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
}

int PlateIt::arraySize(int arr[], int arrSize)
{
 int len = arrSize / sizeof(arr[0]);
 return len;
}