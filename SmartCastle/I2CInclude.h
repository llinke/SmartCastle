// **************************************************
// *** I2C Expander
// **************************************************
#include "Wire.h"
#include "PCF8574.h"

// Pin definitions for I2C OLED
const uint8_t I2C_EXPANDER_ADDRESS = 0x20;
const int I2C_SDA_PIN = D2;
const int I2C_SDC_PIN = D1;
const int I2C_INT_PIN = D5;
const int I2C_BUS_SPEED = 100000; // 100kHz for PCF8574

// Digital GPIO expander over I2C
PCF8574 expander;
