#include "buttons.hpp"
#include <Arduino.h>

#define BUTTON_COUNT 3
#define DEBOUNCE_TIME_MS 10

int button_pins[] = {33, 25, 32};
void (*button_callbacks[BUTTON_COUNT])(int);
unsigned long button_debounce[] = {0, 0, 0};
unsigned int button_state[] = {0, 0, 0};

void interrupt_debounce() {
    for (int i = 0; i < BUTTON_COUNT; i++) {
        if (button_debounce[i]) continue;
        if (button_state[i] != digitalRead(button_pins[i])) {
            button_debounce[i] = millis();
        } 
    }
}

void button_init() {
    for (int i = 0; i < BUTTON_COUNT; i++) {
        pinMode(button_pins[i], INPUT_PULLDOWN);
        attachInterrupt(button_pins[i], interrupt_debounce, CHANGE);    
    }
}

void button_set_callback(int button, void (*button_cb)(int state)) {
    if (button >= BUTTON_COUNT || button < 0) return;
    button_callbacks[button] = button_cb;
}

void button_process_input() {
    for (int i = 0; i < BUTTON_COUNT; i++) {
        if (button_debounce[i]) {
            if (millis() > button_debounce[i] + DEBOUNCE_TIME_MS) {
                int current_state = digitalRead(button_pins[i]);
                if (button_state[i] != current_state) {
                    button_callbacks[i](current_state);
                    button_state[i] = current_state;
                }
                button_debounce[i] = 0;
            }
        }
    }
}
