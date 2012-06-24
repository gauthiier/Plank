/* 
 Music.h - Music library
 Copyright (c) 2012 Copenhagen Institute of Interaction Design. 
 All right reserved.
 
 This library is free software: you can redistribute it and/or modify
 it under the terms of the GNU Lesser Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.
 
 This library is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU Lesser Public License for more details.
 
 You should have received a copy of the GNU Lesser Public License
 along with Foobar.  If not, see <http://www.gnu.org/licenses/>.
 
 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 + author: Jakob Bak
 + contact: j.bak@ciid.dk
 */


// current sample rate is 15625 as defined in the init() section
#define SAMPLE_RATE 15625

// Number of oscillators. Set not higher than 4.
#define NUM_OSCILLATORS 2

class MMusic {    
public:
    void init();
	void synthInterrupt();
	void setFrequency(float frequency);
	void setFrequency1(float frequency1);
	void setFrequency2(float frequency2);
	void setFrequency3(float frequency3);
	void setDetune(float detune);
	void setDetune2(float detune);
	void setDetune3(float detune);
	void setSawtooth();
	void setSine();
	void setGainFloat(float value); // 0.0 - 1.0
	void setGain16bit(uint16_t value); // 0 - 65535
	float getGainFloat();
	uint16_t getGain16bit();
	
    
private:
	// Waveform variables
	bool sawTooth;
	
	//Frequency variable
	uint16_t period1;
	uint16_t period2;
	uint16_t period3;
	float frequency1;
	float frequency2;
	float frequency3;
	float detune2;
	float detune3;
	
	// variables for oscillator
	uint16_t accumulator1;
	uint16_t accumulator2;
	uint16_t accumulator3;
	uint16_t index1;
	uint16_t index2;
	uint16_t index3;
	uint32_t oscil1;
	uint32_t oscil2;
	uint32_t oscil3;
	uint16_t gain;
	
	// final sample that goes to the DAC    
	uint32_t sample;
	
	// the two bytes that go to the DAC over SPI
	uint8_t dacSPI0;
	uint8_t dacSPI1;

    
};

extern MMusic Music;
