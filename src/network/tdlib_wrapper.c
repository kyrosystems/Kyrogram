#include "network.h"
#include "cJSON.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void* client = NULL;

void kyro_net_send(const char* method) {
    if (client) td_json_client_send(client, method);
}

void kyro_net_init_params(void) {
    cJSON *root = cJSON_CreateObject();
    cJSON_AddStringToObject(root, "@type", "setTdlibParameters");
    cJSON_AddBoolToObject(root, "use_test_dc", false);
    cJSON_AddStringToObject(root, "database_directory", "tdlib_data");
    cJSON_AddStringToObject(root, "files_directory", "tdlib_files");
    cJSON_AddBoolToObject(root, "use_file_database", true);
    cJSON_AddBoolToObject(root, "use_chat_info_database", true);
    cJSON_AddBoolToObject(root, "use_message_database", true);
    cJSON_AddBoolToObject(root, "use_secret_chats", true);
    cJSON_AddNumberToObject(root, "api_id", 2040); // Official ID
    cJSON_AddStringToObject(root, "api_hash", "b18441a1ed42944b9e7352f205259445");
    cJSON_AddStringToObject(root, "system_language_code", "ru");
    cJSON_AddStringToObject(root, "device_model", "Kyrogram Desktop");
    cJSON_AddStringToObject(root, "system_version", "Windows 10");
    cJSON_AddStringToObject(root, "application_version", "1.0.0");
    
    char *out = cJSON_PrintUnformatted(root);
    kyro_net_send(out);
    free(out);
    cJSON_Delete(root);
}

void kyro_process_update(const char* json_str) {
    if (!json_str) return;
    cJSON *json = cJSON_Parse(json_str);
    if (!json) return;

    cJSON *type = cJSON_GetObjectItemCaseSensitive(json, "@type");
    if (cJSON_IsString(type) && (type->valuestring != NULL)) {
        if (strcmp(type->valuestring, "updateAuthorizationState") == 0) {
            cJSON *state = cJSON_GetObjectItemCaseSensitive(json, "authorization_state");
            cJSON *state_type = cJSON_GetObjectItemCaseSensitive(state, "@type");
            
            if (strcmp(state_type->valuestring, "authorizationStateWaitTdlibParameters") == 0) {
                kyro_net_init_params();
            } else if (strcmp(state_type->valuestring, "authorizationStateWaitEncryptionKey") == 0) {
                kyro_net_send("{\"@type\": \"checkDatabaseEncryptionKey\", \"encryption_key\": \"kyro_secret_key\"}");
            }
        }
    }
    cJSON_Delete(json);
}

void kyro_net_init(void) {
    client = td_json_client_create();
    kyro_state.td_client = client;
}

void kyro_net_poll(void) {
    if (!client) return;
    const char* result = td_json_client_receive(client, 0.0);
    if (result) kyro_process_update(result);
}

void kyro_send_text(long long chat_id, const char* text) {
    cJSON *root = cJSON_CreateObject();
    cJSON_AddStringToObject(root, "@type", "sendMessage");
    cJSON_AddNumberToObject(root, "chat_id", (double)chat_id);
    
    cJSON *content = cJSON_AddObjectToObject(root, "input_message_content");
    cJSON_AddStringToObject(content, "@type", "inputMessageText");
    cJSON_AddObjectToObject(content, "text");
    cJSON_AddStringToObject(cJSON_GetObjectItem(content, "text"), "text", text);

    char *out = cJSON_PrintUnformatted(root);
    kyro_net_send(out);
    free(out);
    cJSON_Delete(root);
}
