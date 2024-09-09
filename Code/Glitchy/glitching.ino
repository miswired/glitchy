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

// This is a quick and dirty glitching sequence used for testing. It's using nop's for delays as the delay function has too much overhead for short delays.
// The pulse width of the glitch is staticly set right now, this should be dynamic

//Tell the compiler to not optimize so hopefully the timing doesn't get delayed. 
//#pragma GCC push_options
//#pragma GCC optimize("Ofast")

typedef struct {
  uint32_t shortest_delay_ns;
  uint32_t longest_delay_ns;
  uint32_t pause_time_between_glitching_ms;
  uint32_t glitch_time_step_size_ns;
  uint32_t num_of_attempts_at_each_step;
} glitch_param_t;

glitch_param_t g_glitch_param;

#define SHORTEST_DELAY_NS_DEFAULT               200
#define LONGEST_DELAY_NS_DEFAULT                1500
#define PAUSE_TIME_BETWEEN_GLITCHING_MS_DEFAULT 2000
#define GLITCH_TIME_STEP_SIZE_NS_DEFAULT        10
#define NUM_OF_ATTEMPTS_AT_EACH_STEP_DEFAULT    3

void init_glitch()
{
  g_glitch_param.shortest_delay_ns = SHORTEST_DELAY_NS_DEFAULT;
  g_glitch_param.longest_delay_ns = LONGEST_DELAY_NS_DEFAULT;
  g_glitch_param.pause_time_between_glitching_ms = PAUSE_TIME_BETWEEN_GLITCHING_MS_DEFAULT;
  g_glitch_param.glitch_time_step_size_ns = GLITCH_TIME_STEP_SIZE_NS_DEFAULT;
  g_glitch_param.num_of_attempts_at_each_step = NUM_OF_ATTEMPTS_AT_EACH_STEP_DEFAULT;
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

  vspi->beginTransaction(SPISettings(drive_frequency, MSBFIRST, SPI_MODE0));   
  vspi->transfer(0b00000001);
  vspi->endTransaction();

}
