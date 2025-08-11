class EventTimer
{
  private:
  bool isRunning = false;
  uint32_t duration;
  uint32_t startTime;
 
//We use un signed interger because of the long range un signed will
// confirm that the values are positive numbers
  
  public:
    void Start(uint32_t dur)
    {
      startTime = millis();
      duration = dur;
      isRunning = true;
    }
    
    void Cancel(void) {isRunning = false;}
    
    bool CheckExpired(void)
    {
      bool retVal = false;
      uint32_t currTime = millis();
      if(currTime - startTime >= duration && isRunning) // relative time 
      {
        retVal = true;
        isRunning = false;    
      }
    
      return retVal;
    }
};
