#ifndef GLITCHING_H
#define GLITCHING_H

void init_glitch();
void execute_test_glitch();
void execute_spi_driven_glitch(unsigned long time_ns);

#include "driver/gpio.h"

typedef struct {
    uint32_t shortest_delay_ns;
    uint32_t longest_delay_ns;
    uint32_t pause_time_between_glitching_ms;
    uint32_t glitch_time_step_size_ns;
    uint32_t num_of_attempts_at_each_step;
  } glitch_param_t;
  
extern glitch_param_t g_glitch_param;

#endif