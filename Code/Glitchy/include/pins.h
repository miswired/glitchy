#ifndef PINS_H
#define PINS_H

//General system pins
#define POWER_GLITCH_PIN    15
#define AMP_IN_PIN          4
#define BIAS_IN_PIN         2

// Key pins for the power analysis example
#define KEY_1_PIN           5
#define KEY_2_PIN           6
#define KEY_3_PIN           7

// Pins for the glitching example
#define ENTER_KEY_PIN       11
#define GLITCH_SUCCESS_PIN  12

//Set up both SPI's
#define FSPI_MISO   13
#define FSPI_MOSI   POWER_GLITCH_PIN
#define FSPI_SCLK   14
#define FSPI_SS     16

#define HSPI_MISO   47
#define HSPI_MOSI   40
#define HSPI_SCLK   39
#define HSPI_SS     41


#endif
