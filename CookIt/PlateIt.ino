#include <Adafruit_NeoPixel.h>
#define LED_PIN    6
#define LED_PIN2    13
#define LED_COUNT 8
Adafruit_NeoPixel strip_order = Adafruit_NeoPixel(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip_player = Adafruit_NeoPixel(LED_COUNT, LED_PIN2, NEO_GRB + NEO_KHZ800);

uint32_t BunRGB = strip_order.Color(255,50,0);
uint32_t PattyRGB = strip_order.Color(255,255,255);
uint32_t LettuceRGB = strip_order.Color(30,255,0);
uint32_t OnionRGB = strip_order.Color(200,0,255);
uint32_t CheeseRGB = strip_order.Color(255,200,0);
uint32_t TomatoRGB = strip_order.Color(255,0,0);
uint32_t BlankRGB = strip_order.Color(0,0,0);

int resPin0 = A0;
int resPin1 = A1;
int resPin2 = A2;
int resPin3 = A3;
int resPin4 = A4;
int resPin5 = A5;

int Patty = 0;
int Onions = 0;
int Cheese = 0;
int Tomato = 0;
int Lettuce = 0;
int BottomBun = 0;
int TopBun = 0;

int pattyCount = 0;
int onionCount = 0;
int cheeseCount = 0;
int tomatoCount = 0;
int lettuceCount = 0;
int bottomBunCount = 0;
int topBunCount = 0;

int order[] = {6, 0, 3, 5, 2, 7};
// BottomBun, Patty, Cheese, Lettuce, Onion, TopBun
int playerOrder[10] = {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1};
int ing = 0;
bool winFlag = 1;
int len;

// the setup function runs once when you press reset or power the board
void setup() {
  strip_order.begin();
  strip_order.show(); // Initialize all pixels to 'off'
  strip_order.setBrightness(50);
  strip_player.begin();
  strip_player.show(); // Initialize all pixels to 'off'
  strip_player.setBrightness(50);
  pinMode(2, INPUT);
  pinMode(3, INPUT);
  pinMode(4, INPUT);
  Serial.begin(9600);
  Serial.println("Burger Start");
}

// the loop function runs over and over again forever
void loop()
{
  bool roundStarts = true;
  while (roundStarts)
  {
    // Get Size of Order
    len = arraySize(order, sizeof(order));
    // Display Order on Neopixels on Digital Pin 6
    orderDisplay(order, len);
    
      // End Once Bell is Pressed
      while(digitalRead(4) == LOW)
      {
        // Patty
        Patty = analogRead(resPin0);
        if (Patty >=500 && Patty <525 && pattyCount == 0)
        {
          playerOrder[ing] = 0;
          strip_player.setPixelColor(ing, PattyRGB);
          strip_player.show();
          ing++;
          pattyCount++;
          Serial.println("Patty");
          printArray(playerOrder, ing);
        }
        else if (Patty >= 330 && Patty <350 && pattyCount == 1)
        {
          playerOrder[ing] = 0;
          strip_player.setPixelColor(ing, PattyRGB);
          strip_player.show();
          ing++;
          pattyCount++;
          Serial.println("Patty");
          printArray(playerOrder, ing);
        }
        else if((Patty == 1023 && pattyCount == 1) || (Patty >=500 && Patty <525 && pattyCount == 2))
        {
          ing--;
          pattyCount--;
          Serial.println("Patty Removed");
          playerOrder[ing] = -1;
          strip_player.setPixelColor(ing, BlankRGB);
          strip_player.show();
          printArray(playerOrder, ing);
        }
  
        // Onions
        Onions = analogRead(resPin2);
        if (Onions >=500 && Onions <525 && onionCount == 0)
        {
          playerOrder[ing] = 2;
          strip_player.setPixelColor(ing, OnionRGB);
          strip_player.show();
          ing++;
          onionCount++;
          Serial.println("Onions");
          printArray(playerOrder, ing);
        }
        else if (Onions >= 330 && Onions <350 && onionCount == 1)
        {
          playerOrder[ing] = 2;
          strip_player.setPixelColor(ing, OnionRGB);
          strip_player.show();
          ing++;
          onionCount++;
          Serial.println("Onions");
          printArray(playerOrder, ing);
        }
        else if((Onions == 1023 && onionCount == 1) || (Onions >=500 && Onions <525 && onionCount == 2))
        {
          ing--;
          onionCount--;
          Serial.println("Onion Removed");
          playerOrder[ing] = -1;
          strip_player.setPixelColor(ing, BlankRGB);
          strip_player.show();
          printArray(playerOrder, ing);
        }
  
        // Cheese
        Cheese = analogRead(resPin3);
        if (Cheese >=500 && Cheese <525 && cheeseCount == 0)
        {
          playerOrder[ing] = 3;
          strip_player.setPixelColor(ing, CheeseRGB);
          strip_player.show();
          ing++;
          cheeseCount++;
          Serial.println("Cheese");
          printArray(playerOrder, ing);
        }
        else if (Cheese >= 330 && Cheese <350 && cheeseCount == 1)
        {
          playerOrder[ing] = 3;
          strip_player.setPixelColor(ing, CheeseRGB);
          strip_player.show();
          ing++;
          cheeseCount++;
          Serial.println("Cheese");
          printArray(playerOrder, ing);
        }
        else if((Cheese == 1023 && cheeseCount == 1) || (Cheese >=500 && Cheese <525 && cheeseCount == 2))
        {
          ing--;
          cheeseCount--;
          Serial.println("Cheese Removed");
          playerOrder[ing] = -1;
          strip_player.setPixelColor(ing, BlankRGB);
          strip_player.show();
          printArray(playerOrder, ing);
        }
  
        // Tomato
        Tomato = analogRead(resPin4);
        if (Tomato >=0 && Tomato <10 && tomatoCount == 0)
        {
          playerOrder[ing] = 4;
          strip_player.setPixelColor(ing, TomatoRGB);
          strip_player.show();
          ing++;
          tomatoCount++;
          Serial.println("Tomato");
          printArray(playerOrder, ing);
        }
        else if (Tomato >= 330 && Tomato <350 && tomatoCount == 1)
        {
          playerOrder[ing] = 4;
          strip_player.setPixelColor(ing, TomatoRGB);
          strip_player.show();
          ing++;
          tomatoCount++;
          Serial.println("Tomato");
          printArray(playerOrder, ing);
        }
        else if((Tomato == 1023 && tomatoCount == 1) || (Tomato >=500 && Tomato <525 && tomatoCount == 2))
        {
          ing--;
          tomatoCount--;
          Serial.println("Tomato Removed");
          playerOrder[ing] = -1;
          strip_player.setPixelColor(ing, BlankRGB);
          strip_player.show();
          printArray(playerOrder, ing);
        }
  
        // Lettuce
        Lettuce = analogRead(resPin5);
        if (Lettuce >=500 && Lettuce <525 && lettuceCount == 0)
        {
          playerOrder[ing] = 5;
          strip_player.setPixelColor(ing, LettuceRGB);
          strip_player.show();
          ing++;
          lettuceCount++;
          Serial.println("Lettuce");
          printArray(playerOrder, ing);
        }
        else if (Lettuce >= 330 && Lettuce <350 && lettuceCount == 1)
        {
          playerOrder[ing] = 5;
          strip_player.setPixelColor(ing, LettuceRGB);
          strip_player.show();
          ing++;
          lettuceCount++;
          Serial.println("Lettuce");
          printArray(playerOrder, ing);
        }
        else if((Lettuce == 1023 && lettuceCount == 1) || (Lettuce >=500 && Lettuce <525 && lettuceCount == 2))
        {
          ing--;
          lettuceCount--;
          Serial.println("Lettuce Removed");
          playerOrder[ing] = -1;
          strip_player.setPixelColor(ing, BlankRGB);
          strip_player.show();
          printArray(playerOrder, ing);
        }

        // BottomBun
        BottomBun = digitalRead(2);
        if (BottomBun == HIGH && bottomBunCount == 0)
        {
          playerOrder[ing] = 6;
          strip_player.setPixelColor(ing, BunRGB);
          strip_player.show();
          ing++;
          bottomBunCount++;
          Serial.println("BottomBun");
          printArray(playerOrder, ing);
        }
        else if(BottomBun == LOW && bottomBunCount != 0)
        {
          ing--;
          bottomBunCount--;
          Serial.println("BottomBun Removed");
          playerOrder[ing] = -1;
          strip_player.setPixelColor(ing, BlankRGB);
          strip_player.show();
          printArray(playerOrder, ing);
        }
  
        // TopBun
        TopBun = digitalRead(3);
        if (TopBun == HIGH && topBunCount == 0)
        {
          playerOrder[ing] = 7;
          strip_player.setPixelColor(ing, BunRGB);
          strip_player.show();
          ing++;
          topBunCount++;
          Serial.println("TopBun");
          printArray(playerOrder, ing);
        }
        else if(TopBun == LOW && topBunCount != 0)
        {
          ing--;
          topBunCount--;
          Serial.println("TopBun Removed");
          playerOrder[ing] = -1;
          strip_player.setPixelColor(ing, BlankRGB);
          strip_player.show();
          printArray(playerOrder, ing);
        }
        delay(50);
      }
      
      Serial.println("Bell Rung!");
      delay(1000);
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
  
      if (winFlag == 1)
        Serial.println("Good Job!");
      else
        Serial.println("Get Out Of The Kitchen!");
      delay(1000);
      break;
  }

  // Force Player to Disassemble
  bool empty = 0;
  while (empty == 0)
  {
        // Patty
        Patty = analogRead(resPin0);
        if (Patty >=500 && Patty <525 && pattyCount == 0)
        {
          playerOrder[ing] = 0;
          strip_player.setPixelColor(ing, PattyRGB);
          strip_player.show();
          ing++;
          pattyCount++;
          Serial.println("Patty");
          printArray(playerOrder, ing);
        }
        else if (Patty >= 330 && Patty <350 && pattyCount == 1)
        {
          playerOrder[ing] = 0;
          strip_player.setPixelColor(ing, PattyRGB);
          strip_player.show();
          ing++;
          pattyCount++;
          Serial.println("Patty");
          printArray(playerOrder, ing);
        }
        else if((Patty == 1023 && pattyCount == 1) || (Patty >=500 && Patty <525 && pattyCount == 2))
        {
          ing--;
          pattyCount--;
          Serial.println("Patty Removed");
          playerOrder[ing] = -1;
          strip_player.setPixelColor(ing, BlankRGB);
          strip_player.show();
          printArray(playerOrder, ing);
        }
  
        // Onions
        Onions = analogRead(resPin2);
        if (Onions >=500 && Onions <525 && onionCount == 0)
        {
          playerOrder[ing] = 2;
          strip_player.setPixelColor(ing, OnionRGB);
          strip_player.show();
          ing++;
          onionCount++;
          Serial.println("Onions");
          printArray(playerOrder, ing);
        }
        else if (Onions >= 330 && Onions <350 && onionCount == 1)
        {
          playerOrder[ing] = 2;
          strip_player.setPixelColor(ing, OnionRGB);
          strip_player.show();
          ing++;
          onionCount++;
          Serial.println("Onions");
          printArray(playerOrder, ing);
        }
        else if((Onions == 1023 && onionCount == 1) || (Onions >=500 && Onions <525 && onionCount == 2))
        {
          ing--;
          onionCount--;
          Serial.println("Onion Removed");
          playerOrder[ing] = -1;
          strip_player.setPixelColor(ing, BlankRGB);
          strip_player.show();
          printArray(playerOrder, ing);
        }
  
        // Cheese
        Cheese = analogRead(resPin3);
        if (Cheese >=500 && Cheese <525 && cheeseCount == 0)
        {
          playerOrder[ing] = 3;
          strip_player.setPixelColor(ing, CheeseRGB);
          strip_player.show();
          ing++;
          cheeseCount++;
          Serial.println("Cheese");
          printArray(playerOrder, ing);
        }
        else if (Cheese >= 330 && Cheese <350 && cheeseCount == 1)
        {
          playerOrder[ing] = 3;
          strip_player.setPixelColor(ing, CheeseRGB);
          strip_player.show();
          ing++;
          cheeseCount++;
          Serial.println("Cheese");
          printArray(playerOrder, ing);
        }
        else if((Cheese == 1023 && cheeseCount == 1) || (Cheese >=500 && Cheese <525 && cheeseCount == 2))
        {
          ing--;
          cheeseCount--;
          Serial.println("Cheese Removed");
          playerOrder[ing] = -1;
          strip_player.setPixelColor(ing, BlankRGB);
          strip_player.show();
          printArray(playerOrder, ing);
        }
  
        // Tomato
        Tomato = analogRead(resPin4);
        if (Tomato >=0 && Tomato <10 && tomatoCount == 0)
        {
          playerOrder[ing] = 4;
          strip_player.setPixelColor(ing, TomatoRGB);
          strip_player.show();
          ing++;
          tomatoCount++;
          Serial.println("Tomato");
          printArray(playerOrder, ing);
        }
        else if (Tomato >= 330 && Tomato <350 && tomatoCount == 1)
        {
          playerOrder[ing] = 4;
          strip_player.setPixelColor(ing, TomatoRGB);
          strip_player.show();
          ing++;
          tomatoCount++;
          Serial.println("Tomato");
          printArray(playerOrder, ing);
        }
        else if((Tomato == 1023 && tomatoCount == 1) || (Tomato >=500 && Tomato <525 && tomatoCount == 2))
        {
          ing--;
          tomatoCount--;
          Serial.println("Tomato Removed");
          playerOrder[ing] = -1;
          strip_player.setPixelColor(ing, BlankRGB);
          strip_player.show();
          printArray(playerOrder, ing);
        }
  
        // Lettuce
        Lettuce = analogRead(resPin5);
        if (Lettuce >=500 && Lettuce <525 && lettuceCount == 0)
        {
          playerOrder[ing] = 5;
          strip_player.setPixelColor(ing, LettuceRGB);
          strip_player.show();
          ing++;
          lettuceCount++;
          Serial.println("Lettuce");
          printArray(playerOrder, ing);
        }
        else if (Lettuce >= 330 && Lettuce <350 && lettuceCount == 1)
        {
          playerOrder[ing] = 5;
          strip_player.setPixelColor(ing, LettuceRGB);
          strip_player.show();
          ing++;
          lettuceCount++;
          Serial.println("Lettuce");
          printArray(playerOrder, ing);
        }
        else if((Lettuce == 1023 && lettuceCount == 1) || (Lettuce >=500 && Lettuce <525 && lettuceCount == 2))
        {
          ing--;
          lettuceCount--;
          Serial.println("Lettuce Removed");
          playerOrder[ing] = -1;
          strip_player.setPixelColor(ing, BlankRGB);
          strip_player.show();
          printArray(playerOrder, ing);
        }

        // BottomBun
        BottomBun = digitalRead(2);
        if (BottomBun == HIGH && bottomBunCount == 0)
        {
          playerOrder[ing] = 6;
          strip_player.setPixelColor(ing, BunRGB);
          strip_player.show();
          ing++;
          bottomBunCount++;
          Serial.println("BottomBun");
          printArray(playerOrder, ing);
        }
        else if(BottomBun == LOW && bottomBunCount != 0)
        {
          ing--;
          bottomBunCount--;
          Serial.println("BottomBun Removed");
          playerOrder[ing] = -1;
          strip_player.setPixelColor(ing, BlankRGB);
          strip_player.show();
          printArray(playerOrder, ing);
        }
  
        // TopBun
        TopBun = digitalRead(3);
        if (TopBun == HIGH && topBunCount == 0)
        {
          playerOrder[ing] = 7;
          strip_player.setPixelColor(ing, BunRGB);
          strip_player.show();
          ing++;
          topBunCount++;
          Serial.println("TopBun");
          printArray(playerOrder, ing);
        }
        else if(TopBun == LOW && topBunCount != 0)
        {
          ing--;
          topBunCount--;
          Serial.println("TopBun Removed");
          playerOrder[ing] = -1;
          strip_player.setPixelColor(ing, BlankRGB);
          strip_player.show();
          printArray(playerOrder, ing);
        }
        
        if (ing == 0)
          empty = 1;
        delay(50);
  }

  // Reset Parameters
  pattyCount = 0;
  onionCount = 0;
  cheeseCount = 0;
  tomatoCount = 0;
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

void printArray(int arr[], int len)
{
  Serial.print("[");
  for (int i=0; i<len; i++)
  {
    Serial.print(arr[i]);
    if (i!= (len-1))
    Serial.print(",");
  }
  Serial.println("]");
}

int arraySize(int arr[], int arrSize)
{
  int len = arrSize / sizeof(arr[0]);
  Serial.println(len);
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
        
      // Onion
      case 2:
        strip_order.setPixelColor(i, OnionRGB);
        strip_order.show();
        break;
        
      // Cheese
      case 3:
        strip_order.setPixelColor(i, CheeseRGB);
        strip_order.show();
        break;
        
      // Tomato
      case 4:
        strip_order.setPixelColor(i, TomatoRGB);
        strip_order.show();
        break;
        
      // Lettuce
      case 5:
        strip_order.setPixelColor(i, LettuceRGB);
        strip_order.show();
        break;

      // BottomBun
      case 6:
        strip_order.setPixelColor(i, BunRGB);
        strip_order.show();
        break;
        
      // TopBun
      case 7:
        strip_order.setPixelColor(i, BunRGB);
        strip_order.show();
        break;
    }
    delay(1000);
  }
}
