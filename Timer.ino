/* Countdown timer and display logic
 *  void initTimer() - Initialize timer, call from setup()
 *  void timerLoop() - run countdown, call from loop()
 *  void timerSetSeconds(long) - set the timer to a specific number of seconds
 *  void timerAdd(seconds) - add a number of seconds to the timer, up to timerDeltaMax
 *  void timerSubtract(seconds) - subtract a number of seconds from the timer, down to timerDeltaMin
 *  void timerStart() - start the countdown, turn on display
 *  void timerStop() - pause the countdown
 *  void timerOff() - turn off the display, set timer to 0
 *  boolean isTimerEnded() - determine if the countdown has reached zero.  Will only return true once
 *  void blinkColon() - internal function to blink the colon 2x second
 *  void displayTime(seconds) - internal function to display the timer
 *  void printTime(seconds) - internal function to print the timer on Serial
 */
 
// 7 segment display
boolean timerRunning = false;
boolean timerDisplay = false;
boolean timerEnded = false;
long millisRemaining = 0;
long timerLast = 0;
long oldSeconds;
int timerDelta = 0;
long millisStarted = 0;

void timerSetup(void){
  setBrightnessI2C(255);
  timerOff();
  
}

// Explicitly set a timer
void timerSetSeconds(long seconds){
  if (seconds == -1){
    millisRemaining = -1;
  } else {
    millisRemaining = seconds * 1000;
  }
  millisStarted = millisRemaining;
  oldSeconds = seconds;
  //displayTime(millisRemaining);
  timerDelta = 0;
}

void timerSetMillis(long ms){
  millisRemaining = ms;
  millisStarted = ms;
  if (ms > 0){
    oldSeconds = floor(ms/1000);
  }
  //displayTime(millisRemaining);
  timerDelta = 0;
}

// Start/resume the timer
void timerStart(void){
  timerEnded = false;
  timerDisplay = true;
  
  if (millisRemaining == -1){
    displayTime(millisRemaining);
    //printTime(millisRemaining);
    timerRunning = false;
  } else {
    timerRunning = true;
    timerLast = millis();
  }  
}

//Pause the timer
void timerStop(void){
  timerRunning = false;
  if(timerDisplay && millisRemaining > 0){
    blinkColon();
  }
}

void timerOff(void){
  timerRunning = false;
  timerSetSeconds(0);
  timerDisplay = false;
  clearDisplayI2C();
  ringOff();
}

// Run the timer
void timerLoop(void){
  if (millisRemaining > 0){
    if (timerRunning){
      long now = millis();
      millisRemaining -= (now - timerLast);
      if (millisRemaining <= 0){
        timerOff();
        timerEnded = true;
        
        
      }
      timerLast = now;
    }

    long seconds = (millisRemaining) / 1000 ;
    if (seconds < 60){
      seconds = millisRemaining / 10;
    }  
    if (oldSeconds != seconds && timerDisplay) {
      displayTime(millisRemaining);
      displayRing(millisRemaining, millisStarted);
      //printTime(millisRemaining);
    } 
    oldSeconds = seconds;
    
    if(timerDisplay && !timerRunning && millisRemaining > 0){
      blinkColon();
    }
  } else {
    ringIdle(); 
  }
}

void blinkColon(void){
  if (millis()/500 % 2 == 0){
    
    drawColon(true); 
  } else {
    drawColon(false);
  }
}

void drawColon(boolean draw){
  Wire.beginTransmission(s7sAddress);
  Wire.write(0x77);
  
  if (draw){
    Wire.write(0b00010000);
  } else {
    Wire.write(0);
  }  
  Wire.endTransmission();
}

void printTime(long time){
  if (time == -1){
    Serial.println("Infinity");
  } else {
    byte highMins, lowMins, highSecs, lowSecs;
      long seconds;
      if (time > 60000){
        seconds = (time + 999) / 1000 ; 
        highMins = seconds / 60 / 10;
        lowMins = seconds / 60 % 10;
        highSecs = seconds % 60 / 10;
        lowSecs = seconds % 60 % 10;
      } else {
        seconds = (time + 9) / 10;
        highMins = seconds / 100 / 10;
        lowMins = seconds / 100 % 10;
        highSecs = seconds % 100 / 10;
        lowSecs = seconds % 100 % 10;
      }
    Serial.print(highMins);
    Serial.print(lowMins);
    Serial.print(":");
    Serial.print(highSecs);
    Serial.println(lowSecs);
  }
}

void displayTime(long time){
  return displayTime(time, false);
}

void displayTime(long time, boolean blink){
  byte highMins, lowMins, highSecs, lowSecs;
  long seconds;
  
  if (time > 60000){
    seconds = (time + 999) / 1000 ; 
    highMins = seconds / 60 / 10;
    lowMins = seconds / 60 % 10;
    highSecs = seconds % 60 / 10;
    lowSecs = seconds % 60 % 10;
  } else {
    seconds = (time ) / 10;
    highMins = seconds / 100 / 10;
    lowMins = seconds / 100 % 10;
    highSecs = seconds % 100 / 10;
    lowSecs = seconds % 100 % 10;
  }
  Wire.beginTransmission(s7sAddress);
  Wire.write(highMins);
  Wire.write(lowMins);
  Wire.write(highSecs);
  Wire.write(lowSecs);
  Wire.endTransmission();
  
  if (blink){
    if (millis()/500 % 2 == 0){
      drawColon(true); 
    } else {
      drawColon(false);
    }
  } else {  
    drawColon(true); 
  }
}

boolean isTimerEnded(void){
  if (timerEnded){
    timerEnded = false;
    return true;
  } else {
    return false;
  }
}

boolean isTimerRunning(void){
  return timerRunning;
}

// Send the clear display command (0x76)
//  This will clear the display and reset the cursor
void clearDisplayI2C()
{
  Wire.beginTransmission(s7sAddress);
  Wire.write(0x76);  // Clear display command
  Wire.endTransmission();
}

// Set the displays brightness. Should receive byte with the value
//  to set the brightness to
//  dimmest------------->brightest
//     0--------127--------255
void setBrightnessI2C(byte value)
{
  Wire.beginTransmission(s7sAddress);
  Wire.write(0x7A);  // Set brightness command byte
  Wire.write(value);  // brightness data byte
  Wire.endTransmission();
}
