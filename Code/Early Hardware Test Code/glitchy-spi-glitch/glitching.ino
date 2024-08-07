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
void execute_test_glitch(uint32_t shortest_delay_ns, uint32_t longest_delay_ns, uint32_t pause_time_between_glitching_ms, uint32_t glitch_time_step_size_ns, uint32_t num_of_attempts_at_each_step)
{
  int i=0;
  uint32_t current_glitch_time_ns = shortest_delay_ns;

  while((current_glitch_time_ns < longest_delay_ns) && (digitalRead(GLITCH_SUCCESS_PIN) == false))
  {
    for(int j=0; j< num_of_attempts_at_each_step; j++)
    {
      digitalWrite(ENTER_KEY_PIN,HIGH);
      
      execute_spi_driven_glitch(current_glitch_time_ns);
  
      digitalWrite(ENTER_KEY_PIN,LOW);
      
      delay(pause_time_between_glitching_ms);
      
      Serial.print(current_glitch_time_ns);
      Serial.println("us");
      
      //bool running, unsigned int delay_value_ns, unsigned int try_number, bool success)
      udpate_glitch_status_webpage(true,current_glitch_time_ns, j, false);
    }
    current_glitch_time_ns = current_glitch_time_ns + glitch_time_step_size_ns;
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
