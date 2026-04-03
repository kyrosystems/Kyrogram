#ifndef KYRO_NETWORK_H
#define KYRO_NETWORK_H

#include "kyrogram.h"

// TDLib C interface functions (will be linked from tdjson)
extern void* td_json_client_create();
extern void td_json_client_send(void* client, const char* request);
extern const char* td_json_client_receive(void* client, double timeout);
extern void td_json_client_destroy(void* client);

/**
 * Initializes the TDLib client and sets up the event loop.
 */
void kyro_net_init(void);

/**
 * Sends a request to Telegram via TDLib.
 * @param method JSON-formatted request string
 */
void kyro_net_send(const char* method);

/**
 * Main polling loop for incoming Telegram updates.
 */
void kyro_net_poll(void);

/**
 * Sets up proxy and anti-blocking (RKN bypass).
 */
void kyro_net_setup_proxy(const char* server, int port, const char* user, const char* pass);

#endif // KYRO_NETWORK_H
