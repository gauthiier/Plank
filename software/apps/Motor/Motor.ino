#include <TimerOne.h>

/// mOTORS
const int DIRA = 7;
const int DIRB = 8;
const int PWMA = 9;
const int PWMB = 10;

void setup() 
{
  pinMode(DIRA, OUTPUT);
  pinMode(DIRB, OUTPUT);
  
  //Timer1.initialize(64);
  
  //Timer1.pwm(PWMA, 300);
  //Timer1.pwm(PWMB, 300);
  
  digitalWrite(DIRA, HIGH);
  
}

void loop() 
{
  static int j = 1;
  static long cnt = 0;
  
  if(cnt == 50000) {
    j = -1;
    digitalWrite(DIRA, HIGH);
    digitalWrite(DIRB, LOW);
  } else if(cnt == 0) {
    j = 1;
    digitalWrite(DIRA, LOW);
    digitalWrite(DIRB, HIGH);
  }
  cnt += j;  
  
}
  
