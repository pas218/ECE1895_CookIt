#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>
#include <Adafruit_NeoPixel.h>
#include "ADS1X15.h"
#include "mp3tf16p.h"
#define i2c_Address 0x3c
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

#define LED_PIN 2
#define LED_PIN2 1
#define LED_COUNT 8
Adafruit_NeoPixel strip_order = Adafruit_NeoPixel(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip_player = Adafruit_NeoPixel(LED_COUNT, LED_PIN2, NEO_GRBW + NEO_KHZ800);
ADS1115 ADS(0x48);

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SH1106G display = Adafruit_SH1106G(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

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


int Patty = 0;
int Cheese = 0;
int Tomato = 0;
int Onions = 0;
int Lettuce = 0;
int BottomBun = 0;
int TopBun = 0;

int pattyCount = 0;
int cheeseCount = 0;
int tomatoCount = 0;
int onionCount = 0;
int lettuceCount = 0;
int bottomBunCount = 0;
int topBunCount = 0;

int order[] = {5, 0, 1, 4, 3, 2, 6};
// BottomBun, Patty, Cheese, Lettuce, Onion, TopBun
int playerOrder[10] = {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1};
int ing = 0;
bool winFlag = 1;
int len;

MP3Player mp3(10,11);

// the setup function runs once when you press reset or power the board
void setup() {
  delay(250);
  display.begin(i2c_Address, true); // Address 0x3C default
  display.display();
  delay(2000);
   // Bottom Bun
  pinMode(3, INPUT);
  // Top Bun
  pinMode(4, INPUT);
  pinMode(12, INPUT);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SH110X_WHITE);
  display.setCursor(0, 10);
  display.println("GAME NOW");
  display.display(); 
  delay(3000);
  display.clearDisplay();
  display.display();
  ADS.begin();
  ADS.setGain(0);


  mp3.initialize();
  #if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
  clock_prescale_set(clock_div_1);
  #endif
  strip_order.begin();
  strip_order.show(); // Initialize all pixels to 'off'
  strip_order.setBrightness(50);
  strip_player.begin();
  strip_player.show(); // Initialize all pixels to 'off'
  strip_player.setBrightness(50);
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
 mp3.playTrackNumber(3, 25);

 // End Once Bell is Pressed
 while(digitalRead(12) == HIGH)
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
 display.clearDisplay();
 display.display();
 display.setCursor(0, 15);
 display.print("Patty");
 display.display();
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
 display.clearDisplay();
 display.display();
 display.setCursor(0, 15);
 display.print("Patty Removed");
 display.display();
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
 display.clearDisplay();
 display.display();
 display.setCursor(0, 15);
 display.print("Cheese");
 display.display();
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
 display.clearDisplay();
 display.display();
 display.setCursor(0, 15);
 display.print("Cheese Removed");
 display.display();
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
 display.clearDisplay();
 display.display();
 display.setCursor(0, 15);
 display.print("Tomato");
 display.display();
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
 display.clearDisplay();
 display.display();
 display.setCursor(0, 15);
 display.print("Tomato Remove");
 display.display();
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
 display.clearDisplay();
 display.display();
 display.setCursor(0, 15);
 display.print("Onion");
 display.display();
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
 display.clearDisplay();
 display.display();
 display.setCursor(0, 15);
 display.print("Onion Remove");
 display.display();
 }

 // Lettuce
 Lettuce = ADS.readADC(1);
 if (Lettuce >=13200 && Lettuce <13600 && lettuceCount == 0)
 {
 playerOrder[ing] = 4;
 strip_player.setPixelColor(ing, LettuceRGB);
 strip_player.show();
 ing++;
 lettuceCount++;
 display.clearDisplay();
 display.display();
 display.setCursor(0, 15);
 display.print("Lettuce");
 display.display();
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
 display.clearDisplay();
 display.display();
 display.setCursor(0, 15);
 display.print("Lettuce Removed");
 display.display();
 }

 // BottomBun
 BottomBun = digitalRead(3);
 if (BottomBun == LOW && bottomBunCount == 0)
 {
 playerOrder[ing] = 5;
 strip_player.setPixelColor(ing, BunRGB);
 strip_player.show();
 ing++;
 bottomBunCount++;
 display.clearDisplay();
 display.display();
 display.setCursor(0, 15);
 display.print("Bottom Bun");
 display.display();
 }
 else if(BottomBun == HIGH && bottomBunCount != 0)
 {
 ing--;
 bottomBunCount--;
 playerOrder[ing] = -1;
 strip_player.setPixelColor(ing, BlankRGB);
 strip_player.show();
 display.clearDisplay();
 display.display();
 display.setCursor(0, 15);
 display.print("Bottom Bun Removed");
 display.display();
 }

 // TopBun
 TopBun = digitalRead(4);
 if (TopBun == LOW && topBunCount == 0)
 {
 playerOrder[ing] = 6;
 strip_player.setPixelColor(ing, BunRGB);
 strip_player.show();
 ing++;
 topBunCount++;
 display.clearDisplay();
 display.display();
 display.setCursor(0, 15);
 display.print("Top Bun");
 display.display();
 }
 else if(TopBun == HIGH && topBunCount != 0)
 {
 ing--;
 topBunCount--;
 playerOrder[ing] = -1;
 strip_player.setPixelColor(ing, BlankRGB);
 strip_player.show();
 display.clearDisplay();
 display.display();
 display.setCursor(0, 15);
 display.print("Top Bun Removed");
 display.display();
 }
 delay(50);
 }
 display.clearDisplay();
 display.display();
 display.setCursor(0, 15);
 display.print("Bell Rung Loser");
 display.display();
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
 Lettuce = ADS.readADC(1);
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
 BottomBun = digitalRead(3);
 if (BottomBun == LOW && bottomBunCount == 0)
 {
 playerOrder[ing] = 5;
 strip_player.setPixelColor(ing, BunRGB);
 strip_player.show();
 ing++;
 bottomBunCount++;
 }
 else if(BottomBun == HIGH && bottomBunCount != 0)
 {
 ing--;
 bottomBunCount--;
 playerOrder[ing] = -1;
 strip_player.setPixelColor(ing, BlankRGB);
 strip_player.show();
 }

 // TopBun
 TopBun = digitalRead(4);
 if (TopBun == LOW && topBunCount == 0)
 {
 playerOrder[ing] = 6;
 strip_player.setPixelColor(ing, BunRGB);
 strip_player.show();
 ing++;
 topBunCount++;
 }
 else if(TopBun == HIGH && topBunCount != 0)
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
}