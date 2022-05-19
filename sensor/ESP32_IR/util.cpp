#include "util.hpp"

#define TOKEN_LENGTH 17
#define CODE_LENGTH 8

char json[48] = "{\"token\":\"00:00:00:00:00:00\",\"code\":\"00000000\"}";

const char* json_get() {
    return json;
}

void json_set_token(const char* token) {
    int t_start = 10;
    for (int i = 0; i < TOKEN_LENGTH; i++) json[t_start+i] = token[i];
}

void json_set_code(char* code) {
    int c_start = 37;
    for (int i = 0; i < CODE_LENGTH; i++) json[c_start + i] = code[i];
}



char toHex(unsigned int nibble) {
  unsigned int part = nibble & 0xF;
  if (part < 10) return (char) part + 48; // (0 = 48)
  else return (char) part + 55; // (A = 65)
}

void uint_to_hex(unsigned int ui, char* hex) {
    for (int i = 0; i < 8; i++) hex[7u-i] = toHex(ui >> i*4);
}