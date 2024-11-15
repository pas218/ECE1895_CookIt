#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>
#include "ADS1X15.h"
#define i2c_Address 0x3c

ADS1115 ADS(0x48);

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SH1106G display = Adafruit_SH1106G(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void setup() {

  delay(250); // wait for the OLED to power up
  display.begin(i2c_Address, true); // Address 0x3C default
 //display.setContrast (0); // dim display
  display.display();
  delay(2000);

  pinMode(4, INPUT);
  display.clearDisplay();

  display.setTextSize(1);
  display.setTextColor(SH110X_WHITE);
  display.setCursor(0, 10);
  // Display static text
  display.println("I2C ADC Test");
  display.display(); 
  delay(3000);
  display.clearDisplay();
  display.display();
  ADS.begin();
}

void loop() {
  ADS.setGain(0);
  // Collect Input ftom pin 9 connected as button and pulldown resistor to ground
  int input = digitalRead(4);
  if (input == LOW)
  {
    display.clearDisplay();
    display.display();
    display.setCursor(0, 15);
    display.print("A0 = ");
    display.print(ADS.readADC(0));
    display.display();

    display.setCursor(0, 30);
    display.print("A1 = ");
    display.print(ADS.readADC(1));
    display.display();
  }
}