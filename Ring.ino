int pixels = 16;
int offset = 2;

Adafruit_NeoPixel ring = Adafruit_NeoPixel(pixels, 4, NEO_GRB + NEO_KHZ800);
int bright = 24;
int dim = 6;
int medium = bright/2;

uint32_t normalColor = getColor(bright, 0, 0);
uint32_t normalColorOff = getColor(0, 0, dim);
uint32_t normalColorPulse = getColor(medium,0, 0);
uint32_t stanceColor = getColor(bright,bright,0);
uint32_t stanceColorOff = getColor(0,dim,0);
uint32_t stanceColorPulse = getColor(medium, medium, 0);
uint32_t pulseColor = normalColorPulse;
uint32_t activeColor = normalColor;
uint32_t offColor = normalColorOff;

void ringSetup(void){
  ring.begin();
  ring.show();
  ringOff();
}

void displayRing(long remaining, long started){
  int lit;
  if (remaining < 0 ){
    remaining = 0;
  }
  lit = map(remaining, 0, started, 0, pixels) +1;
  if (remaining <= 0){
    lit = 0;
  }
  
  for (int i = 0; i < pixels; i++){
    int pixel = (i + offset) % pixels;
    if (i < lit){
      if (lit - i == 1){
        ring.setPixelColor(pixel, pulseColor);
      } else {
        ring.setPixelColor(pixel, activeColor);
      }
    } else {
      ring.setPixelColor(pixel, offColor);
    }
  }
  ring.show();
}

void ringOff(void){
  for (int i = 0; i < pixels; i++){
    ring.setPixelColor(i, offColor);
  }
  ring.show();
}

uint32_t getColor(int r, int g, int b){
  return ring.Color(r,g,b);
}

byte getRed(uint32_t color){
  return (color >> 16);
}
byte getGreen(uint32_t color){
  return (color >> 8);
}
byte getBlue(uint32_t color){
  return (color & 0x000000ff);
}

void setColor(boolean stance){
  if (stance){
    activeColor = stanceColor;
    offColor = stanceColorOff;
    pulseColor = stanceColorPulse;
  } else {
    activeColor = normalColor;
    offColor = normalColorOff;
    pulseColor = normalColorPulse;
  }
  ringOff();
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return ring.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return ring.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return ring.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}

