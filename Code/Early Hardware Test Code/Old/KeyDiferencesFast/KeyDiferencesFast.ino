#include "analog.h"


unsigned long time1 = 0;
unsigned long time2 = 0;

#define adc_buffer_len 50
uint16_t adc_buffer1[adc_buffer_len];
uint16_t adc_buffer2[adc_buffer_len];
uint16_t adc_buffer3[adc_buffer_len];

#define KEY_1_PIN 5
#define KEY_2_PIN 6
#define KEY_3_PIN 7

//Note, the fast ADC library uses pins and channles, define both
#define ADC_PIN   4
#define ADC_CHAN  3

void setup() {
  // initialize serial communication at 115200 bits per second:
  Serial.begin(115200);

  //Setup key's and set them high, a low is a key press
  pinMode(KEY_1_PIN, OUTPUT);
  pinMode(KEY_2_PIN, OUTPUT);
  pinMode(KEY_3_PIN, OUTPUT);
  digitalWrite(KEY_1_PIN, HIGH);
  digitalWrite(KEY_2_PIN, HIGH);
  digitalWrite(KEY_3_PIN, HIGH);
  
  //set the resolution to 12 bits (0-4096)
  //analogReadResolution(12);

  //Setup the fast ADC library. This one uses pins
  //All the others use channels. 
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
  fadcInit(1, ADC_PIN);
  Serial.printf("Waiting for target to settle\n");
  delay(10000);
  Serial.printf("Running\n");



  //Serial.printf("Key 1\n");
  pinMode(KEY_1_PIN, OUTPUT);
  pinMode(KEY_2_PIN, INPUT);
  pinMode(KEY_3_PIN, INPUT);
  delay(100);
  digitalWrite(KEY_1_PIN, LOW);
  //time1=micros();
  for(int i=0; i<adc_buffer_len; i++)
  {
    //adc_buffer1[i]=analogRead(ADC_PIN);
    adc_buffer1[i]=analogReadFast(ADC_CHAN);
  }
  //time2=micros();
  digitalWrite(KEY_1_PIN, HIGH);
  delay(100);
  pinMode(KEY_1_PIN, INPUT);
  pinMode(KEY_2_PIN, INPUT);
  pinMode(KEY_3_PIN, INPUT);
  //Serial.printf("Micros Elapsed for ADC: %ul\n",time2-time1);

  /*
  for(int i=0; i<adc_buffer_len; i++)
  {
    Serial.printf("Key1:%d\n",adc_buffer[i]);
  }
  */
  
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
    //adc_buffer2[i]=analogRead(ADC_PIN);
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
  
  time1=micros();
  for(int i=0; i<adc_buffer_len; i++)
  {
    //adc_buffer3[i]=analogRead(ADC_PIN);
    adc_buffer3[i]=analogReadFast(ADC_CHAN);
  }
  time2=micros();
  digitalWrite(KEY_3_PIN, HIGH);
  delay(100);
  pinMode(KEY_1_PIN, INPUT);
  pinMode(KEY_2_PIN, INPUT);
  pinMode(KEY_3_PIN, INPUT);

  
  for(int i=0; i<adc_buffer_len; i++)
  {
    Serial.printf("Key1:%d,Key2:%d,Key3:%d\n",adc_buffer1[i],adc_buffer2[i],adc_buffer3[i]);
  }
  
}

void loop() {
  // read the analog / millivolts value for pin 2:

  
  
  delay(100);  // delay in between reads for clear read from serial
}
