#include "kyrogram.h"
#include <stdio.h>
#include <stdlib.h>

void kyro_media_process_video_note(const char* input_path, char* output_path) {
    char command[1024];
    sprintf(command, "ffmpeg -y -i \"%s\" -vf \"crop=ih:ih,scale=640:640\" -c:v libx264 -preset superfast -crf 23 -c:a aac \"%s\"", 
            input_path, output_path);
    
    int ret = system(command);
    if (ret == 0) {
        printf("[MEDIA] Video note processed successfully: %s\n", output_path);
    } else {
        fprintf(stderr, "[MEDIA] FFmpeg error during video processing.\n");
    }
}

void kyro_media_encode_voice(const char* input_path, char* output_path) {
    char command[512];
    // Конвертация в OGG OPUS
    sprintf(command, "ffmpeg -y -i \"%s\" -c:a libopus -b:a 32k \"%s\"", input_path, output_path);
    system(command);
}
