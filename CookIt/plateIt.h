// plateIt Class
#include <Adafruit_NeoPixel.h>
#include "ADS1X15.h"

class PlateIt
{
private:

  ADS1115 ADS(0x48);

  Adafruit_NeoPixel strip_order;
  Adafruit_NeoPixel strip_player;

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

  int order[7] = {5, 0, 1, 4, 3, 2, 6};
  //BottomBun, Patty, Cheese, Lettuce, Onion, TopBun
  int playerOrder[10] = {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1};
  int ing = 0;
  bool winFlag = 1;
  int len;

public:
    PlateIt(int plateItBottomBun, int plateItTopBun, int plateItBell, int neoPixelLED0, int neoPixelLED1, int neoPixelLEDCount);
    ~PlateIt();
    void plateItNormal();
    bool checkPatty();
    bool checkCheese();
    bool checkTomato();
    bool checkOnion();
    bool checkLettuce();
};

PlateIt::PlateIt(int plateItBottomBun, int plateItTopBun, int plateItBell, int neoPixelLED0, int neoPixelLED1, int neoPixelLEDCount)
{

  strip_order = Adafruit_NeoPixel(neoPixelLEDCount, neoPixelLED0, NEO_GRB + NEO_KHZ800);
  strip_player = Adafruit_NeoPixel(neoPixelLEDCount, neoPixelLED1, NEO_GRB + NEO_KHZ800);

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
}

void::plateItNormal()
{
  while(true)
  {
    if (checkPatty() == 1){
      break;
    }
    else if(checkCheese() == 1)
    {
      break;
    }
    else if(checkTomato() == 1)
    {
      break;
    }
    else if(checkOnion() == 1)
    {
      break;
    }
    else if(checkLettuce() == 1)
    {
      break;
    }
  }
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
    readInput = 1
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
  bool readInput;
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