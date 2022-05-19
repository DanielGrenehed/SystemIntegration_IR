#include <Arduino.h>

#include "network.hpp"
#include "oled.hpp"
#include "util.hpp"
#include "ir.hpp"

#define USE_SERIAL Serial

enum DisplayMode { IR_DISPLAY, WIFI_DISPLAY };
DisplayMode display_mode = IR_DISPLAY;

int sensor_read_success = 0;
int debug_serial = 0;

unsigned int last_valid_signal = 0;
const char* ir_display_header = "IR code:";
const char* url = "http://<hostname>:<port>/ir";
const char* content_type = "application/json";

void displayWiFiInfo() {
  oled_clear();
  oled_add_text(0, 1, "SSID: ");
  oled_set_text(network_ssid());
  oled_add_text(10, 1, "IP: ");
  oled_set_text(network_ip());
  oled_add_text(20, 1, "MAC:");
  oled_set_text(network_mac());
  oled_show();
}

void displayIRInfo() {
  oled_clear();
  oled_add_text(0, 1, ir_display_header);
  char hex[9];
  hex[8] = 0;
  uint_to_hex(last_valid_signal, hex);
  oled_add_text(18, 2, hex);
  oled_show();
}

void setDisplayMode(DisplayMode dm) {
  display_mode = dm;
  if (display_mode == IR_DISPLAY) displayIRInfo();
  else if (display_mode == WIFI_DISPLAY) displayWiFiInfo();
}

void updateIRDisplay(unsigned int code) {
  if (code != last_valid_signal) {
    last_valid_signal = code;
    if (display_mode == IR_DISPLAY) displayIRInfo();
  }
}

void setSensorOutput(unsigned int value) {
  char code[8];
  uint_to_hex(value, code);
  json_set_code(code);
  updateIRDisplay(value);
  sensor_read_success = 1;
}

void onInvalidIRSignal(unsigned int signal) {
  if (debug_serial) {
    USE_SERIAL.print(F("BAD IR SIGNAL "));
    USE_SERIAL.println(signal, HEX);
  }
}

void setup() {
  USE_SERIAL.begin(115200);
  oled_init();
  network_init();

  json_set_token(network_mac());
  ir_init(setSensorOutput, onInvalidIRSignal);
  //setDisplayMode(WIFI_DISPLAY);
}

unsigned long last_time = millis();
unsigned long update_time_millis = 100;

void loop() {
  if (network_is_connected()) {
    if (millis() > last_time + update_time_millis) {
      
      ir_scan();
      if (sensor_read_success) {
        network_post_data(url, json_get(), content_type);
        sensor_read_success = 0; // clear flag, message sent
      }

      last_time = millis();
    }
  } else {
    USE_SERIAL.println(F("No WiFi"));
    network_reconnect();
  }
  delay(2);
}
