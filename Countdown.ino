boolean stance = false;

void countdown(void){
  if (isTimerRunning()){
    if (isPressed(0)){
      Serial.println(0);
      timerOff();
    }
  } else {
    if (isPressed(1)){ // Protocol
      timerSetSeconds(5);
      timerStart();
    } else if (isPressed(2)){ // armor repair - other
      timerSetSeconds(30);
      timerStart();
    } else if (isPressed(3)){ // armor repair - self
      timerSetSeconds(60);
      timerStart();
    } else if (isPressed(4)){ // First aid - heal
      if (stance){
        timerSetSeconds(10);
      } else {
        timerSetSeconds(15);
      }
      timerStart();
    } else if (isPressed(5)){ // First Aid - Cure Known
      if (stance){
        timerSetSeconds(15);
      } else {
        timerSetSeconds(20);
      }
      timerStart();
    } else if (isPressed(6)){ // First Aid - Cure Any
      if (stance){
        timerSetSeconds(25);
      } else {
        timerSetSeconds(30);
      }
      timerStart();
    } else if (isPressed(7)){
      stance = !stance;
      setMode(stance);
    }
  }
}

