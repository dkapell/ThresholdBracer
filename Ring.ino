int pixels = 16;
int offset = 2;

Adafruit_NeoPixel ring = Adafruit_NeoPixel(pixels, 4, NEO_GRB + NEO_KHZ800);
int bright = 24;
int dim = 2;
int medium = bright/3*2;

uint32_t normalColor = getColor(bright, 0, 0);
uint32_t normalColorOff = getColor(0, 128, 255);
uint32_t normalColorPulse = getColor(medium,0, 0);
uint32_t stanceColor = getColor(bright,bright,0);
uint32_t stanceColorOff = getColor(0,255,0);
uint32_t stanceColorPulse = getColor(medium, medium, 0);
uint32_t pulseColor = normalColorPulse;
uint32_t activeColor = normalColor;
uint32_t offColor = normalColorOff;

long ringTimer = millis();
long pulseTime = 2000;
byte ringLightsIncreasing;

int ringAnimationCycle = 0;
long ringAnimationDelay = 100;
byte elements = 3;

void ringSetup(void){
  ring.begin();
  ringOff();
}

void setMode(boolean stance){
  if (stance){
    activeColor = stanceColor;
    offColor = stanceColorOff;
    pulseColor = stanceColorPulse;
    idleAnimation = PULSE;
  } else {
    activeColor = normalColor;
    offColor = normalColorOff;
    pulseColor = normalColorPulse;
    idleAnimation = SPIN;
  }
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
  RGB offclr = getPulseColor(offColor, medium);
  
  for (int i = 0; i < pixels; i++){
    int pixel = (i + offset) % pixels;
    if (i < lit){
      if (lit - i == 1){
        ring.setPixelColor(pixel, pulseColor);
      } else {
        ring.setPixelColor(pixel, activeColor);
      }
    } else {
      setPixel(pixel, offclr);
    }
  }
  ring.show();
}

void ringIdle(void){
  switch(idleAnimation){
    case STATIC: break;
    case PULSE: pulseRing(); break;
    case SPIN: spin(); break;
    case RAINBOW: rainbow(); break;
  }
}

void pulseRing(void){
  long now = millis();
  if ((now - ringTimer) >= pulseTime){
    ++ringLightsIncreasing %= 2;
    ringTimer = now;
  }
  long currentStep = now - ringTimer;
  long fader = currentStep*(bright/2)/pulseTime;
  
  byte brightness;
  if (ringLightsIncreasing){
    brightness = (bright/2) - fader; // fade out
  } else {
    brightness = fader; // fade in
  }
  RGB clr = getPulseColor(offColor, brightness);

  for (int i = 0; i < pixels; i++){
    int pixel = (i + offset) % pixels;
    setPixel(pixel, clr);
  }
  ring.show();
}


void rainbow(void){
  long now = millis();
  if ((now - ringTimer) < ringAnimationDelay){
    return;
  }
  ringTimer = now;

  ringAnimationCycle = ++ringAnimationCycle % 255;
  
  for(int i=0; i<pixels; i++) {
    int p = (i+ringAnimationCycle) & 255;
    setPixel(i,  getPulseColor(Wheel(map(p, 0, pixels, 0, 255)), medium));
  }
  ring.show();
}

void spin(){
  long now = millis();
  if ((now - ringTimer) < ringAnimationDelay){
    return;
  }
  ringTimer = now;
  ringAnimationCycle = ++ringAnimationCycle % (pixels/elements);
  for(int i=0; i<pixels; i++) {
    RGB clr = getSpinColor(i, medium, offColor );
    setPixel(i,  getSpinColor(i, medium, offColor ));
  }
  ring.show();
}


void ringOff(void){
  RGB clr = getPulseColor(offColor, medium);
  for (int i = 0; i < pixels; i++){
    if (idleAnimation == RAINBOW){
      clr = getPulseColor(Wheel(map(i, 0, pixels, 0, 255)), medium);
    }
    setPixel(i,  clr);
    
  }
  ring.show();
}

void setPixel(byte pixel, RGB clr){
  ring.setPixelColor(pixel, clr.r, clr.g, clr.b);
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

RGB getPulseColor(RGB c, byte brightness){
  return getPulseColor(getColor(c.r, c.g, c.b), brightness);
}
RGB getPulseColor(uint32_t color, byte brightness){
  byte ringRed = getRed(color);
  byte ringGreen = getGreen(color);
  byte ringBlue = getBlue(color);

  byte red = ringRed;
  byte green = ringGreen;
  byte blue = ringBlue;

  if (brightness != 255){
    red = map(red, 0, 255, 0, brightness);
    green = map(green, 0, 255, 0, brightness);
    blue = map(blue, 0, 255, 0, brightness);
  }
  if (ringRed){
    red = max(dim, red);
  }
  if (ringGreen){
    green = max(dim, green);
  }
  if (ringBlue){
    blue = max(dim, blue);
  }
  return {red, green, blue}; 
}

RGB getSpinColor(int pos, byte brightness, uint32_t color){
  int cycle = ringAnimationCycle;
  int chunkSize = pixels / elements;
  int index = pos % chunkSize;
  int diffPos = cycle - index;
  int diffNeg = (chunkSize + cycle) - index;
  int absPos = abs(diffPos);
  int absNeg = abs(diffNeg);
  int diff = min(absPos, absNeg);
  
  byte ringRed = getRed(color);
  byte ringGreen = getGreen(color);
  byte ringBlue = getBlue(color);
  
  byte red = 0;
  byte green = 0;
  byte blue = 0;
  
  if (index == cycle){
      red = ringRed;
      green = ringGreen;
      blue = ringBlue;
    
  } else if (diff < 5){
    int diffFade = (diff*3);
    red = ringRed/diffFade;
    green = ringGreen/diffFade;
    blue = ringBlue/diffFade;
    
  }  else {
      red = min(1, ringRed);
      green = min(1, ringGreen);
      blue = min(1, ringBlue);
  }
  
  if (brightness != 255){
    red = map(red, 0, 255, 0, brightness);
    green = map(green, 0, 255, 0, brightness);
    blue = map(blue, 0, 255, 0, brightness);
  }

  if (ringRed){
    red = max(dim, red);
  }
  if (ringGreen){
    green = max(dim, green);
  }
  if (ringBlue){
    blue = max(dim, blue);
  }
  return {red, green, blue};
} 
