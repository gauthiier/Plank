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

void setup() {
  
  Music.init();
  
  Music.setSawtooth();
  //Music.setSine();

  Music.setFrequency1(70);
  Music.setFrequency2(140);
  Music.setFrequency3(210);
  Music.setDetune2(0.00383671);
  Music.setDetune3(-0.0043276);
    
}

void loop() {
    
  // This section creates a staccatoed shuffled rythm using only the gain of the synth and roll-over.  
  static int cnt = 0; 

  if(cnt == 1000) {
    cnt = 0;
  }
  
  Music.setGain16bit(65535-(cnt*120));
  cnt++;   
}
