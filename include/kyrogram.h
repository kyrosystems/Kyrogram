/**
 * Kyrogram - Modern Telegram Client in C
 * Author: pon4ikisdonut (KyroSystems NUA)
 * Version: 1.0.0-Stable
 * License: MIT
 */

#ifndef KYROGRAM_H
#define KYROGRAM_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

// Versioning System: MAJOR.MINOR.PATCH-STATUS
#define KYRO_VERSION_MAJOR 1
#define KYRO_VERSION_MINOR 1
#define KYRO_VERSION_PATCH 4
#define KYRO_VERSION_STATUS "Stable"

#define KYRO_AUTHOR "pon4ikisdonut (KyroSystems NUA)"

// Global Client State
typedef struct {
    bool is_running;
    int current_theme;
    char* account_id;
    void* td_client; // Pointer to TDLib client
} KyroState;

extern KyroState kyro_state;

// UI Styles
typedef enum {
    STYLE_LIQUID_GLASS,
    STYLE_MATERIAL_YOU,
    STYLE_FLUENT,
    STYLE_SCEUOMORPHISM,
    STYLE_CLASSIC
} KyroUIStyle;

#endif // KYROGRAM_H
