#ifndef UTIL_HPP
#define UTIL_HPP

const char* json_get();
void json_set_token(const char* token);
void json_set_code(char* code);

void uint_to_hex(unsigned int ui, char * hex_out);

#endif /* ifndef UTIL_HPP */