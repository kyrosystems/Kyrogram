#include "theme.h"
#include <raymath.h>
#include <stdio.h>

static Shader glass_shader;
static KyroPalette current_palette;
static float global_time = 0.0f;

void kyro_theme_init(KyroUIStyle style) {
    InitWindow(1280, 720, "Kyrogram");
    SetTargetFPS(60);
    
    // Загрузка шейдеров
    glass_shader = LoadShader(0, "shaders/glass.fs");
    int res_loc = GetShaderLocation(glass_shader, "resolution");
    float res[2] = { (float)GetScreenWidth(), (float)GetScreenHeight() };
    SetShaderValue(glass_shader, res_loc, res, SHADER_UNIFORM_VEC2);
    
    kyro_theme_apply(style);
}

void kyro_theme_apply(KyroUIStyle style) {
    if (style == STYLE_MATERIAL_YOU) {
        // Динамическая генерация палитры (Material You)
        // В реальном Kyrogram мы берем цвет из обоев системы
        Color seed = (Color){66, 133, 244, 255}; // Google Blue
        current_palette.primary = seed;
        current_palette.secondary = (Color){seed.r + 20, seed.g + 20, seed.b + 40, 255};
        current_palette.background = (Color){20, 20, 25, 255};
        current_palette.text = WHITE;
        current_palette.accent = seed;
    } else if (style == STYLE_LIQUID_GLASS) {
        current_palette.background = (Color){10, 10, 15, 255};
        current_palette.primary = (Color){255, 255, 255, 30};
        current_palette.text = WHITE;
    }
}

// Requirement 2: Отрисовка элементов UI
void kyro_draw_sidebar(void) {
    Rectangle sidebar = {0, 0, 300, GetScreenHeight()};
    
    // Glass effect for sidebar
    BeginShaderMode(glass_shader);
    DrawRectangleRec(sidebar, current_palette.primary);
    EndShaderMode();
    
    DrawRectangleLinesEx(sidebar, 1.0f, (Color){255, 255, 255, 20});
    
    // Header
    DrawText("Kyrogram", 20, 20, 24, current_palette.text);
}

void kyro_draw_chat_item(int index, const char* name, const char* last_msg, bool is_selected) {
    float y = 80 + (index * 70);
    Rectangle item_rect = {10, y, 280, 60};
    
    if (is_selected) {
        DrawRectangleRounded(item_rect, 0.2f, 10, current_palette.accent);
    }
    
    // Avatar placeholder
    DrawCircle(40, y + 30, 20, DARKGRAY);
    DrawText(name, 70, y + 10, 18, WHITE);
    DrawText(last_msg, 70, y + 35, 14, LIGHTGRAY);
}

void kyro_draw_input_field(Rectangle rect, char* buffer, int buffer_size, bool is_active) {
    // Внешняя рамка
    DrawRectangleRounded(rect, 0.4f, 10, (Color){30, 30, 35, 255});
    if (is_active) DrawRectangleRoundedLines(rect, 0.4f, 10, 2, current_palette.accent);
    
    // Текст
    DrawText(buffer, rect.x + 15, rect.y + 12, 18, WHITE);
    
    // Курсор (мигающий)
    if (is_active && ((int)(GetTime() * 2) % 2 == 0)) {
        float text_width = MeasureText(buffer, 18);
        DrawRectangle(rect.x + 18 + text_width, rect.y + 12, 2, 20, current_palette.accent);
    }
}

void kyro_draw_message_list(KyroUIMessage* messages, int count, float scroll_y) {
    float y_offset = GetScreenHeight() - 120 + scroll_y;
    
    for (int i = count - 1; i >= 0; i--) {
        float bubble_width = MeasureText(messages[i].text, 16) + 30;
        if (bubble_width < 100) bubble_width = 100;
        if (bubble_width > 400) bubble_width = 400;
        
        float x = messages[i].is_outgoing ? (GetScreenWidth() - bubble_width - 20) : 320;
        y_offset -= 55;
        
        Rectangle rect = { x, y_offset, bubble_width, 45 };
        Color color = messages[i].is_outgoing ? current_palette.accent : (Color){45, 45, 55, 255};
        
        kyro_draw_bubble(rect, color, messages[i].is_outgoing);
        DrawText(messages[i].text, rect.x + 15, rect.y + 15, 16, WHITE);
    }
}

void kyro_update_ui_state(void) {
    global_time += GetFrameTime();
    int time_loc = GetShaderLocation(glass_shader, "time");
    SetShaderValue(glass_shader, time_loc, &global_time, SHADER_UNIFORM_FLOAT);
}
