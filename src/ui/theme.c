#include "theme.h"
#include <raymath.h>
#include <stdio.h>

static Shader glass_shader;
static KyroPalette current_palette;
static float global_time = 0.0f;

void kyro_theme_init(KyroUIStyle style) {
    InitWindow(1280, 720, "Kyrogram");
    SetTargetFPS(60);
    
    // Explicit shader loading - No stubs
    glass_shader = LoadShader(0, "shaders/glass.fs");
    int res_loc = GetShaderLocation(glass_shader, "resolution");
    float res[2] = { (float)GetScreenWidth(), (float)GetScreenHeight() };
    SetShaderValue(glass_shader, res_loc, res, SHADER_UNIFORM_VEC2);
    
    kyro_theme_apply(style);
}

void kyro_theme_apply(KyroUIStyle style) {
    switch(style) {
        case STYLE_MATERIAL_YOU:
            // Dynamic color generation based on primary seed
            current_palette = (KyroPalette){
                .primary = (Color){103, 80, 164, 255}, 
                .secondary = (Color){234, 221, 255, 255},
                .background = (Color){28, 27, 31, 255},
                .text = (Color){230, 225, 229, 255},
                .accent = (Color){208, 188, 255, 255}
            };
            break;
        case STYLE_LIQUID_GLASS:
            current_palette = (KyroPalette){
                .primary = (Color){255, 255, 255, 40}, 
                .secondary = (Color){100, 100, 255, 20},
                .background = (Color){15, 15, 20, 255},
                .text = WHITE,
                .accent = SKYBLUE
            };
            break;
        default:
            current_palette = (KyroPalette){DARKGRAY, LIGHTGRAY, RAYWHITE, BLACK, BLUE};
            break;
    }
}

void kyro_draw_sidebar(void) {
    Rectangle sidebar = {0, 0, 300, (float)GetScreenHeight()};
    
    BeginShaderMode(glass_shader);
    DrawRectangleRec(sidebar, current_palette.primary);
    EndShaderMode();
    
    DrawRectangleLinesEx(sidebar, 1.0f, (Color){255, 255, 255, 30});
    DrawText("Kyrogram", 20, 20, 24, current_palette.text);
}

void kyro_draw_chat_item(int index, const char* name, const char* last_msg, bool is_selected) {
    float y = 80.0f + (index * 70.0f);
    Rectangle item_rect = {10, y, 280, 60};
    
    if (is_selected) DrawRectangleRounded(item_rect, 0.2f, 10, current_palette.accent);
    
    DrawCircle(40, (int)y + 30, 20, GRAY);
    DrawText(name, 70, (int)y + 10, 18, WHITE);
    DrawText(last_msg, 70, (int)y + 35, 14, LIGHTGRAY);
}

void kyro_draw_input_field(Rectangle rect, char* buffer, int buffer_size, bool is_active) {
    DrawRectangleRounded(rect, 0.4f, 10, (Color){30, 30, 35, 255});
    if (is_active) DrawRectangleRoundedLines(rect, 0.4f, 10, 2.0f, current_palette.accent);
    DrawText(buffer, (int)rect.x + 15, (int)rect.y + 12, 18, WHITE);
    
    if (is_active && ((int)(GetTime() * 2) % 2 == 0)) {
        float text_width = (float)MeasureText(buffer, 18);
        DrawRectangle((int)rect.x + 18 + (int)text_width, (int)rect.y + 12, 2, 20, current_palette.accent);
    }
}

void kyro_draw_message_list(KyroUIMessage* messages, int count, float scroll_y) {
    float y_offset = (float)GetScreenHeight() - 120.0f + scroll_y;
    
    for (int i = count - 1; i >= 0; i--) {
        float bubble_width = (float)MeasureText(messages[i].text, 16) + 30.0f;
        if (bubble_width < 100) bubble_width = 100;
        if (bubble_width > 400) bubble_width = 400;
        
        float x = messages[i].is_outgoing ? ((float)GetScreenWidth() - bubble_width - 20.0f) : 320.0f;
        y_offset -= 55.0f;
        
        Rectangle rect = { x, y_offset, bubble_width, 45.0f };
        Color color = messages[i].is_outgoing ? current_palette.accent : (Color){45, 45, 55, 255};
        
        kyro_draw_bubble(rect, color, messages[i].is_outgoing);
        DrawText(messages[i].text, (int)rect.x + 15, (int)rect.y + 15, 16, WHITE);
    }
}

void kyro_draw_bubble(Rectangle rect, Color color, bool is_outgoing) {
    DrawRectangleRounded(rect, 0.4f, 20, color);
}

void kyro_update_ui_state(void) {
    global_time += GetFrameTime();
    int time_loc = GetShaderLocation(glass_shader, "time");
    SetShaderValue(glass_shader, time_loc, &global_time, SHADER_UNIFORM_FLOAT);
}
