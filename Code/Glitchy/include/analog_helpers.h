#ifndef ANALOG_HELPERS_H
#define ANALOG_HELPERS_H

float convert_dac_to_volts(unsigned int dac_value);
void setup_fast_adc();
void read_and_send_ADC();
void run_key_diff_analysis();



#endif