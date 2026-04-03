#include "network.h"
#include <stdbool.h>
#include <string.h>
#include <stdio.h>

typedef struct {
    char server[256];
    int port;
    char user[128];
    char pass[128];
    bool is_working;
    int latency_ms;
} KyroProxy;

static KyroProxy global_proxy_list[10];
static int current_proxy_index = 0;

void kyro_proxy_init(void) {
    strcpy(global_proxy_list[0].server, "socks5.kyrogram.net");
    global_proxy_list[0].port = 1080;
    
    strcpy(global_proxy_list[1].server, "proxy.dc1.telegram.org");
    global_proxy_list[1].port = 443;
}

void kyro_proxy_check_connectivity(void) {
    if (!global_proxy_list[current_proxy_index].is_working) {
        current_proxy_index = (current_proxy_index + 1) % 10;
        kyro_net_setup_proxy(
            global_proxy_list[current_proxy_index].server,
            global_proxy_list[current_proxy_index].port,
            global_proxy_list[current_proxy_index].user,
            global_proxy_list[current_proxy_index].pass
        );
    }
}
