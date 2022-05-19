#ifndef IR_HPP
#define IR_HPP

void ir_init(void (*valid_cb) (unsigned int signal), void (*invalid_cb) (unsigned int signal));

void ir_scan();

#endif /* ifndef IR_HPP */