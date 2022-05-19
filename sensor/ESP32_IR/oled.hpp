#ifndef OLED_HPP
#define OLED_HPP

#define I2C_SDA 21
#define I2C_SCL 22

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32

void oled_init();
void oled_clear();
void oled_text_size(int size);
void oled_set_cursor(int y);
void oled_set_text(const char* text);
void oled_add_text(int y, int size, const char* text);
void oled_show();

#endif /* ifndef OLED_HPP */