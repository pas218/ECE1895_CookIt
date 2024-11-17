// Class to keep track of miscellaneous items

class Misc
{
  private:

    int score;
    int lives;
    bool buttonPressed;
    
    // Neopixels to keep track of lives and show fire when doing Cook-It
    Adafruit_NeoPixel strip_lives = Adafruit_NeoPixel(8, 1, NEO_GRB + NEO_KHZ800);   // lives
    Adafruit_NeoPixel strip_fire = Adafruit_NeoPixel(8, 2, NEO_GRB + NEO_KHZ800);    // fire
    int startButtonInput;

    int sizeFire;
    int sizeLives;
    // RGB Colors for fire
    uint32_t redRGB;
    uint32_t yellowRGB;
    uint32_t orangeRGB;
    uint32_t whiteRGB;
    uint32_t blankRGB;

  public:

    Misc(int startScore, int startLives, int startbuttonInput);
    bool increaseScore();
    int getScore();
    void decreaseLives();
    int getLives();
    void turnOnFire();
    void turnOffFire();
    bool startButtonPressed();
};


Misc::Misc(int startScore, int startLives, int startbuttonInput){
  
  score = startScore;
  lives = startLives;
  buttonPressed = false;

  // Get variables for lives/scores and their neopixels
  sizeFire = 8;
  sizeLives = 3;
  redRGB    = strip_fire.Color(255, 0, 0);
  yellowRGB = strip_fire.Color(255, 200, 0);
  orangeRGB = strip_fire.Color(255, 255, 255);
  whiteRGB = strip_fire.Color(255, 255, 255);
  blankRGB = strip_fire.Color(0, 0, 0);

  // Setup the lives and scores NeoPixels
  strip_lives.begin();
  for (int i = 0; i < sizeLives; i++) {
    strip_lives.setPixelColor(0, redRGB);
  }
  strip_lives.show();  // Initialize all pixels to 'off'
  strip_lives.setBrightness(50);

  strip_fire.begin();
  for (int i = 0; i < sizeFire; i++) {
    strip_fire.Color(0, 0, 0);
  }
  strip_fire.show();  // Initialize all pixels to 'off'
  strip_fire.setBrightness(50);


}

// If made a transition from 99 to 0, then return 1
bool Misc::increaseScore(){
  bool madeFullCycle = false;
  if (score >= 99){
    score = 0;
    madeFullCycle = true;
  }
  else{
    score++;
  }
  return madeFullCycle;
}

int Misc::getScore(){
  return score;
}

// Decrease lives and output the lives to the neopixels.
void Misc::decreaseLives(){
  lives = lives - 1;

  for (int i = 0; i < sizeLives; i++) {
    if (i < lives){
      strip_lives.setPixelColor(i, redRGB);
    }
    else{
      strip_lives.setPixelColor(i, blankRGB);
    }
  }
}

int Misc::getLives(){
  return lives;
}

void Misc::turnOnFire(){

  strip_fire.setPixelColor(0, yellowRGB);
  strip_fire.setPixelColor(1, redRGB);
  strip_fire.setPixelColor(2, whiteRGB);
  strip_fire.setPixelColor(3, orangeRGB);
  strip_fire.setPixelColor(4, orangeRGB);
  strip_fire.setPixelColor(5, whiteRGB);
  strip_fire.setPixelColor(6, redRGB);
  strip_fire.setPixelColor(7, yellowRGB);
  strip_fire.show();
}
void Misc::turnOffFire(){

  for (int i = 0; i < sizeFire; i++){
    strip_fire.setPixelColor(i, blankRGB);
  }
  strip_fire.show();
}

bool Misc::startButtonPressed(){
  if(digitalRead(startButtonInput) == HIGH){
    return true;
  }
  else{
    return false;
  }
}



