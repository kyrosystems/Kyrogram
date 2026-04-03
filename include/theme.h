#ifndef KYRO_THEME_H
#define KYRO_THEME_H

#include "kyrogram.h"
#include <raylib.h>

typedef struct {
    Color primary;
    Color secondary;
    Color background;
    Color text;
    Color accent;
} KyroPalette;

typedef struct {
    char text[1024];
    bool is_outgoing;
    float height;
} KyroUIMessage;

// UI Core
void kyro_theme_init(KyroUIStyle style);
void kyro_theme_apply(KyroUIStyle style);
void kyro_update_ui_state(void);

// Drawing Elements
void kyro_draw_sidebar(void);
void kyro_draw_chat_item(int index, const char* name, const char* last_msg, bool is_selected);
void kyro_draw_bubble(Rectangle rect, Color color, bool is_outgoing);
void kyro_draw_input_field(Rectangle rect, char* buffer, int buffer_size, bool is_active);
void kyro_draw_message_list(KyroUIMessage* messages, int count, float scroll_y);
void kyro_render_glass(Rectangle rect);

#endif // KYRO_THEME_H
