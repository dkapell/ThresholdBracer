// You can have up to 4 on one i2c bus but one is enough for testing!
Adafruit_MPR121 cap = Adafruit_MPR121();

// Keeps track of the last pins touched
// so we know when buttons are 'released'
uint16_t lasttouched = 0;
uint16_t currtouched = 0;

boolean touched[12];
boolean pressed[12];

void touchSetup(void){
  cap.begin(0x5A);
}

void touchLoop(void){
  currtouched = cap.touched();
  for (uint8_t i=0; i<12; i++) {
    // it if *is* touched and *wasnt* touched before, alert!
    if ((currtouched & _BV(i)) && !(lasttouched & _BV(i)) ) {
      touched[i] = true;
      pressed[i] = true;      
    }
    // if it *was* touched and now *isnt*, alert!
    if (!(currtouched & _BV(i)) && (lasttouched & _BV(i)) ) {
      touched[i] = false;
      pressed[i] = false;
    }
  }
  lasttouched = currtouched;
}

boolean isPressed(byte input){
  if (pressed[input]){
    pressed[input] = false;
    return true;
  } else {
    return false;
  }
}

