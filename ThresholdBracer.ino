#include <Wire.h> // Include the Arduino SPI library
#include "Adafruit_MPR121.h"
#include <Adafruit_NeoPixel.h>

// Common defines
#include "thresholdBracer.h"

// Here we'll define the I2C address of our S7S. By default it
//  should be 0x71. This can be changed, though.
const byte s7sAddress = 0x71;

// Startup idle animation
byte idleAnimation = RAINBOW;

void setup() {
  Wire.begin();
  timerSetup();
  touchSetup();
  ringSetup();
  Serial.begin(9600);
}

void loop() {
  timerLoop();
  touchLoop();
  countdown();
}

