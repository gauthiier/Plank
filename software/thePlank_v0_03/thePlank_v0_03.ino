//#include <TimerOne.h>

// We will be using program memory to hold the look-up table for the waveform
#include <avr/pgmspace.h> 

// The Music object is automatically instantiated when the header file is included.
// Make calls to the Music object with "Music.function(args)"
#include <Music.h>

// Specify highest and lowest pitch in Hz
#define LOW_PITCH 20
#define MID_PITCH 200
#define HIGH_PITCH 1000

uint16_t frequency = LOW_PITCH;
float detune = 1;

/// mOTORS

const int DIRA = 7;
const int DIRB = 8;
const int PWMA = 9;
const int PWMB = 10;


int r_old = 0;



void setup() {
  
  Music.init();

  // motors

  pinMode(DIRA, OUTPUT);
  pinMode(DIRB, OUTPUT);
  
  //Timer1.initialize(64);
  
  //Timer1.pwm(PWMA, 300);
  //Timer1.pwm(PWMB, 300);
  
  Serial.begin(9600);
  
  Music.setSawtooth();
  //Music.setSine();
  
  Music.setFrequency1(70);
  Music.setFrequency2(140);
  Music.setFrequency3(210);
  Music.setDetune2(0.00383671);
  Music.setDetune3(-0.0043276);
    
}

void loop() {

  updatePhysics();
  
  
  // This section creates a staccatoed shuffled rythm using only the gain of the synth and roll-over.  
  static int cnt = 0; 

  if(cnt == 1000) {
    //Serial.println(value);
    cnt = 0;
  }
  Music.setGain16bit(65535-(cnt*120));
  cnt++;
    
}


void updatePhysics() {
  // we put our physics code here. 
   
  int r = analogRead(0);  
  int m = (680 - 538) / 2 + 538;
  
  if(r < m && r_old >= 0) {
    digitalWrite(DIRA, HIGH);
  } else if(r > m && r_old <= 0) {
    digitalWrite(DIRA, LOW);
  }
  
  //Timer1.pwm(PWMA, abs(r));
  
  r_old = m - r;
  
  
  float value = map(r, 20, 800, -0.02, 0.02);
  //Music.setDetune(value);
  
  
}

