#include "network.h"
#include <string.h>
#include <stdio.h>

typedef struct {
    char host[128];
    int port;
    char user[64];
    char pass[64];
} KyroProxyNode;

static KyroProxyNode builtin_proxies[] = {
};

void kyro_net_setup_smart_proxy(void) {
    // Выбираем первый рабочий прокси
    KyroProxyNode p = builtin_proxies[0];
    
    char proxy_json[512];
    sprintf(proxy_json, 
        "{\"@type\": \"addProxy\", \"server\": \"%s\", \"port\": %d, \"enable\": true, "
        "\"type\": {\"@type\": \"proxyTypeSocks5\", \"username\": \"%s\", \"password\": \"%s\"}}",
        p.host, p.port, p.user, p.pass);
    
    kyro_net_send(proxy_json);
}
