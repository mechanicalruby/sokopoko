#include "string.hpp"

namespace Turbine {
void queue_bitmap_string(Batch& batch, const char* string, float x, float y) {
    int line = 0;
    int offset = 0;
    size_t len = strlen(string);
    Sprite char_sprite;
    char_sprite.centered = false;

    // create lookup table for character indices (only needs to be done once)
    static int char_indices[128] = {0};
    static int initialized = 0;
    
    if (!initialized) {
        static const char* monogram_char_map = " !\"#$%&'()*+,-./"
            "0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`"
            "abcdefghijklmnopqrstuvwxyz{|}~";
            
        // initialize all to -1 (not found)
        for (int i = 0; i < 128; i++) {
            char_indices[i] = -1;
        }
        
        // fill in the indices for characters in our map
        for (int i = 0; monogram_char_map[i] != '\0'; i++) {
            unsigned char c = monogram_char_map[i];
            if (c < 128) {
                char_indices[c] = i;
            }
        }
        initialized = 1;
    }

    for(int i = 0; i < len; i++) {
        char c = string[i];
        
        if(c == '\n') {
            line++;
            offset = 0;
            continue;
        }
        
        // use lookup table for faster indexing
        int char_index = (c < 128) ? char_indices[(unsigned char)c] : -1;

        if(char_index != -1) {
            size_t row = char_index / MONOGRAM_SUBTEX_WIDTH;
            size_t col = char_index % MONOGRAM_SUBTEX_WIDTH;

            float char_x = col * 6.0f;
            float char_y = row * 12.0f;
            
            char_sprite.position.x = x + (offset * 6.0f);
            char_sprite.position.y = y + (line * 12.0f);
            char_sprite.region = Rect{char_x, char_y, 6.0f, 12.0f};
        
            batch.queue(char_sprite);
        }

        offset++;
    }
}
}
