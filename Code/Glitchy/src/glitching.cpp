/*
This file contains helper functions for the power glitching features. 

Normally in a C program this would be a self contained module.
But as I am trying to keep this more beginnner friendly and use the Arduino IDE, there are some limitations on how it links files
and auto creates prototypes. So the main instantiation is still in the main ino.

This file is licensed under the Creative Commons Attribution-Share Alike 3.0 Unported license.
https://creativecommons.org/licenses/by-sa/3.0/deed.en

Attribution: Miswired
Special thanks goes to Rui Santos and the RandomNerdTutorials site. The work here was inspired by those examples.

    You are free:
        to share – to copy, distribute and transmit the work
        to remix – to adapt the work
    Under the following conditions:
        attribution – You must give appropriate credit, provide a link to the license, and indicate if changes were made. You may do so in any reasonable manner, but not in any way that suggests the licensor endorses you or your use.
        share alike – If you remix, transform, or build upon the material, you must distribute your contributions under the same or compatible license as the original. https://creativecommons.org/share-your-work/licensing-considerations/compatible-licenses
*/
#include "driver/gpio.h"
#include <Arduino.h>
#include "pins.h"
#include "global.h"
#include "SPI.h"
#include "glitching.h"
#include "web_server.h"



#define SHORTEST_DELAY_NS_DEFAULT               200
#define LONGEST_DELAY_NS_DEFAULT                1500
#define PAUSE_TIME_BETWEEN_GLITCHING_MS_DEFAULT 2000
#define GLITCH_TIME_STEP_SIZE_NS_DEFAULT        10
#define NUM_OF_ATTEMPTS_AT_EACH_STEP_DEFAULT    3

//The ESP32-S3 doesn't have an VSPI, they define FSPI instead

SPIClass spi_fspi = SPIClass(FSPI);

glitch_param_t g_glitch_param;

void init_glitch()
{
  g_glitch_param.shortest_delay_ns = SHORTEST_DELAY_NS_DEFAULT;
  g_glitch_param.longest_delay_ns = LONGEST_DELAY_NS_DEFAULT;
  g_glitch_param.pause_time_between_glitching_ms = PAUSE_TIME_BETWEEN_GLITCHING_MS_DEFAULT;
  g_glitch_param.glitch_time_step_size_ns = GLITCH_TIME_STEP_SIZE_NS_DEFAULT;
  g_glitch_param.num_of_attempts_at_each_step = NUM_OF_ATTEMPTS_AT_EACH_STEP_DEFAULT;


  //Setup SPI module
  spi_fspi.begin(FSPI_SCLK, FSPI_MISO, FSPI_MOSI, FSPI_SS);

  spi_fspi.beginTransaction(SPISettings(80000000, MSBFIRST, SPI_MODE0));
  // Setting this up now so that the line is drivien low quickly after startup
  spi_fspi.transfer(0b00000000);
  spi_fspi.endTransaction();

}

void execute_test_glitch()
{
  int i=0;
  uint32_t current_glitch_time_ns = g_glitch_param.shortest_delay_ns;

  while((current_glitch_time_ns < g_glitch_param.longest_delay_ns) && (digitalRead(GLITCH_SUCCESS_PIN) == false))
  {
    for(int j=0; j< g_glitch_param.num_of_attempts_at_each_step; j++)
    {
      digitalWrite(ENTER_KEY_PIN,HIGH);
      
      execute_spi_driven_glitch(current_glitch_time_ns);
  
      digitalWrite(ENTER_KEY_PIN,LOW);
      
      delay(g_glitch_param.pause_time_between_glitching_ms);
      
      Serial.print(current_glitch_time_ns);
      Serial.println("us");
      
      //bool running, unsigned int delay_value_ns, unsigned int try_number, bool success)
      udpate_glitch_status_webpage(true,current_glitch_time_ns, j, false);
    }
    current_glitch_time_ns = current_glitch_time_ns + g_glitch_param.glitch_time_step_size_ns;
  }

  if(digitalRead(GLITCH_SUCCESS_PIN) == true)
  {
    Serial.println("SUCCESS! It worked!");
    udpate_glitch_status_webpage(false,current_glitch_time_ns, 0, true);
  }
  else
  {
    udpate_glitch_status_webpage(false,current_glitch_time_ns, 0, false);
    Serial.println("No Luck :( Try Again");
  }
}


//Shortest time about 18ns, longest time about 10000ns
void execute_spi_driven_glitch(unsigned long time_ns)
{
  unsigned long drive_frequency = 1/(0.000000001 * time_ns);

  spi_fspi.beginTransaction(SPISettings(drive_frequency, MSBFIRST, SPI_MODE0));   
  spi_fspi.transfer(0b00000001);
  spi_fspi.endTransaction();

}
