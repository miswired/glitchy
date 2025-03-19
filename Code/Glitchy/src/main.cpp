/*
Glitchy
An introduction to hardware exploitation.

This file is licensed under the Creative Commons Attribution-Share Alike 3.0 Unported license.
https://creativecommons.org/licenses/by-sa/3.0/deed.en

Attribution: Miswired
Special thanks goes to Rui Santos and the RandomNerdTutorials site. The work here was inspired by those examples and used with permission.
Please visit https://randomnerdtutorials.com/

    You are free:
        to share – to copy, distribute and transmit the work
        to remix – to adapt the work
    Under the following conditions:
        attribution – You must give appropriate credit, provide a link to the license, and indicate if changes were made. You may do so in any reasonable manner, but not in any way that suggests the licensor endorses you or your use.
        share alike – If you remix, transform, or build upon the material, you must distribute your contributions under the same or compatible license as the original. https://creativecommons.org/share-your-work/licensing-considerations/compatible-licenses
*/

#include <Arduino.h>
#include <WiFi.h>
#include "FS.h"
#include "SD.h"
#include "SPI.h"
#include "pins.h"
#include "web_server.h"
#include "global.h"
#include "glitching.h"
#include "analog_helpers.h"


/* This section is for global compiled options */
#define HOST_ACCESS_POINT   //Uncomment to have Glitchy host it's own wifi network, comment to tell it to connect to an existing network

const char* ssid = "YOUR_SSID_HERE";
const char* password = "YOUR_PASSWORD_HERE";

//Set SSID and Password here if hosting access point
const char* ap_ssid     = "Glitchy";
const char* ap_password = "123456789";

unsigned int g_timer_send_chart_data_ms = 0;
unsigned int g_chart_update_rate_ms = 100;

SPIClass spi_hspi = SPIClass(HSPI);

hw_timer_t *Timer0_Cfg = NULL;

// Initialize SD Card
void initSDCard(){
    uint64_t card_size = 0;
    uint8_t card_type = 0;

    spi_hspi.begin(HSPI_SCLK, HSPI_MISO, HSPI_MOSI, HSPI_SS);
  
    if(!SD.begin(HSPI_SS, spi_hspi, 80000000,"/sd",20)){
      Serial.println("SD Card Init Error");
      return;
    }
    
    //Get the card type
    card_type = SD.cardType();
  
    if(card_type == CARD_NONE){
      Serial.println("No SD Card Detected");
      return;
    }
  
    switch(card_type)
    {
      case CARD_MMC:
        Serial.println("Card Detected is an MMC");
      break;
  
      case CARD_SD:
        Serial.println("Card Detected is an SD");
      break;
  
      case CARD_SDHC:
        Serial.println("Card Detected is an SDHC");
      break;
  
      default:
        Serial.println("Unknown card type");
    }
  
    //Get the size of the SD card
    card_size = SD.cardSize() / (1024 * 1024);
    Serial.printf("SD Card Size: %lluMB\n", card_size);
  }

// Start the wifi module and connect to the configured AP
// Make sure to set up credentials in the wifi_credentials.h file
// If you are connecting to an access point instead of hosting one
void initWiFi() {
    Serial.println("Seting up Wifi");
  
    //Might be good to make unique SSID names automatically, future update.
    /*
    uint8_t mac[8];
    esp_efuse_mac_get_default(mac);
    Serial.print("Mac: ");
    Serial.println("Seting up Wifi");
    */
  
    #ifndef HOST_ACCESS_POINT
  
    Serial.println("SSID Begin");
    WiFi.begin(ssid, password);
    Serial.print("Connecting to WiFi ..");
    while (WiFi.status() != WL_CONNECTED) {
      Serial.print('.');
      delay(1000);
    }
    Serial.println(WiFi.localIP());
  
    WiFi.softAP(ssid, password);
    WiFi.mode(WIFI_STA);
  
    #else
  
    WiFi.softAP(ap_ssid, ap_password);
  
    IPAddress IP = WiFi.softAPIP();
    
    Serial.print("AP IP address: ");
    Serial.println(IP);
    #endif
    
  }

void process_timers()
  {
    if(g_timer_send_chart_data_ms > 0){g_timer_send_chart_data_ms--;}
    
  }

// Inturrupt vector for timer 0
void IRAM_ATTR Timer0_ISR()
{
    process_timers();
}



void setup() {

    Serial.begin(115200);

    pinMode(GLITCH_SUCCESS_PIN, INPUT);
    pinMode(ENTER_KEY_PIN, OUTPUT);
    digitalWrite(ENTER_KEY_PIN, LOW);
    
    initWiFi();
    initSDCard();
    web_server_init();
    initWebSocket();
    init_glitch();
    read_and_send_ADC();
    setup_fast_adc();

    //Inturrupt setup for 1 ms timers

    Timer0_Cfg = timerBegin(0, 240, true);
    
    timerWrite(Timer0_Cfg, 0);
    timerAlarmWrite(Timer0_Cfg, 500, true);
    timerAttachInterrupt(Timer0_Cfg, &Timer0_ISR, true);
    timerAlarmEnable(Timer0_Cfg);


}

void loop() {
  service_webserver();

  if(get_glitching_activate() == true){
    set_glitching_activate(false);

    execute_test_glitch();
    
  }

  if(g_timer_send_chart_data_ms == 0 && get_enable_amp_bias_streaming() == true)
  {
    g_timer_send_chart_data_ms = g_chart_update_rate_ms;
    read_and_send_ADC();
  }

  if(get_send_glitch_params() == true)
  {
    set_send_glitch_params(false);
    send_glitch_params();
  }
}