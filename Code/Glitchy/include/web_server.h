#ifndef WEB_SERVER_H
#define WEB_SERVER_H

#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "FS.h"
#include "SD.h"


extern AsyncWebSocket ws;


void web_server_init();
void service_webserver(void);
void notFound(AsyncWebServerRequest *request);
void initWebSocket();
void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len);
void handleWebSocketMessage(void *arg, uint8_t *data, size_t len);
void udpate_glitch_status_webpage(bool running, unsigned int delay_value_ns, unsigned int try_number, bool success);
void send_glitch_params();
void set_glitching_activate(bool var_state);
bool get_glitching_activate(void);
void set_enable_amp_bias_streaming(bool var_state);
bool get_enable_amp_bias_streaming(void);
void set_send_glitch_params(bool var_state);
bool get_send_glitch_params(void);



#endif