#include "analog.h"

#define DAC_CAL_Y0      1.42
#define DAC_CAL_X0      1665
#define DAC_CAL_SLOPE   0.000721 

//Note, the fast ADC library uses pins and channles, define both because the library is silly 
  /*
   * Pin / Channel
   * 1/0
   * 2/1
   * 3/2
   * 4/3
   * 5/4
   * 6/5
   * 7/6
   * 8/7
   * 9/8
   * 10/9
   * 
   */
#define ADC_PIN   AMP_IN_PIN
#define ADC_CHAN  (AMP_IN_PIN - 1)

#define adc_buffer_len 1000
uint16_t adc_buffer1[adc_buffer_len];
uint16_t adc_buffer2[adc_buffer_len];
uint16_t adc_buffer3[adc_buffer_len];


float convert_dac_to_volts(unsigned int dac_value)
{
  return (float) DAC_CAL_Y0 + (((float)dac_value) - (float)DAC_CAL_X0) * (float)DAC_CAL_SLOPE;
}

void setup_fast_adc()
{
  fadcInit(1, ADC_PIN);
}

void read_and_send_ADC()
{
  JsonDocument doc;
  doc["CommsVersion"] = 1.1;
  doc["PacketType"] = "ADCStream";
  doc["LastAmpInVolts"] = convert_dac_to_volts(analogRead(AMP_IN_PIN));
  doc["LastBiasVolts"] = convert_dac_to_volts(analogRead(BIAS_IN_PIN));
  //JsonArray chartarray_jsonarray = doc["chartarray"].to<JsonArray>();

  //adc_amp_in_buffer.push(analogRead(AMP_IN_PIN));
  
  doc.shrinkToFit();  // optional
  
  String jsonString;
  serializeJson(doc, jsonString);
  
  ws.textAll(jsonString);

}

void run_key_diff_analysis()
{
  Serial.printf("Running key diff analysis\n");
  
  //Serial.printf("Key 1\n");
  pinMode(KEY_1_PIN, OUTPUT);
  pinMode(KEY_2_PIN, INPUT);
  pinMode(KEY_3_PIN, INPUT);
  delay(100);
  digitalWrite(KEY_1_PIN, LOW);
  //time1=micros();
  for(int i=0; i<adc_buffer_len; i++)
  {
    adc_buffer1[i]=analogReadFast(ADC_CHAN);
  }
  //time2=micros();
  digitalWrite(KEY_1_PIN, HIGH);
  delay(100);
  pinMode(KEY_1_PIN, INPUT);
  pinMode(KEY_2_PIN, INPUT);
  pinMode(KEY_3_PIN, INPUT);
  //Serial.printf("Micros Elapsed for ADC: %ul\n",time2-time1);
  
  delay(1000);

  //Serial.printf("Key 2\n");
  pinMode(KEY_1_PIN, INPUT);
  pinMode(KEY_2_PIN, OUTPUT);
  pinMode(KEY_3_PIN, INPUT);
  delay(100);
  digitalWrite(KEY_2_PIN, LOW);
  //time1=micros();
  for(int i=0; i<adc_buffer_len; i++)
  {
    adc_buffer2[i]=analogReadFast(ADC_CHAN);
  }
  //time2=micros();
  digitalWrite(KEY_2_PIN, HIGH);
  delay(100);
  pinMode(KEY_1_PIN, INPUT);
  pinMode(KEY_2_PIN, INPUT);
  pinMode(KEY_3_PIN, INPUT);
  //Serial.printf("Micros Elapsed for ADC: %ul\n",time2-time1);


  delay(1000);

  pinMode(KEY_1_PIN, INPUT);
  pinMode(KEY_2_PIN, INPUT);
  pinMode(KEY_3_PIN, OUTPUT);
  //Serial.printf("Key 3\n");
  delay(100);
  digitalWrite(KEY_3_PIN, LOW);
  
  //time1=micros();
  for(int i=0; i<adc_buffer_len; i++)
  {
    adc_buffer3[i]=analogReadFast(ADC_CHAN);
  }
  //time2=micros();
  digitalWrite(KEY_3_PIN, HIGH);
  delay(100);
  pinMode(KEY_1_PIN, INPUT);
  pinMode(KEY_2_PIN, INPUT);
  pinMode(KEY_3_PIN, INPUT);

  JsonDocument doc;
  doc["CommsVersion"] = 1.1;
  doc["PacketType"] = "KeyTraces";
  JsonArray key1_jsonarray = doc["key_1_array"].to<JsonArray>();
  JsonArray key2_jsonarray = doc["key_2_array"].to<JsonArray>();
  JsonArray key3_jsonarray = doc["key_3_array"].to<JsonArray>();


  for(unsigned int i=0; i<adc_buffer_len; i++)
  {
    key1_jsonarray.add(adc_buffer1[i]);
    key2_jsonarray.add(adc_buffer2[i]);
    key3_jsonarray.add(adc_buffer3[i]);
  }

  doc.shrinkToFit();  // optional
  
  String jsonString;
  serializeJson(doc, jsonString);
  
  ws.textAll(jsonString);

}
