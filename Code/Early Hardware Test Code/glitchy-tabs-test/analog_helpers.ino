#define DAC_CAL_Y0      1.42
#define DAC_CAL_X0      1665
#define DAC_CAL_SLOPE   0.000721 


float convert_dac_to_volts(unsigned int dac_value)
{
  return (float) DAC_CAL_Y0 + (((float)dac_value) - (float)DAC_CAL_X0) * (float)DAC_CAL_SLOPE;
}
