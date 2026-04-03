#include "network.h"
#include <stdio.h>

#ifndef HAS_TDLIB
// Stubs for CI/Build without TDLib binary
void* td_json_client_create() { return NULL; }
void td_json_client_send(void* client, const char* request) { (void)client; (void)request; }
const char* td_json_client_receive(void* client, double timeout) { (void)client; (void)timeout; return NULL; }
void td_json_client_destroy(void* client) { (void)client; }
#endif
