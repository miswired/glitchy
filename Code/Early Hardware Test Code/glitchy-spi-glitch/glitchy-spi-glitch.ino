

/*
This project is a proof of concept for generating glitch pulses when triggered from a web page. This is unpolished messy code.

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

//I was checking if this would remove code optimizations and if it changed performance  
//#pragma GCC optimize ("-O0")
//#pragma GCC push_options

#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "FS.h"
#include "SD.h"
#include "SPI.h"
#include "wifi_credentials.h"
//#include <Arduino_JSON.h>
#include <ArduinoJson.h>
//#include "CircularBuffer.hpp"



//NOTE: Put your wifi info in wifi_credentials.h

//Pin Definitions
#define LED_PIN             5
#define POWER_GLITCH_PIN    15
#define ENTER_KEY_PIN       11
#define GLITCH_SUCCESS_PIN  12

//#define SCK_PIN             39
//#define MISO_PIN            47
//#define MOSI_PIN            40
//#define CS_PIN              41

#define AMP_IN_PIN          4
#define BIAS_IN_PIN         2

// Key pins for the power analysis example
#define KEY_1_PIN           9
#define KEY_2_PIN           10
#define KEY_3_PIN           11


//Set up both SPI's

#define VSPI_MISO   6
#define VSPI_MOSI   5
#define VSPI_SCLK   7
#define VSPI_SS     8

#define HSPI_MISO   47
#define HSPI_MOSI   40
#define HSPI_SCLK   39
#define HSPI_SS     41

  
#if CONFIG_IDF_TARGET_ESP32S2 || CONFIG_IDF_TARGET_ESP32S3
#define VSPI FSPI
#endif

static const int spiClk = 1000000; // 1 MHz

//uninitalised pointers to SPI objects
SPIClass * vspi = NULL;
SPIClass * hspi = NULL;




// Create webserver on port 80 and websocket at /ws
AsyncWebServer server(80);
AsyncWebSocket ws("/ws");

// Globals


String g_test_ssid;
char ssid_char[30];

#define CAL_ARRAY_LENGTH 100
unsigned int cal_array[CAL_ARRAY_LENGTH];

unsigned int g_chart_update_rate_ms = 100;
unsigned int g_timer_send_chart_data_ms = 0;
unsigned long g_last_milis_reading = 0;

#define BUFFER_SIZE   100
//CircularBuffer<unsigned int, BUFFER_SIZE> adc_amp_in_buffer;
//CircularBuffer<unsigned int, BUFFER_SIZE> adc_bias_buffer;

bool g_inturupt_called = false;
bool g_glitching_acivate = false;

hw_timer_t *Timer0_Cfg = NULL;

bool g_enable_amp_bias_streaming = true;

uint8_t i=0;

// Initialize SD Card
void initSDCard(){
  uint64_t card_size = 0;
  uint8_t card_type = 0;

  //SPI.begin(SCK_PIN, MISO_PIN, MOSI_PIN, CS_PIN);
  if(!SD.begin(hspi->pinSS(), *hspi)){
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
void initWiFi() {
  Serial.println("Seting up Wifi");
  /*
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
  */

  //ESP32 as AP
  const char* ap_ssid     = "ESP32-Access-Point";
  const char* ap_password = "123456789";

  WiFi.softAP(ap_ssid, ap_password);

  IPAddress IP = WiFi.softAPIP();
  
  Serial.print("AP IP address: ");
  Serial.println(IP);
  
  server.begin();
  
}

// Inturrupt vector for timer 0
void IRAM_ATTR Timer0_ISR()
{
    process_timers();
}


//Start system initiation
void setup(){
  // Serial port for debugging purposes
  Serial.begin(115200);

  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  pinMode(POWER_GLITCH_PIN, OUTPUT);
  digitalWrite(POWER_GLITCH_PIN, LOW);

  pinMode(GLITCH_SUCCESS_PIN, INPUT);

  pinMode(ENTER_KEY_PIN, OUTPUT);
  digitalWrite(ENTER_KEY_PIN, LOW);

  //Setup SPI ports
  //initialise two instances of the SPIClass attached to VSPI and HSPI respectively
  vspi = new SPIClass(VSPI);
  hspi = new SPIClass(HSPI);

  vspi->begin(VSPI_SCLK, VSPI_MISO, VSPI_MOSI, VSPI_SS); //SCLK, MISO, MOSI, SS
  hspi->begin(HSPI_SCLK, HSPI_MISO, HSPI_MOSI, HSPI_SS); //SCLK, MISO, MOSI, SS

  pinMode(vspi->pinSS(), OUTPUT); //VSPI SS
  pinMode(hspi->pinSS(), OUTPUT); //HSPI SS
  
  vspi->beginTransaction(SPISettings(80000000, MSBFIRST, SPI_MODE0));
  // Setting this up now so that the line is drivien low quickly after startup
  vspi->transfer(0b00000000);
  vspi->endTransaction();

  // Connect to Wi-Fi
  initSDCard();

  //Load config file
  //config_test();

  initWiFi();
  

  initWebSocket();

  // This is serving from the SD card. Note the false tells it to render the page
  // a true tells it to download page as a file. The processor processes the template page
  // before displaying. I got confused initially because the send vs send_P had an extra argument
  // and I missed the true/false part.
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SD, "/index.html", "text/html", false, processor);
  });
  server.serveStatic("/", SD, "/");
  // Start server
  server.begin();


  //Inturrupt setup for 1 ms timers

  Timer0_Cfg = timerBegin(0, 240, true);
  
  timerWrite(Timer0_Cfg, 0);
  timerAlarmWrite(Timer0_Cfg, 500, true);
  timerAttachInterrupt(Timer0_Cfg, &Timer0_ISR, true);
  timerAlarmEnable(Timer0_Cfg);

  
  setup_fast_adc();
}

void process_timers()
{
  if(g_timer_send_chart_data_ms > 0){g_timer_send_chart_data_ms--;}
  
}

// Main loop
void loop() {
  ws.cleanupClients();

  if(g_glitching_acivate == true){
    g_glitching_acivate = false;
    //execute_test_glitch(1,90,2000,1,3);
    

      //execute_fast_test_glitch(i);

    //execute_fast_test_glitch(2);
    

    vspi->beginTransaction(SPISettings(80000000, MSBFIRST, SPI_MODE0));
    vspi->transfer(0b10101010);
    vspi->endTransaction();
    
    vspi->beginTransaction(SPISettings(70000000, MSBFIRST, SPI_MODE0));
    vspi->transfer(0b10101010);
    vspi->endTransaction();
    
    vspi->beginTransaction(SPISettings(60000000, MSBFIRST, SPI_MODE0));
    vspi->transfer(0b10101010);
    vspi->endTransaction();
    
    vspi->beginTransaction(SPISettings(50000000, MSBFIRST, SPI_MODE0));
    vspi->transfer(0b10101010);
    vspi->endTransaction();
    
    vspi->beginTransaction(SPISettings(10000000, MSBFIRST, SPI_MODE0));
    vspi->transfer(0b10101010);
    vspi->endTransaction();
    
    vspi->beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE0));
    vspi->transfer(0b10101010);
    vspi->endTransaction();
    
  
    
  }

  if(g_timer_send_chart_data_ms == 0 && g_enable_amp_bias_streaming == true)
  {
    g_timer_send_chart_data_ms = g_chart_update_rate_ms;
    //Serial.println("Sending ADC");
    read_and_send_ADC();
  }
}
