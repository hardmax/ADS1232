# ADS123X
A Simple Non-blocking Library for reading from Texas Instruments ADS1232 MultiChannel 24-bit ADC For Bridge Sensors.

To simplify its use, only the DOUT, SCLK, PDWN and SPEED communication connections are physically required.

The library has been adapted for use with faster processors such as the ESP32 and uses functions such as Offset calibration which can minimize Input Offset Error errors to the level of noise at any temperature.

one function example:
  ERROR_t get_units(float& value, byte times = 1, bool Calibrating = false);

Errors returned by the library include:

```cpp
NoERROR,
TIMEOUT_HIGH,     // Timeout waiting for HIGH
TIMEOUT_LOW,      // Timeout waiting for LOW
WOULD_BLOCK,      // weight not measured, measuring takes too long
STABLE_TIMEOUT,   // weight not stable within timeout
DIVIDED_by_ZERO 
```

based on these libraries:

HX711: by bodge -> https://github.com/bogde/HX711 

ADS123X by Hamid Saffari -> https://github.com/HamidSaffari/ADS123X

ADS1232: by Jeffrey M. Kubascik -> https://github.com/jeffkub/beer-gauge/tree/master/firmware/beer-gauge
