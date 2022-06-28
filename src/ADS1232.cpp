/**
 * V1.0.0
 * ADS1232 library for Arduino
 * ADS1232.h - Library for reading from an ADS1232 24-bit ADC.
 * Created by Max Sanchez @ June 2022. https://github.com/hardmax/ADS1232
 * Released into the public domain.
 *
 * MIT License
 * (c) 2022 Max Sanchez
 *
**/

#include <Arduino.h>
#include "ADS1232.h"

ADS1232::ADS1232() {
}

ADS1232::~ADS1232() {
}

void ADS1232::begin(byte pin_DOUT, byte pin_SCLK, byte pin_PDWN, byte pin_SPEED, Speed speed){
  _pin_DOUT = pin_DOUT;
  _pin_SCLK = pin_SCLK;
  _pin_PDWN = pin_PDWN;
  _pin_SPEED = pin_SPEED;

  pinMode(_pin_DOUT,  INPUT_PULLUP);
  pinMode(_pin_SCLK, OUTPUT);
  pinMode(_pin_PDWN, OUTPUT);
  pinMode(_pin_SPEED, OUTPUT);
   
  setSpeed(speed);
  
  power_up();

}

bool ADS1232::is_ready(void)
{
  return digitalRead(_pin_DOUT) == LOW;
}

void ADS1232::power_up(void)
{
  digitalWrite(_pin_PDWN, HIGH);
  delayMicroseconds(1);
  // Set CLK low to get the ADS1231 out of suspend
  digitalWrite(_pin_SCLK, LOW);
  delayMicroseconds(1);
}

void ADS1232::power_down(void)
{
  digitalWrite(_pin_PDWN, LOW);
  delayMicroseconds(1);
  digitalWrite(_pin_SCLK, HIGH);
  delayMicroseconds(1);
}


void ADS1232::setSpeed(Speed speed)
{
  _speed = speed;
  switch(speed)
  {
    case SLOW:
    {
      digitalWrite(_pin_SPEED, LOW);
      break;
    }
    case FAST:
    {
      digitalWrite(_pin_SPEED, HIGH);
      break;
    }
  }
 
}


/*
 * Get the raw ADC value. Can block up to 100ms in normal operation.
 * Returns 0 on success, an error code otherwise.
 */
ERROR_t ADS1232::read(long& value, bool Calibrating)
{
  int i=0;
  unsigned long start;
	unsigned int waitingTime;
	
    /* A high to low transition on the data pin means that the ADS1231
     * has finished a measurement (see datasheet page 13).
     * This can take up to 100ms (the ADS1231 runs at 10 samples per
     * second!).
     * Note that just testing for the state of the pin is unsafe.
     */
	 
	if(Calibrating){
		if(_speed==FAST) waitingTime=150;
		else waitingTime=850;
	}
	else{
		if(_speed==FAST) waitingTime=20;
		else waitingTime=150;
	}

	waitingTime+=600; //[ms] Add some extra time ( sometimes takes longer than what datasheet claims! )
	
    start=millis();
    while(digitalRead(_pin_DOUT) != HIGH)
    {
        if(millis()-start > waitingTime) return TIMEOUT_HIGH; // Timeout waiting for HIGH
        delay(0);
    }

    start=millis();
    while(digitalRead(_pin_DOUT) != LOW)
    {
        if(millis()-start > waitingTime) return TIMEOUT_LOW; // Timeout waiting for LOW
        delay(0);
    }

    // Read 24 bits
    for(i=23 ; i >= 0; i--) {
        digitalWrite(_pin_SCLK, HIGH);
        delayMicroseconds(1);
        value = (value << 1) + digitalRead(_pin_DOUT);
        digitalWrite(_pin_SCLK, LOW);
        delayMicroseconds(1);
    }

	
	if(Calibrating){
	// 2 extra bits for calibrating
		for(i=1 ; i >= 0; i--) {
			digitalWrite(_pin_SCLK, HIGH);
      delayMicroseconds(1);
			digitalWrite(_pin_SCLK, LOW); 
      delayMicroseconds(1);
		}
	}
	
    /* Bit 23 is acutally the sign bit. Shift by 8 to get it to the
     * right position (31), divide by 256 to restore the correct value.
     */
    value = (value << 8) / 256;

	if(!Calibrating){
		/* The data pin now is high or low depending on the last bit that
		 * was read.
		 * To get it to the default state (high) we toggle the clock one
		 * more time (see datasheet).
		 */
		digitalWrite(_pin_SCLK, HIGH);
    delayMicroseconds(1);
		digitalWrite(_pin_SCLK, LOW);
    delayMicroseconds(1);
	}
    return NoERROR; // Success
}

ERROR_t ADS1232::read_average(float& value, byte times, bool Calibrating) {

	long sum = 0;
	ERROR_t err;
	for (byte i = 0; i < times; i++) {
		long val;
		err = read(val, Calibrating);
		if(err!=NoERROR) return err;
		
		sum += val;
		yield();

	}
	if(times==0) return DIVIDED_by_ZERO;
	value = (float)sum / times;
	return NoERROR;
}

ERROR_t ADS1232::get_value(float& value, byte times, bool Calibrating) {
	float val = 0;
	ERROR_t err;
	err = read_average(val, times, Calibrating);
	if(err!=NoERROR) return err;
	value = val - OFFSET[0];
	return NoERROR;
}

ERROR_t ADS1232::get_units(float& value, byte times, bool Calibrating) {
	float val = 0;
	ERROR_t err;
	err = get_value(val, times, Calibrating);
	if(err!=NoERROR) return err;
	if(SCALE[0]==0) return DIVIDED_by_ZERO;
	value = val / SCALE[0];
	return NoERROR;
}

ERROR_t ADS1232::tare(byte times, bool Calibrating) {
	ERROR_t err;
	float sum = 0;
	err = read_average(sum, times, Calibrating);
	if(err!=NoERROR) return err;
	set_offset(sum);
	return NoERROR;
}

void ADS1232::set_scale(float scale) {
	SCALE[0] = scale;
}

float ADS1232::get_scale() {
	return SCALE[0];
}

void ADS1232::set_offset(float offset) {
	OFFSET[0] = offset;
}

float ADS1232::get_offset() {
	return OFFSET[0];
}
