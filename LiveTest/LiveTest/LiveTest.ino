#include <Adafruit_NeoPixel.h>
Adafruit_NeoPixel strip_lives = Adafruit_NeoPixel(3, 0, NEO_RGBW + NEO_KHZ800);
int blankRGB = strip_lives.Color(0, 0, 0, 0);
int whiteRGB = strip_lives.Color(255, 255, 255, 255);
void setup() {
  // put your setup code here, to run once:
  strip_lives.begin();
  strip_lives.setBrightness(50);
  strip_lives.setPixelColor(0, whiteRGB);
  strip_lives.setPixelColor(1, whiteRGB);
  strip_lives.setPixelColor(2, whiteRGB);
  strip_lives.show();

}

void loop() {
  // put your main code here, to run repeatedly:
  strip_lives.setPixelColor(0, whiteRGB);
  strip_lives.setPixelColor(1, whiteRGB);
  strip_lives.setPixelColor(2, whiteRGB);
  strip_lives.show();
  delay(2000);


  strip_lives.setPixelColor(0, blankRGB);
  strip_lives.setPixelColor(1, blankRGB);
  strip_lives.setPixelColor(2, blankRGB);
  strip_lives.show();
  delay(2000);
  
}
