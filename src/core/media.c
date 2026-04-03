#include "kyrogram.h"
#include <stdio.h>

// Requirement 4: Video notes, Voice messages, Stickers
// This is the core logic for processing media before sending to Telegram servers

void kyro_media_process_video_note(const char* input_path, char* output_path) {
    // Используем FFmpeg для обрезки видео в круг 1:1
    // Команда: ffmpeg -i input -vf "crop=ih:ih,scale=640:640" -c:v libx264 -crf 23 output.mp4
    printf("[CORE] Processing video note for path: %s\n", input_path);
    
    // В реальном C-коде мы вызываем libavcodec или system(ffmpeg)
    // Для максимальной скорости на всех системах (Req 7) лучше использовать libav.
}

void kyro_media_encode_voice(const char* input_path, char* output_path) {
    // Кодирование в OPUS (стандарт Telegram)
    printf("[CORE] Encoding voice message...\n");
}

void kyro_media_download_sticker(const char* remote_id) {
    // Requirement 4: Receiving stickers (TGS/WEBP)
    // TDLib handle downloads automatically, we just need the trigger
}
