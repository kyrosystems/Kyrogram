#include "kyrogram.h"
#include "theme.h"
#include "storage.h"
#include "network.h"

KyroState kyro_state = {
    .is_running = true,
    .current_theme = STYLE_LIQUID_GLASS,
    .account_id = "default",
    .td_client = NULL
};

// Messaging State
static char input_buffer[256] = {0};
static int input_len = 0;
static KyroUIMessage messages[100];
static int message_count = 0;

void add_local_message(const char* text, bool outgoing) {
    if (message_count < 100) {
        strcpy(messages[message_count].text, text);
        messages[message_count].is_outgoing = outgoing;
        message_count++;
    }
}

int main(int argc, char** argv) {
    kyro_crypto_init();
    kyro_theme_init(kyro_state.current_theme);
    kyro_net_init();
    kyro_net_setup_smart_proxy();

    // Начальные сообщения
    add_local_message("Welcome to Kyrogram!", false);
    add_local_message("Type something and press Enter", false);

    while (!WindowShouldClose() && kyro_state.is_running) {
        kyro_net_poll();
        kyro_update_ui_state();
        
        // --- ОБРАБОТКА ВВОДА ---
        int key = GetCharPressed();
        while (key > 0) {
            if ((key >= 32) && (key <= 125) && (input_len < 255)) {
                input_buffer[input_len] = (char)key;
                input_buffer[input_len + 1] = '\0';
                input_len++;
            }
            key = GetCharPressed();
        }

        if (IsKeyPressed(KEY_BACKSPACE)) {
            if (input_len > 0) {
                input_len--;
                input_buffer[input_len] = '\0';
            }
        }

        if (IsKeyPressed(KEY_ENTER) && input_len > 0) {
            // 1. Отправляем в Telegram через наш Core
            kyro_send_text(0, input_buffer); // 0 - ID чата для теста
            
            // 2. Добавляем в UI
            add_local_message(input_buffer, true);
            
            // 3. Очищаем буфер
            input_buffer[0] = '\0';
            input_len = 0;
        }

        BeginDrawing();
            ClearBackground((Color){15, 15, 20, 255});
            
            // Сайдбар
            kyro_draw_sidebar();
            kyro_draw_chat_item(0, "KyroSystems", "Online", true);
            
            // История сообщений
            kyro_draw_message_list(messages, message_count, 0);
            
            // Поле ввода
            Rectangle input_rect = { 320, GetScreenHeight() - 60, GetScreenWidth() - 340, 45 };
            kyro_draw_input_field(input_rect, input_buffer, 256, true);
            
            // Статус
            DrawText("MTProto Secure Session Active", 320, 20, 12, GREEN);
            
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
