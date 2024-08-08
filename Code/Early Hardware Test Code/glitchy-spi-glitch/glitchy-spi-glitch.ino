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


#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "FS.h"
#include "SD.h"
#include "SPI.h"
#include "wifi_credentials.h"
#include <ArduinoJson.h>


//COMMENT OUT THE FOLLOWING TO CONNECT TO WIRELESS NETWORK INSTEAD OF HOSTING ONE
#define HOST_ACCESS_POINT
//NOTE: Put your wifi info in wifi_credentials.h if not hosting

//General system pins
#define POWER_GLITCH_PIN    15
#define AMP_IN_PIN          4
#define BIAS_IN_PIN         2

// Key pins for the power analysis example
#define KEY_1_PIN           5
#define KEY_2_PIN           6
#define KEY_3_PIN           7

// Pins for the glitching example
#define ENTER_KEY_PIN       11
#define GLITCH_SUCCESS_PIN  12

//Set up both SPI's

#define VSPI_MISO   13
#define VSPI_MOSI   POWER_GLITCH_PIN
#define VSPI_SCLK   14
#define VSPI_SS     16

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
unsigned int g_chart_update_rate_ms = 100;
unsigned int g_timer_send_chart_data_ms = 0;
bool g_glitching_acivate = false;
bool g_enable_amp_bias_streaming = true;

hw_timer_t *Timer0_Cfg = NULL;



// Initialize SD Card
void initSDCard(){
  uint64_t card_size = 0;
  uint8_t card_type = 0;

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

  //Set SSID and Password here if hosting access point
  const char* ap_ssid     = "ESP32-Access-Point";
  const char* ap_password = "123456789";

  

  WiFi.softAP(ap_ssid, ap_password);

  IPAddress IP = WiFi.softAPIP();
  
  Serial.print("AP IP address: ");
  Serial.println(IP);
  
  server.begin();
  #endif
  
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

    //Edit the below to change the glitching parameters that will run
    //shortest_delay_ns, longest_delay_ns, pause_time_between_glitching_ms, glitch_time_step_size_ns, num_of_attempts_at_each_step
    execute_test_glitch(200,1500,2000,10,3);
    
  }

  if(g_timer_send_chart_data_ms == 0 && g_enable_amp_bias_streaming == true)
  {
    g_timer_send_chart_data_ms = g_chart_update_rate_ms;
    read_and_send_ADC();
  }
}
