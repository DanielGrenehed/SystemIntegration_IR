#include <Arduino.h>
#include "network.hpp"
// IR Receiver
#include <IRremote.h>
#include <IRremoteInt.h>

// I2C OLED
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define USE_SERIAL Serial
#define I2C_SDA 21
#define I2C_SCL 22

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

enum DisplayMode { IR_DISPLAY, WIFI_DISPLAY };
DisplayMode display_mode = IR_DISPLAY;

const int ir_receive_pin = 25;
int sensor_read_success = 0;
int debug_serial = 0;

#define TOKEN_LENGTH 17
#define CODE_LENGTH 8

char json[48] = "{\"token\":\"00:00:00:00:00:00\",\"code\":\"00000000\"}";

void JSONSetToken(const char* token) {
  int t_start = 10;
  for (int i = 0; i < TOKEN_LENGTH; i++) {
    json[t_start+i] = token[i];
  }
}

void JSONSetCode(char* code) {
  int c_start = 37;
  for (int i = 0; i < CODE_LENGTH; i++) {
    json[c_start + i] = code[i];
  }
}

void setScreenText(const char* text) {
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println(text);
  display.display();
}

void initDisplay() {
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3C for 128x32
    USE_SERIAL.println(F("SSD1306 allocation failed"));
  } else {
    display.setTextColor(WHITE);
    const char* str = "ESP32_IR_OLED";
    setScreenText(str);
  }
}



void displayWiFiInfo() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.print("SSID: ");
  display.println(network_ssid());
  display.setCursor(0, 10);
  display.print("IP: ");
  display.println(network_ip());
  display.setCursor(0, 20);
  display.print("MAC:");
  display.println(network_mac());
  display.display();
}

const char* screen_header = "IR code:";
unsigned int scan_code = 0;

void displayIRInfo() {
    setScreenText(screen_header);
    display.setCursor(0, 18);
    display.setTextSize(2);
    display.println(scan_code, HEX);
    display.display();
}

void setDisplayMode(DisplayMode dm) {
  display_mode = dm;
  if (display_mode == IR_DISPLAY) displayIRInfo();
  else if (display_mode == WIFI_DISPLAY) displayWiFiInfo();
}

char toHex(unsigned int nibble) {
  unsigned int part = nibble & 0xF;
  if (part < 10) return (char) part + 48; // (0 = 48)
  else return (char) part + 55; // (A = 65)
}

void uitoHex(unsigned int ui, char* ca) {
  for (int i = 0; i < 8; i++) ca[7u-i] = toHex(ui >> i*4);
}

void setSensorOutput(unsigned long value) {
  char code[8];
  uitoHex(value, code);
  JSONSetCode(code);
  sensor_read_success = 1;
}

void updateScanCode(unsigned int code) {
  if (code != scan_code) {
    scan_code = code;
    if (display_mode == IR_DISPLAY) displayIRInfo();
  }
}

void onInvalidIRSignal(unsigned int signal) {
  if (debug_serial) {
    USE_SERIAL.print(F("BAD IR SIGNAL "));
    USE_SERIAL.println(signal, HEX);
  }
}

unsigned int last_id = 0x00000000;
const unsigned int signal_id_mask = 0x000000FF;

int isValidSignal(unsigned int signal) {
  if (!(signal & 0xFFF00000)) return 1; // short signal with no id
  unsigned int id = signal & signal_id_mask;
  int result = 0;
  if (id == last_id) result = 1; 
  last_id = id;
  return result;
}

void scanIR() {
  if (IrReceiver.decode()) {
    unsigned int ir_signal = IrReceiver.decodedIRData.decodedRawData;
    if (ir_signal < 0x100 && ir_signal > 0.) ir_signal += 0x10000;

    if (ir_signal != 0) {
      if (!isValidSignal(ir_signal)) onInvalidIRSignal(ir_signal);
      else {
        setSensorOutput(ir_signal);
        updateScanCode(ir_signal);
      }
    }
    IrReceiver.resume();
  }
}

const char* url = "http://<hostname>:<port>/ir";
const char* content_type = "application/json";

void sendData() {
  scanIR();
  if (sensor_read_success) {
    
    network_post_data(url, json, content_type);
    sensor_read_success = 0; // clear flag, message sent
  }
}

void setup() {
  USE_SERIAL.begin(115200);
  initDisplay();
  network_init();

  JSONSetToken(network_mac());
  IrReceiver.begin(ir_receive_pin);
  //setDisplayMode(WIFI_DISPLAY);
}

unsigned long last_time = millis();
unsigned long update_time_millis = 100;

void loop() {
  if (network_is_connected()) {
    if (millis() > last_time + update_time_millis) {
      sendData();
      last_time = millis();
    }
  } else {
    USE_SERIAL.println(F("No WiFi"));
    network_reconnect();
  }
  delay(2);
}
