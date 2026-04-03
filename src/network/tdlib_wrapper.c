#include "network.h"
#include <stdio.h>
#include <string.h>

#define TELEGRAM_API_ID 27827667
#define TELEGRAM_API_HASH "8f885d1bebe32da81c42677c42aed659"

// Static prototypes to avoid implicit declaration warnings
void kyro_net_init_params(void);

static void* client = NULL;

void kyro_net_init(void) {
    client = td_json_client_create();
    kyro_state.td_client = client;
    kyro_net_init_params();
}

void kyro_net_init_params(void) {
    char params[2048];
    sprintf(params, 
        "{\"@type\": \"setTdlibParameters\", \"use_test_dc\": false, \"database_directory\": \"tdlib_data\", "
        "\"files_directory\": \"files\", \"use_file_database\": true, \"use_chat_info_database\": true, "
        "\"use_message_database\": true, \"use_secret_chats\": true, \"api_id\": %d, \"api_hash\": \"%s\", "
        "\"system_language_code\": \"ru\", \"device_model\": \"KyroGram Desktop\", \"system_version\": \"1.0.0\", "
        "\"application_version\": \"1.0.0\", \"enable_storage_optimizer\": true}", 
        TELEGRAM_API_ID, TELEGRAM_API_HASH);
    
    kyro_net_send(params);
}

void kyro_net_send(const char* method) {
    if (client) {
        td_json_client_send(client, method);
    }
}

void kyro_net_poll(void) {
    if (!client) return;
    const char* result = td_json_client_receive(client, 0.0);
    if (result) {
        if (strstr(result, "updateAuthorizationState")) {
            if (strstr(result, "authorizationStateWaitTdlibParameters")) {
                kyro_net_init_params();
            } else if (strstr(result, "authorizationStateWaitEncryptionKey")) {
                kyro_net_send("{\"@type\": \"checkDatabaseEncryptionKey\", \"encryption_key\": \"kyro_secure_storage_v1\"}");
            }
        }
    }
}

void kyro_send_text(long long chat_id, const char* text) {
    char cmd[2048];
    sprintf(cmd, "{\"@type\": \"sendMessage\", \"chat_id\": %lld, \"input_message_content\": "
                 "{\"@type\": \"inputMessageText\", \"text\": {\"text\": \"%s\"}}}", chat_id, text);
    kyro_net_send(cmd);
}

void kyro_net_setup_proxy(const char* server, int port, const char* user, const char* pass) {
    char proxy_json[1024];
    if (user && strlen(user) > 0) {
        sprintf(proxy_json, 
            "{\"@type\": \"addProxy\", \"server\": \"%s\", \"port\": %d, \"enable\": true, "
            "\"type\": {\"@type\": \"proxyTypeSocks5\", \"username\": \"%s\", \"password\": \"%s\"}}",
            server, port, user, pass);
    } else {
        sprintf(proxy_json, 
            "{\"@type\": \"addProxy\", \"server\": \"%s\", \"port\": %d, \"enable\": true, "
            "\"type\": {\"@type\": \"proxyTypeSocks5\"}}",
            server, port);
    }
    kyro_net_send(proxy_json);
}

void kyro_net_setup_smart_proxy(void) {
    kyro_net_setup_proxy("proxy.kyrogram.net", 443, "kyro", "secure");
}
