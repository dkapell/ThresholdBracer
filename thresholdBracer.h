struct RGB {
  byte r;
  byte g;
  byte b;
};

RGB getPulseColor(uint32_t color, byte brightness);
RGB getPulseColor(RGB color, byte brightness);

#define STATIC 0
#define PULSE 1
#define SPIN 2
#define RAINBOW 3
