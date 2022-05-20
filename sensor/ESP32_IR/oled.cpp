#include <Arduino.h>
#include "oled.hpp"
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void oled_init() {
    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3C for 128x32
    Serial.println(F("SSD1306 allocation failed"));
  } else {
    display.setTextColor(WHITE);
    oled_clear();
    oled_text_size(1);
    oled_set_cursor(0);
    oled_set_text("ESP32_IR_OLED");
    oled_show();
  }
}

void oled_clear() {
    display.clearDisplay();
}

void oled_text_size(int size) {
    display.setTextSize(size);
}

void oled_set_text(const char* text) {
    display.print(text);
}

void oled_set_cursor(int y) {
    display.setCursor(0, y);
}

void oled_add_text(int y, int size, const char* text) {
    oled_set_cursor(y);
    oled_text_size(size);
    oled_set_text(text);
}

void oled_show() {
    display.display();
}