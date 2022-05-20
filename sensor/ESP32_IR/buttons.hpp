#ifndef BUTTONS_HPP
#define BUTTONS_HPP

void button_init();
void button_set_callback(int button, void (*button_cb)(int state));
void button_process_input();

#endif /* ifndef BUTTONS_HPP */