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
void execute_test_glitch(uint32_t shortest_delay_nops, uint32_t longest_delay_nops, uint32_t pause_time_between_glitching_ms, uint32_t glitch_time_step_size, uint32_t num_of_attempts_at_each_step)
{
  int i=0;
  uint32_t glitch_time_nops = shortest_delay_nops;

  while((glitch_time_nops < longest_delay_nops) && (digitalRead(GLITCH_SUCCESS_PIN) == false))
  {
    for(int j=0; j< num_of_attempts_at_each_step; j++)
    {
      digitalWrite(ENTER_KEY_PIN,HIGH);
      REG_SET_BIT(GPIO_OUT_REG,BIT15);
  
      for(i=0; i<glitch_time_nops; i++)
      {
        asm ( "nop \n" );
      }
  
     
      REG_CLR_BIT(GPIO_OUT_REG,BIT15);
  
      digitalWrite(ENTER_KEY_PIN,LOW);
      
      delay(pause_time_between_glitching_ms);
      
      Serial.println(glitch_time_nops);
    }
    glitch_time_nops = glitch_time_nops + glitch_time_step_size;
  }

  if(digitalRead(GLITCH_SUCCESS_PIN) == true)
  {
    Serial.println("SUCCESS! It worked!");
  }
}

void execute_fast_test_glitch()
{
  int i=0;
  digitalWrite(ENTER_KEY_PIN,HIGH);

  REG_SET_BIT(GPIO_OUT_REG,BIT15);

  /*
  for(i=0; i<100; i++)
    {
      asm ( "nop \n" );
    } 
  */
  asm ( "nop \n" );
    
  REG_CLR_BIT(GPIO_OUT_REG,BIT15);
  
  digitalWrite(ENTER_KEY_PIN,LOW);
  delay(1000);
  Serial.println("Glitched");
}
