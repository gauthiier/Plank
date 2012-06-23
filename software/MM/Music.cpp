/* 
 Music.cpp - Music library
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

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h> 
#include <math.h>

#include "Music.h"
#include "Wavetable.h"

MMusic Music;

void MMusic::init()
{
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
	
	sawTooth = false;
	
	
	
	
	
	sei(); // global interrupt enable 
}


void MMusic::setFrequency(float frequency)
{
	period1 = int((frequency * 65536.0) / SAMPLE_RATE);
	period2 = int(((frequency + (frequency * detune2)) * 65536.0) / SAMPLE_RATE);
	period3 = int(((frequency + (frequency * detune3)) * 65536.0) / SAMPLE_RATE);
	frequency1 = frequency;
	frequency2 = frequency;
	frequency3 = frequency;
}

void MMusic::setFrequency1(float frequency)
{
	period1 = int((frequency * 65536.0) / SAMPLE_RATE);
	frequency1 = frequency;
}

void MMusic::setFrequency2(float frequency)
{
	period2 = int(((frequency + (frequency * detune2)) * 65536.0) / SAMPLE_RATE);
	frequency2 = frequency;
}

void MMusic::setFrequency3(float frequency)
{
	period3 = int(((frequency + (frequency * detune3)) * 65536.0) / SAMPLE_RATE);
	frequency3 = frequency;
}



void MMusic::setDetune(float detune)
{
	detune2 = detune;
	detune3 = -detune;
	period2 = int(((frequency2 + (frequency2 * detune2)) * 65536.0) / SAMPLE_RATE);
	period3 = int(((frequency3 + (frequency3 * detune3)) * 65536.0) / SAMPLE_RATE);
}

void MMusic::setDetune2(float detune)
{
	detune2 = detune;
	period2 = int(((frequency2 + (frequency2 * detune2)) * 65536.0) / SAMPLE_RATE);
}

void MMusic::setDetune3(float detune)
{
	detune3 = detune;
	period3 = int(((frequency3 + (frequency3 * detune3)) * 65536.0) / SAMPLE_RATE);
}

void MMusic::setSawtooth()
{
	sawTooth = true;
}

void MMusic::setSine() 
{
	sawTooth = false;
}

void MMusic::setGainFloat(float value)
{
	gain = uint16_t(value * 65535);
}

void MMusic::setGain16bit(uint16_t value)
{
	gain = value;
}

void MMusic::synthInterrupt()
{
	// Frame sync low for SPI (making it low here so that we can measure lenght of interrupt with scope)
	PORTD &= ~(1<<6);
	
	// The accumulator (16bit) keeps track of the pitch by adding the 
	// the amount of "index" points that the frequency has "travelled" 
	// since the last sample was sent to the DAC, i.e. the current phase
	// of the waveform.
	accumulator1 = accumulator1 + period1;
	accumulator2 = accumulator2 + period2;
	accumulator3 = accumulator3 + period3;

	// To use the accumulator position to find the right index in the 
	// waveform look-up table, we truncate it to 12bit.
	index1 = accumulator1 >> 4;
	index2 = accumulator2 >> 4;
	index3 = accumulator3 >> 4;
	
	// Because the waveform look-up table resides in program memory
	// we most use memcpy_P to copy the data from that table to our
	// oscilator variable.
	if(!sawTooth) {
		memcpy_P(&oscil1, &sineTable[index1],2);
		memcpy_P(&oscil2, &sineTable[index2],2);
		memcpy_P(&oscil3, &sineTable[index3],2);
	}

	// Just using the index for the oscillator produces a sawtooth shape waveform
	if(sawTooth) {
		oscil1 = index1;
		oscil2 = index2;
		oscil3 = index3;
	}
	
	// The DAC formatting routine below assumes the sample to be transmitted
	// is in the higher 12 bits of the 2 byte variable, so we shift the 
	// sample up 4 bits.
	//sample += oscil[i] << 4; 
	sample = oscil1 << 2; 
	sample += oscil2 << 2; 
	sample += oscil3 << 2; 
	
	// Adding the gain (16bit) we bring it back to the 16bit frame again afterwards.
	sample = sample * gain;
	sample = sample >> 16;
	
	
	// Formatting the samples to be transfered to the MCP4921 DAC  
	dacSPI0 = sample >> 8;
	dacSPI0 >>= 4;
	dacSPI0 |= 0x30;
	dacSPI1 = sample >> 4;
	
	SPCR |= (1 << MSTR);
	
	// transmit value out the SPI port
	SPDR = dacSPI0;
	while (!(SPSR & (1<<SPIF)));  // Maybe this can be optimised
	SPDR = dacSPI1;
	while (!(SPSR & (1<<SPIF)));  // Maybe this can be optimised
	
	// Frame sync high
	PORTD |= (1<<6);
	
}



// Defining which pins the SPI interface is connected to.
#define SPI_SCK 5
#define SPI_MOSI 3

// timer 2 is audio interrupt timer
ISR(TIMER2_COMPA_vect) {
	
	OCR2A = 127;

	Music.synthInterrupt();
		
}