#include "../../include/fern/text/font.hpp"
#include "../../include/fern/core/canvas.hpp"
#include "font_data.hpp"
#include <cstring>

namespace Fern {
    namespace DrawText {
        void drawChar(char c, int x, int y, int scale, uint32_t color) {
            if (!globalCanvas) return;
            
            int char_index;
            
            if (c >= 'A' && c <= 'Z') {
                char_index = c - 'A';  // 0-25 (A-Z)
            } else if (c >= '0' && c <= '9') {
                char_index = 26 + (c - '0');  // 26-35 (0-9)
            } else if (c >= 'a' && c <= 'z') {
                char_index = 36 + (c - 'a');  // 36-61 (a-z)
            } else {
                return;
            }
            
            for (int row = 0; row < 8; row++) {
                unsigned char row_bits = FontData::SIMPLE_FONT[char_index][row];
                
                for (int col = 0; col < 8; col++) {
                    if (row_bits & (1 << (7 - col))) {
                        int base_x = x + col * scale;
                        int base_y = y + row * scale;
                        
                        for (int sy = 0; sy < scale; sy++) {
                            for (int sx = 0; sx < scale; sx++) {
                                int px = base_x + sx;
                                int py = base_y + sy;
                                
                                if (px >= 0 && px < globalCanvas->getWidth() && 
                                    py >= 0 && py < globalCanvas->getHeight()) {
                                    globalCanvas->getBuffer()[py * globalCanvas->getWidth() + px] = color;
                                }
                            }
                        }
                    }
                }
            }
        }
        
        void drawText(const char* text, int x, int y, int scale, uint32_t color) {
            int cursor_x = x;
            for (const char* p = text; *p != '\0'; p++) {
                if (*p == ' ') {
                    cursor_x += 4 * scale;
                    continue;
                } else if ((*p >= 'A' && *p <= 'Z') || (*p >= 'a' && *p <= 'z') || (*p >= '0' && *p <= '9')) {
                    drawChar(*p, cursor_x, y, scale, color);
                    cursor_x += 8 * scale;
                } else {
                    // For unsupported characters, just add some space
                    cursor_x += 4 * scale;
                }
            }
        }
    }
}