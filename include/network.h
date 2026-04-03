#ifndef KYRO_NETWORK_H
#define KYRO_NETWORK_H

#include "kyrogram.h"

// TDLib C interface (must be linked from tdjson)
void* td_json_client_create();
void td_json_client_send(void* client, const char* request);
const char* td_json_client_receive(void* client, double timeout);
void td_json_client_destroy(void* client);

// Kyro Network Core
void kyro_net_init(void);
void kyro_net_init_params(void);
void kyro_net_send(const char* method);
void kyro_net_poll(void);
void kyro_net_setup_proxy(const char* server, int port, const char* user, const char* pass);
void kyro_net_setup_smart_proxy(void);

// Messaging
void kyro_send_text(long long chat_id, const char* text);
void kyro_send_video_note(long long chat_id, const char* path);

#endif // KYRO_NETWORK_H
