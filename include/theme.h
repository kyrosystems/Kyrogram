#ifndef KYRO_THEME_H
#define KYRO_THEME_H

#include "kyrogram.h"
#include <raylib.h>

// Color pallet for Material You
typedef struct {
    Color primary;
    Color secondary;
    Color background;
    Color text;
    Color accent;
} KyroPalette;

// Message structure for rendering
typedef struct {
    char text[1024];
    bool is_outgoing;
    float height;
} KyroUIMessage;

// Initialize the theme system
void kyro_theme_init(KyroUIStyle style);

// Apply a theme to the current rendering context
void kyro_theme_apply(KyroUIStyle style);

// Draw a stylized UI element (e.g., a message bubble)
void kyro_draw_bubble(Rectangle rect, Color color, bool is_outgoing);

// Render the "Liquid Glass" effect using shaders
void kyro_render_glass(Rectangle rect);

// New UI Elements for Messaging
void kyro_draw_input_field(Rectangle rect, char* buffer, int buffer_size, bool is_active);
void kyro_draw_message_list(KyroUIMessage* messages, int count, float scroll_y);

#endif // KYRO_THEME_H
