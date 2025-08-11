#include <Servo.h>

class Greenhouse
{
//  if(greenhouse.CheckTooCold()) greenhouse.EngageHeat();
  private:
    const int heaterPin = 6;
    const int servoPin = 9;
    float heatPoint = 18.00;        // tempIn = 23.00 on 3/7/19 at 18:13 pm @ 4 C outside; T_in: 22.10, 20.00 (lower)
    float coolPoint = 15.00;
    
  public:
      Servo myservo;                 
      enum {CONHOFF, COFFHOFF, COFFHON};
      int currentState = COFFHOFF;

      float getHeatPoint(){
        return heatPoint;
      }

      float getCoolPoint(){
        return coolPoint;
      }

      void setHeatPoint(float hp){
        heatPoint = hp;
      }

      void setCoolPoint(float cp){
        coolPoint = cp;
      }

      bool isHeatOn()
      {
        if(currentState == COFFHON){
          return true;
        } else {
          return false;
        }
      }

      bool isCoolOn()
      {
        if(currentState == CONHOFF){
          return true;
        } else {
          return false;
        }
      }
      
      bool CheckTooCold(float temp)
      {
        bool retVal = false;
        
        if(temp < coolPoint - 1) // hysteresis, wait for temp to fall here before closing lid
        {
          retVal = true;   
        }
        return retVal;
      }

      bool CheckTooHot(float temp)
      {
        bool retVal = false;
        
        if(temp > heatPoint + 1) // more hysteresis, wait for temp to rise here before turning off heater
        {
          retVal = true;   
        }
        return retVal;
      }

      EngageHeat() 
      {
        digitalWrite(heaterPin, HIGH);
      }

      DisengageHeat()
      {
        digitalWrite(heaterPin, LOW);
      }

      RaiseRoof()
      {
        myservo.write(180);
      }

      LowerRoof()
      {
        myservo.write(93);
      } 
};
