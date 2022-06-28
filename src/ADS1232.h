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

#ifndef ADS1232_h
#define ADS1232_h

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

enum ERROR_t {
	NoERROR,
	TIMEOUT_HIGH,     // Timeout waiting for HIGH
	TIMEOUT_LOW,      // Timeout waiting for LOW
	WOULD_BLOCK,      // weight not measured, measuring takes too long
	STABLE_TIMEOUT,   // weight not stable within timeout
	DIVIDED_by_ZERO    
};

enum Speed{
	SLOW = 0,
	FAST
};

class ADS1232
{
	public:
	
		ADS1232();
		
		virtual ~ADS1232();
		
        // Initialize library
        void begin(byte pin_DOUT, byte pin_SCLK, byte pin_PDWN, byte pin_SPEED, Speed = SLOW);


		// check if chip is ready
		// from the datasheet: When output data is not ready for retrieval, digital output pin DOUT is high. Serial clock
		// input PD_SCK should be low. When DOUT goes to low, it indicates data is ready for retrieval.
		bool is_ready();
		
		void setSpeed(Speed speed);
		
		// waits for the chip to be ready and returns a reading
		ERROR_t read(long& value, bool Calibrating = false);

		// returns an average reading; times = how many times to read
		ERROR_t read_average(float& value, byte times = 10, bool Calibrating = false);
		
		// returns (read_average() - OFFSET), that is the current value without the tare weight; times = how many readings to do
		ERROR_t get_value(float& value, byte times = 1, bool Calibrating = false);

		// returns get_value() divided by SCALE, that is the raw value divided by a value obtained via calibration
		// times = how many readings to do
		ERROR_t get_units(float& value, byte times = 1, bool Calibrating = false);

		// set the OFFSET value for tare weight; times = how many times to read the tare value
		ERROR_t tare(byte times = 10, bool Calibrating = false);

		// set the SCALE value; this value is used to convert the raw data to "human readable" data (measure units)
		void set_scale(float scale = 1.f);

		// get the current SCALE
		float get_scale();

		// set OFFSET, the value that's subtracted from the actual reading (tare weight)
		void set_offset(float offset = 0);

		// get the current OFFSET
		float get_offset();
		
		// puts the chip into power down mode
		void power_down();

		// wakes up the chip after power down mode
		void power_up();


	private:
		int _pin_DOUT;
		int _pin_SCLK;
		int _pin_PDWN;
		int _pin_SPEED;
	
		float OFFSET[2] = {};	// used for tare weight
		float SCALE[2] = {1,1};	// used to return weight in grams, kg, ounces, whatever
	  
		Speed _speed ;
};

#endif /* #ifndef ADS1232_h */

