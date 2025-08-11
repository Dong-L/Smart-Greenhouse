class Button {
  private:
    int _pin = -1;
    int prevButtonState = 1;
    bool isRunning = false;
    bool pressed;

  public:
    Button(uint8_t pin)
    {
      _pin = pin;
      pinMode(_pin, INPUT_PULLUP); // LOW when pressed, HIGH when not pressed
    }

    bool checkPressed(void)
    {
      bool retVal = false;

      int currButtonState = digitalRead(_pin);
      if(prevButtonState != currButtonState)
      {
        delay(50); //this is a cheat for debouncing
        if(currButtonState == LOW) retVal = true;  //button is down => pin reads LOW
      }
      prevButtonState = currButtonState;

      return retVal;
    }

//     void HandleUpButton(void)
//     {
//         greenhouse.heatPoint += 0.5;
//         greenhouse.coolPoint += 0.5;      
//     }    
//
//     void HandleDownButton(void)
//     {
//         greenhouse.heatPoint -= 0.5;
//         greenhouse.coolPoint -= 0.5;      
//     }  
};

    // void HandleButtonPressed(void)
    // {
    //   if(currentState == )
    //   {
    //     Serial.println("Arming!");

    //     //take the action: light the LED
    //     digitalWrite(ledPin, HIGH);    
        
    //     currentState = ALARM_ARMED; //Don't forget to change the state!
    //   }
    // }
