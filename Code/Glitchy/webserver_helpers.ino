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


//If the clients initiate a web socket message, parse it here, look for the toggle button, and 
//toggle the trigger event. Then send a response to all the clients that it's been initiated.
void handleWebSocketMessage(void *arg, uint8_t *data, size_t len) {
  AwsFrameInfo *info = (AwsFrameInfo*)arg;
  if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT) {
    data[len] = 0;
    Serial.println((char*)data);

    JsonDocument doc;
    
    DeserializationError error = deserializeJson(doc, data, len);
    
    if (error) {
      Serial.print("deserializeJson() failed: ");
      Serial.println(error.c_str());
      return;
    }

    if(doc["CommsVersion"] != NULL)
    {
      if(doc["CommsVersion"] == 1.1)
      {
        
        if (doc["PacketType"] == "start_glitching") {
          g_glitching_acivate = true;
    
        }
        if (doc["PacketType"] == "start_power_analysis"){
          Serial.println("Got start_power_analysis");
          run_key_diff_analysis();
        }
        if (doc["PacketType"] == "enable_adc_stream"){
          Serial.println("Got enable_adc_stream");
          g_enable_amp_bias_streaming = true;
    
        }
        if (doc["PacketType"] == "disable_adc_stream"){
          Serial.println("Got disable_adc_streams");
          g_enable_amp_bias_streaming = false;
        }

        if (doc["PacketType"] == "get_glitch_param"){
          Serial.println("Got get_glitch_param");
          g_send_glitch_params = true;
        }

        if (doc["PacketType"] == "set_glitch_param"){
          Serial.println("Got set_glitch_param");
          g_glitch_param.shortest_delay_ns = doc["start_time_ns"];
          g_glitch_param.longest_delay_ns = doc["stop_time_ns"];
          g_glitch_param.pause_time_between_glitching_ms = doc["delay_between_glitches_ms"];
          g_glitch_param.glitch_time_step_size_ns = doc["step_size_ns"];
          g_glitch_param.num_of_attempts_at_each_step = doc["retry_times"];
        }

        if(doc["PacketType"] == "testpacket")
        {
          Serial.println("Got test packet");
          Serial.println((int)doc["somevalue"]);
          Serial.println((bool)doc["somebool"]);
        }
      }
    }

    

    /*
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
    */
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

//This is a pre-processor that will look for vairables to replace
//On the first load of the page. Moved away from this to using JS
//On the page end and websockets instead.
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
  */
  return String();
  
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
