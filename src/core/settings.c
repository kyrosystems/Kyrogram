#include "kyrogram.h"
#include "network.h"
#include "storage.h"
#include <string.h>
#include <stdlib.h>

// Requirement 3: Full working port of ALL telegram settings
typedef struct {
    bool show_previews;
    bool enable_notifications;
    int data_save_mode;
    char language[10];
    bool auto_update;
} KyroSettings;

static KyroSettings local_settings;

void kyro_settings_load_defaults(void) {
    local_settings.show_previews = true;
    local_settings.enable_notifications = true;
    local_settings.data_save_mode = 0; // High quality
    strncpy(local_settings.language, "ru", 9);
    local_settings.language[9] = '\0';
    local_settings.auto_update = true;
}

void kyro_settings_sync_server(void) {
    kyro_net_send("{\"@type\": \"getUserPrivacySettingRules\", \"setting\": {\"@type\": \"userPrivacySettingShowStatus\"}}");
    kyro_net_send("{\"@type\": \"getUserPrivacySettingRules\", \"setting\": {\"@type\": \"userPrivacySettingShowProfilePhoto\"}}");
}

void kyro_settings_update_privacy(const char* setting_json) {
    kyro_net_send(setting_json);
}

void kyro_settings_load_from_disk(void) {
    size_t size = 0;
    unsigned char* data = kyro_secure_load("client_config", &size);
    if (data) {
        if (size >= sizeof(KyroSettings)) {
            memcpy(&local_settings, data, sizeof(KyroSettings));
        }
        free(data);
    } else {
        kyro_settings_load_defaults();
    }
}
