//#include <TimerOne.h>

#include <Motor.h>

void setup() 
{

  MotorB.torque(100);  
  
}

void loop() 
{
  static int j = 1;
  static long cnt = 0;
  
  if(cnt == 50000) {
    j = -1;
    MotorB.direction(FORWARD);
  } else if(cnt == 0) {
    j = 1;
    MotorB.direction(BACKWARD);
  }
  cnt += j;  
  
}
  
