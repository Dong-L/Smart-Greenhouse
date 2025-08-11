#include "DHT.h"
#include <Wire.h>
#include "Button.h"
#include "event_timer.h"
#include "Greenhouse.h"
#include <Servo.h>

//#define Serial SerialUSB
#define DISPLAY_ADDRESS1 0x72 //This is the default address of the OpenLCD 
#define SENSOR_INTERVAL 500UL
#define DHT_PIN_1 4
#define DHT_PIN_2 5
DHT dhtIn(DHT_PIN_1, DHT22);
DHT dhtOut(DHT_PIN_2, DHT22);
const uint32_t READ_INTERVAL = 500;

Greenhouse greenhouse;
EventTimer myTimer;
Servo myservo;

const int heaterPin = 6; // controls heaters
const int upButtonPin = 7;
const int downButtonPin = 8;
const int servoPin = 9; // controls servo

float heatPoint = greenhouse.getHeatPoint(); // tempIn = 23.00 on 3/7/19 at 18:13 pm @ 4 C outside
float coolPoint = greenhouse.getCoolPoint(); //roofUp = false;

Button upButton(upButtonPin);
Button downButton(downButtonPin);

void setup() 
{ 
  pinMode(heaterPin, OUTPUT);
  
  Wire.begin(); //Join the bus as master

  //By default .begin() will set I2C SCL to Standard Speed mode of 100kHz
  //Wire.setClock(400000); //Optional - set I2C SCL to High Speed Mode of 400kHz

  Serial.begin(9600); //Start serial communication at 9600 for debug statements
  Serial.println("OpenLCD Example Code");

  //Send the reset command to the display - this forces the cursor to return to the beginning of the display
  Wire.beginTransmission(DISPLAY_ADDRESS1);
  Wire.write('|'); //Put LCD into setting mode
  Wire.write('-'); //Send clear display command
  Wire.endTransmission();
  
  Serial.begin(115200);

  dhtIn.begin();
  dhtOut.begin();

  Serial.println(F("/setup()"));
  greenhouse.myservo.attach(9);
  greenhouse.myservo.write(0); //0 to 180
  myTimer.Start(5000);
}

void loop() 
{ 
  uint32_t currTime = millis();
  static uint32_t lastRead = 0;
  static uint32_t lastLCD = 0;
  float tempIn = dhtIn.CalcTemperature();
  float humidity = dhtIn.CalcHumidity();
  float tempOut = dhtOut.CalcTemperature();
  if(millis() - lastRead > SENSOR_INTERVAL) //SENSOR_INTERVAL //gcl: this should all be done with an event_timer
  {
    dhtIn.read();
    dhtOut.read();
    
    Serial.print(millis());
    Serial.print(tempIn);
    Serial.print("\t");
    Serial.print(tempOut);
    Serial.print("\t");    
    Serial.print(humidity);
    Serial.print("\t");
    Serial.print(greenhouse.CheckTooHot(tempIn));
    Serial.print("\t");
    Serial.print((greenhouse.getHeatPoint()) + 1);
    Serial.print("\t");
    Serial.print(myservo.read());
    Serial.print("\n");

    Wire.beginTransmission(DISPLAY_ADDRESS1); // transmit to device #1

    if(myTimer.CheckExpired()) { //gcl: a timer within a timer -- this will cause problems
      writeLCD(tempIn, tempOut, humidity);
      myTimer.Start(5000);
    }
    lastRead += SENSOR_INTERVAL;
  }

  if(greenhouse.CheckTooCold(tempIn)){
    greenhouse.EngageHeat();
    if(myservo.read() > 90){ //gcl: again, you don't have to check this -- you _know_ the roof state
      greenhouse.myservo.attach(servoPin);
      greenhouse.LowerRoof();
      greenhouse.myservo.detach();
//      myservo.attach(servoPin);
//      myservo.write(93);
//      myservo.detach();
    }
  } else if(greenhouse.CheckTooHot(tempIn)){
    greenhouse.DisengageHeat();
    if(myservo.read() < 90){
      greenhouse.myservo.attach(servoPin);
      greenhouse.RaiseRoof();
      greenhouse.myservo.detach();
    }
    
  else {
    greenhouse.DisengageHeat();
  }
  
  if(upButton.checkPressed()){
    heatPoint += 0.5;
    coolPoint += 0.5;
    greenhouse.setHeatPoint(heatPoint);
    greenhouse.setCoolPoint(coolPoint);

    writeLCD(tempIn, tempOut, humidity);
  } 
  if(downButton.checkPressed()) {
    heatPoint -= 0.5;
    coolPoint -= 0.5;
    greenhouse.setHeatPoint(heatPoint);
    greenhouse.setCoolPoint(coolPoint);

    writeLCD(tempIn, tempOut, humidity);
  }
  }
}

void writeLCD(float Ti, float To, float Hum){
  Wire.write('|'); //Put LCD into setting mode
  Wire.write('-'); //Send clear display command
    
  Wire.print("iT");
  Wire.print(Ti);
  Wire.print("oT");
  Wire.print(To);
  Wire.print("rH");
  Wire.print(Hum);
  Wire.print("cP");
  Wire.print(greenhouse.getCoolPoint());

  Wire.endTransmission();
};
