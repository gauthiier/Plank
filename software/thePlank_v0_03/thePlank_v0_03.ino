#include <TimerOne.h>

// We will be using program memory to hold the look-up table for the waveform
#include <avr/pgmspace.h> 

// current sample rate is 15625 defined in the setup() section
#define SAMPLE_RATE 15625

// Specify highest and lowest pitch in Hz
#define LOW_PITCH 20
#define MID_PITCH 200
#define HIGH_PITCH 1000

// Specify which analog input is mapped to which physical controller
#define POTENTIOMETER 0 // Comment out if not used
#define THE_PLANK 1 // For example

uint16_t frequency = LOW_PITCH;
float period = 0;

// Slider switch
int slider = 0;

// Variables for potentiometer readings
int pot = 0;
int lastPot = 0;

// Variblles for The Plank readings:
int plank = 0;
int lastPlank = 0;

/// mOTORS

const int DIRA = 7;
const int DIRB = 8;
const int PWMA = 9;
const int PWMB = 10;


int r_old = 0;



void setup() {
  
  // arduino pocket piano code follows
  // more info on the pocket piano here: http://www.critterandguitari.com/home/store/arduino-piano.php

  cli(); // clear interrupts. this has been added, to get the pocket piano code working
  
  // set up syntheziser
  // this is the timer 2 audio rate timer, fires an interrupt at 15625 Hz sampling rate
  TIMSK2 = 1<<OCIE2A;                      // interrupt enable audio timer
  OCR2A = 127;
  TCCR2A = 2;                              // CTC mode, counts up to 127 then resets
  TCCR2B = 0<<CS22 | 1<<CS21 | 0<<CS20;    // different for atmega8 (no 'B' i think)
  
  
  // OUTPUTS
  // sck + mosi + ss
  DDRB = (1 << DDB2) | (1 << DDB3) | (1 << DDB5);
  // dac_cs output
  DDRD = (1 << DDD6);
  
  
  // set up SPI port
  SPCR = 0x50;
  SPSR = 0x01;
    
  
  //SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPR0);  
  
//  DDRB |= 0x2E;                            // PB output for DAC CS, and SPI port
                                           // DAC SCK is digital 13, DAC MOSI is digital 11, DAC CS is digital 10
                                                                                    
  // DAC frame sync HIGH, so that the SPI port doesn't start wirting straight away
  PORTD |= (1<<6);
  
  sei(); // global interrupt enable 

  // motors

  pinMode(DIRA, OUTPUT);
  pinMode(DIRB, OUTPUT);
  
  Timer1.initialize(64);
  
  Timer1.pwm(PWMA, 300);
  Timer1.pwm(PWMB, 300);
  
  Serial.begin(9600);
  
  //frequency = 440;
  //period = (frequency * 65536.0) / SAMPLE_RATE;
  
  
  
}

void loop() {

  updatePhysics();
  
  // Check if the potentiometer has a new value and update
  // the frequency if it has.
  /*
  if(checkPot(1024)) {
    frequency = map(pot, 0, 1023, LOW_PITCH, HIGH_PITCH);
  }
  */
    
}

// Check whether the potentiometer has a new value since last check.
// Returns boolean true or false.


boolean checkPot(int resolution) {
  
  pot = analogRead(POTENTIOMETER);
  int potDiff = pot-lastPot;
  potDiff = abs(potDiff);
  //Serial.println(potDiff);
  int threshold = 1024/resolution; 
  if(potDiff > threshold) {
    lastPot = pot;
    return true;
  } else {
    pot = lastPot;
    return false;
  }
}

void updatePhysics() {
  // we put our physics code here. 
 
  static int cnt = 0; 
  
  int r = analogRead(0);  
  int m = (680 - 538) / 2 + 538;
  
  if(r < m && r_old >= 0) {
    digitalWrite(DIRA, HIGH);
  } else if(r > m && r_old <= 0) {
    digitalWrite(DIRA, LOW);
  }
  
  //Timer1.pwm(PWMA, abs(r));
  
  r_old = m - r;
  
  
  frequency = map(r, 538, 680, LOW_PITCH, HIGH_PITCH);
  period = (frequency * 65536.0) / SAMPLE_RATE;
  
  if(cnt == 5000) {
    Serial.println(frequency);
    cnt = 0;
  }
  cnt++;
  
  
}

