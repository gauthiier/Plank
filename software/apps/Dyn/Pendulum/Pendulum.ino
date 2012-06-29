// "Pendulum" - spring-mass oscillator
//

#include <TimerOne.h>
#include <Motor.h>
float dt = .005; //time per cycle
float m = 4.0; //time constant was .002

float k = 1.0;  // spring constant was .15
float xf,vf,ff; // floating point versions of pendulum x and v and force
int x; // input from analogRead();
int f; // integer version of force
int duty; // abs(f)
byte c=0;  //counts up until 0 then prints

byte incomingByte;


void setup() 
{
  //Timer1.initialize(64); //64 microsecond period
  //MotorA.torque(100); //initializes Timer1, pinModes
  Serial.begin(9600);
  //initialize position (xf), velocity (vf), force (ff)
  x = analogRead(A0) - 512; //middle of 0-1-23 range
  xf = x;
  vf = 0.0;
  ff = 0.0;
  
}

void loop(){
  x = analogRead(0) - 512; // position [0-1024]
  ff = k*(x - xf); // spring
  vf = vf + (ff/m)*dt; // integrate F/m to get velocity
  xf = xf + vf*dt; // integrate velocity to get position
  
  f = max(ff,-1024);
  f = min(f,1024);
  duty = abs(f);
  
  
  MotorA.torque(duty);
  if(ff>0)MotorA.direction(FORWARD);
  else MotorA.direction(BACKWARD);
  
  


if (c++ == 0){
  Serial.println(x + ' ' + xf);
}
/*
if (Serial.available() > 0) {
    incomingByte = Serial.read();
    if(incomingByte == '\''){
      tf = tf * 1.1;
      Serial.println(tf);
    }
    if(incomingByte == ';'){
     tf = tf / 1.1;
     Serial.println(tf);
    }
    if(incomingByte == '.'){
      kf = kf / 1.1;
      Serial.println(tf);
    }
    if(incomingByte == '/'){
      kf = kf * 1.1;
      Serial.println(tf);
    }
  }
  */
}
