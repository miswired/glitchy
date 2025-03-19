#include "web_server.h"
#include <ArduinoJson.h>
#include "global.h"
#include "glitching.h"
#include "analog_helpers.h"


// Create webserver on port 80 and websocket at /ws
AsyncWebServer server(80);
AsyncWebSocket ws("/ws");

//Globals for getters and setters
bool g_glitching_acivate;
bool g_enable_amp_bias_streaming;
bool g_send_glitch_params;

void set_glitching_activate(bool var_state)
{
    g_glitching_acivate = var_state;
}

bool get_glitching_activate(void)
{
    return g_glitching_acivate;
}

void set_enable_amp_bias_streaming(bool var_state)
{
    g_enable_amp_bias_streaming = var_state;
}

bool get_enable_amp_bias_streaming(void)
{
    return g_enable_amp_bias_streaming;
}

void set_send_glitch_params(bool var_state)
{
    g_send_glitch_params = var_state;
}

bool get_send_glitch_params(void)
{
    return g_send_glitch_params;
}


void web_server_init()
{

    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(SD, "/index.html", "text/html", false);
      });
  
      server.serveStatic("/", SD, "/");
  
  
  
      /*
      server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
          request->send(200, "text/plain", "Hello, world");
      });
  
      // Send a GET request to <IP>/get?message=<message>
      server.on("/get", HTTP_GET, [] (AsyncWebServerRequest *request) {
          String message;
          if (request->hasParam(PARAM_MESSAGE)) {
              message = request->getParam(PARAM_MESSAGE)->value();
          } else {
              message = "No message sent";
          }
          request->send(200, "text/plain", "Hello, GET: " + message);
      });
  
      // Send a POST request to <IP>/post with a form field message set to <message>
      server.on("/post", HTTP_POST, [](AsyncWebServerRequest *request){
          String message;
          if (request->hasParam(PARAM_MESSAGE, true)) {
              message = request->getParam(PARAM_MESSAGE, true)->value();
          } else {
              message = "No message sent";
          }
          request->send(200, "text/plain", "Hello, POST: " + message);
      });
      */
  
      server.onNotFound(notFound);
  
      server.begin();
}

void service_webserver(void)
{
    ws.cleanupClients();
}

void notFound(AsyncWebServerRequest *request) {
    request->send(404, "text/plain", "Not found");
}

// Start the websocket handler
void initWebSocket() {
    ws.onEvent(onEvent);
    server.addHandler(&ws);
}

// Look for clients connecting or disconnecting, or sending data
void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type,
    void *arg, uint8_t *data, size_t len) {
    switch (type) {
        case WS_EVT_CONNECT:
        Serial.printf("WebSocket client #%u connected from %s\n", client->id(), client->remoteIP().toString().c_str());
        break;

        case WS_EVT_DISCONNECT:
        Serial.printf("WebSocket client #%u disconnected\n", client->id());
        break;

        case WS_EVT_DATA:
        handleWebSocketMessage(arg, data, len);
        break;

        case WS_EVT_PONG:
        case WS_EVT_ERROR:
        break;
    }
}

//If the clients initiate a web socket message, parse it here, look for the toggle button, and 
//toggle the trigger event. Then send a response to all the clients that it's been initiated.
void handleWebSocketMessage(void *arg, uint8_t *data, size_t len) {
    AwsFrameInfo *info = (AwsFrameInfo*)arg;
    if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT) 
    {
        data[len] = 0;
        Serial.print("Got data packet: ");
        Serial.println((char*)data);
    
        JsonDocument doc;
        
        //DeserializationError error = deserializeJson(doc, data, len);
        DeserializationError error = deserializeJson(doc, data);

        if (error){
            Serial.print("deserializeJson() failed: ");
            Serial.println(error.c_str());
            return;
        }
        
        const char* comms_version = doc["CommsVersion"];
        Serial.println(comms_version);

        const char* packet_type = doc["PacketType"];
        Serial.println(packet_type);


        if(doc["CommsVersion"] == "1.1"){
            if (doc["PacketType"] == "start_glitching") {
            set_glitching_activate(true);
            Serial.println("Got start_glitching");
            //g_glitching_acivate = true;
        
            }
            if (doc["PacketType"] == "start_power_analysis"){
            Serial.println("Got start_power_analysis");
            run_key_diff_analysis();
            }
            if (doc["PacketType"] == "enable_adc_stream"){
            Serial.println("Got enable_adc_stream");
            set_enable_amp_bias_streaming(true);
            //g_enable_amp_bias_streaming = true;
        
            }
            if (doc["PacketType"] == "disable_adc_stream"){
            Serial.println("Got disable_adc_streams");
            set_enable_amp_bias_streaming(false);
            //g_enable_amp_bias_streaming = false;
            }
    
            if (doc["PacketType"] == "get_glitch_param"){
            Serial.println("Got get_glitch_param");
            set_send_glitch_params(true);
            //g_send_glitch_params = true;
            }
    
            if (doc["PacketType"] == "set_glitch_param"){
            Serial.println("Got set_glitch_param");           
            g_glitch_param.shortest_delay_ns = doc["start_time_ns"];
            g_glitch_param.longest_delay_ns = doc["stop_time_ns"];
            g_glitch_param.pause_time_between_glitching_ms = doc["delay_between_glitches_ms"];
            g_glitch_param.glitch_time_step_size_ns = doc["step_size_ns"];
            g_glitch_param.num_of_attempts_at_each_step = doc["retry_times"];
            }  
        }
    }
}


void udpate_glitch_status_webpage(bool running, unsigned int delay_value_ns, unsigned int try_number, bool success)
{
  JsonDocument doc;
  doc["CommsVersion"] = 1.1;
  doc["PacketType"] = "UpdateGlitchStatus";
  doc["running"] = running;
  doc["delay_value"] = delay_value_ns;
  doc["try_number"] = try_number;
  doc["success"] = success;
  
  doc.shrinkToFit();  // optional
  
  String jsonString;
  serializeJson(doc, jsonString);
  
  ws.textAll(jsonString);

}

void send_glitch_params()
{  
  JsonDocument doc;
  doc["CommsVersion"] = 1.1;
  doc["PacketType"] = "glitch_param";
  doc["start_time_ns"] = g_glitch_param.shortest_delay_ns;
  doc["stop_time_ns"] = g_glitch_param.longest_delay_ns;
  doc["delay_between_glitches_ms"] = g_glitch_param.pause_time_between_glitching_ms;
  doc["step_size_ns"] = g_glitch_param.glitch_time_step_size_ns;
  doc["retry_times"] = g_glitch_param.num_of_attempts_at_each_step;
  
  doc.shrinkToFit();  // optional
  
  String jsonString;
  serializeJson(doc, jsonString);
  
  ws.textAll(jsonString);

}
