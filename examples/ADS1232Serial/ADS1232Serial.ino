#include "ADS1232.h"

#define SCALE_DOUT   33
#define SCALE_SCLK   26
#define SCALE_PDWN   25
#define SCALE_SPEED  32

ADS1232 scale;


void setup() {
	
  Serial.begin(115200);
  Serial.println("ADS1232 Demo");

  Serial.println("Before setting up the scale:");
  Serial.print("read: \t\t");
  
  scale.begin(SCALE_DOUT, SCALE_SCLK, SCALE_PDWN, SCALE_SPEED);
  
  long value_long;
  scale.read(value_long);
  Serial.println(value_long);			// print a raw reading from the ADC

  float value_double;
  Serial.print("read average: \t\t");
  scale.read_average(value_double,20);
  Serial.println(value_double);  	// print the average of 20 readings from the ADC

  Serial.print("get value: \t\t");
  scale.get_value(value_double,5);
  Serial.println(value_double);		// print the average of 5 readings from the ADC minus the tare weight (not set yet)

  Serial.print("get units: \t\t");
  scale.get_units(value_double,5);
  Serial.println(value_double, 1);	// print the average of 5 readings from the ADC minus tare weight (not set) divided 
						// by the SCALE parameter (not set yet)  

  Serial.println("==========================");
  scale.set_scale(267.4f);        // this value is obtained by calibrating the scale with known weights; see the README for details
  scale.tare();				        // reset the scale to 0

  Serial.println("After setting up the scale:");

  Serial.print("read: \t\t");
  scale.read(value_long);
  Serial.println(value_long);                 // print a raw reading from the ADC

  Serial.print("read average: \t\t");
  scale.read_average(value_double,20);
  Serial.println(value_long);       // print the average of 20 readings from the ADC

  Serial.print("get value: \t\t");
  scale.get_value(value_double,5);
  Serial.println(value_double);		// print the average of 5 readings from the ADC minus the tare weight, set with tare()

  Serial.print("get Calibrated units: \t\t");
  scale.get_units(value_double,5,true);
  Serial.println(value_double, 1);        // print the average of 5 readings from the ADC minus tare weight, divided 
						// by the SCALE parameter set with set_scale

  Serial.println("Readings:");
}

void loop() {
  float value;
  Serial.print("one reading:\t");
  scale.get_units(value,1);
  Serial.print(value, 1);
  
  //float value_avg;
  //Serial.print("\t| average:\t");
  //scale.get_units(value_avg,10);
  //Serial.print(value_avg, 1);

  Serial.println();
  //scale.power_down();			        // put the ADC in sleep mode
  //delay(1000);
  //scale.power_up();
}
