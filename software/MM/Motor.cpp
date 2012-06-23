/* 
 Motor.cpp - Motor library
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
 + author: dviid
 + contact: dviid@labs.ciid.dk 
 */

#include <avr/io.h>

#include "Motor.h"

#define RESOLUTION 65536

bool reg_init_A = false;

void MMotor::init(MOTOR m)
{
    _m = m;
    if(!reg_init){
        //PWM, Phase and Frequency
        //Correctthese modes are preferred for motor control applications p.130
        TCCR1A = 0;
        TCCR1B |= (1 << WGM13);
        _period(64);
        reg_init = true;
    }
}

void MMotor::_period(long ms)
{
    long c = (F_CPU * ms) / 2000000;
    
    // clear prescaler reg
    TCCR1B &= ~((1 << CS10) | (1 << CS11) | (1 << CS12));
    
    if(c < RESOLUTION)                  TCCR1B |= (1 << CS10);                  // pre-s 0
    else if((c >>= 3) < RESOLUTION)     TCCR1B |= (1 << CS11);                  // pre-s 8
    else if((c >>= 3) < RESOLUTION)     TCCR1B |= (1 << CS11) | (1 << CS10);    // pre-s 64
    else if((c >>= 2) < RESOLUTION)     TCCR1B |= (1 << CS12);                  // pre-s 256
    else if((c >>= 2) < RESOLUTION)     TCCR1B |= (1 << CS12) | (1 << CS10);    // pre-s 1024
    else c = RESOLUTION - 1,            TCCR1B |= (1 << CS12) | (1 << CS10);    // pre-s 1024
    
    // TOP
    ICR1 = _p = c;
    
}

void MMotor::speed(int value) 
{
    unsigned long duty = _p * value;
    duty >>= 10;
    if(_m == MOTORA) OCR1A = duty;
    else if(_m == MOTORB) OCR1B = duty;    
    _s = value;
}

int speed()
{
    return _s;
}

void MMotor::direction(DIRECTION d)
{
    _d = d;
}
