#include <Wire.h> // Include the Arduino SPI library
#include "Adafruit_MPR121.h"
#include <Adafruit_NeoPixel.h>


// Here we'll define the I2C address of our S7S. By default it
//  should be 0x71. This can be changed, though.
const byte s7sAddress = 0x71;

void setup() {
  Wire.begin();
  // put your setup code here, to run once:
  timerSetup();
  touchSetup();
  ringSetup();
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  timerLoop();
  touchLoop();
  countdown();
}

