/*
This file contains helper functions for the webserver feature. 

Normally in a C program this would be a self contained module.
But as I am trying to keep this more beginnner friendly and use the Arduino IDE, there are some limitations on how it links files
and auto creates prototypes.

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


//Calling this will send a message to all the currently subscribed web clients via websockets
//Right now it's just a simple string about the state of the glitching
/*
void notifyClients() {
  ws.textAll(String(g_trigger_state));
}
*/

//If the clients initiate a web socket message, parse it here, look for the toggle button, and 
//toggle the trigger event. Then send a response to all the clients that it's been initiated.
void handleWebSocketMessage(void *arg, uint8_t *data, size_t len) {
  AwsFrameInfo *info = (AwsFrameInfo*)arg;
  if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT) {
    data[len] = 0;
    if (strcmp((char*)data, "start_glitching") == 0) {
      g_glitching_acivate = true;
      
    }
    if (strcmp((char*)data, "start_power_analysis") == 0) {
      Serial.println("Got start_power_analysis");
      run_key_diff_analysis();
    }
    if (strcmp((char*)data, "enable_adc_stream") == 0) {
      Serial.println("Got enable_adc_stream");
      g_enable_amp_bias_streaming = true;

    }
    if (strcmp((char*)data, "disable_adc_stream") == 0) {
      Serial.println("Got disable_adc_streams");
      g_enable_amp_bias_streaming = false;
    }
  }
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

// Start the websocket handler
void initWebSocket() {
  ws.onEvent(onEvent);
  server.addHandler(&ws);
}

// Sort through strings looking for "STATE" and replace it with ON or OFF depending on 
// current trigger state. This is needed for first time page loads so the initial static page has
// the right state reflected.
String processor(const String& var){
  /*
  Serial.println(var);
  if(var == "STATE"){
    if (g_trigger_state){
      return "ON";
    }
    else{
      return "OFF";
    }
  }
  return String();
  */
}

void udpate_glitch_status_webpage(bool running, unsigned int delay_value, unsigned int try_number, bool success)
{
  JsonDocument doc;
  doc["CommsVersion"] = 1.1;
  doc["PacketType"] = "UpdateGlitchStatus";
  doc["running"] = running;
  doc["delay_value"] = delay_value;
  doc["try_number"] = try_number;
  doc["success"] = success;
  
  doc.shrinkToFit();  // optional
  
  String jsonString;
  serializeJson(doc, jsonString);
  
  ws.textAll(jsonString);

}
