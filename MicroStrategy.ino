#include <QTRSensors.h>
#include <ZumoReflectanceSensorArray.h>
#include <ZumoMotors.h>

#define LED_PIN 13
#define MZ80_PIN 6
#define NUM_SENSORS 6

//Pushbutton button(ZUMO_BUTTON);
ZumoReflectanceSensorArray reflectanceSensors;
ZumoMotors motors;

// Define an array for holding sensor values.
unsigned int sensorValues[NUM_SENSORS];
unsigned int positionVal = 0;
unsigned int count = 0;
unsigned int timer = 0;
unsigned int hitObject = false;

void setup() {
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, HIGH);

  // --------------------------- Start Of The Calibration -------------------------
  reflectanceSensors.init();
  unsigned long startTime = millis();
  while (millis() - startTime < 5000)  // make the calibration take 10 seconds
  {
    reflectanceSensors.calibrate();
  }
  // --------------------------- End Of The Calibration ---------------------------
}

unsigned int mostLeftSensor() 
{
  if (sensorValues[0] < 600)
    return 1;
  else
    return 0;
}

unsigned int leftSensor() 
{
  if (sensorValues[1] < 600)
    return 1;
  else
    return 0;
}

unsigned int midLeftSensor() 
{
  if (sensorValues[2] < 600)
    return 1;
  else
    return 0;
}

unsigned int midRightSensor() 
{
  if (sensorValues[3] < 600)
    return 1;
  else
    return 0;
}

unsigned int rightSensor() 
{
  if (sensorValues[4] < 600)
    return 1;
  else
    return 0;
}

unsigned int mostRightSensor() 
{
  if (sensorValues[5] < 600)
    return 1;
  else
    return 0;
}

void turnRight() 
{
  motors.setSpeeds(130, -130);
}

void go() 
{
  motors.setSpeeds(880, 880);
}

void go_back() 
{
  motors.setSpeeds(-400, -400);
}

void stop_moving()
{
  motors.setSpeeds(0, 0);
}

void loop() 
{
  positionVal = reflectanceSensors.readLine(sensorValues);
  if(mostLeftSensor() == 1) 
  {
    go_back();
    delay(350);
    turnRight();
    delay(100);
  } 
  else 
  {
    if(digitalRead(MZ80_PIN) == 1)
    {
      timer = timer + 1;
      turnRight();
      if((timer >= 1500) && digitalRead(MZ80_PIN) == 1) 
      {
        stop_moving();
        for(int index=0; index<count; index++) 
        {
          digitalWrite(LED_PIN, HIGH);
          delay(1000);
          digitalWrite(LED_PIN, LOW);
          delay(1000);
        }
        timer = 0;
      }
      hitObject = false;
    }
    else
    {
      if(hitObject == false)
      {
        go();
        count = count + 1;
        hitObject = true;
        timer = 0;
      }
    }  
  }
}
