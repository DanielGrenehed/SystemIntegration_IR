#include <Arduino.h>
#include <WiFi.h>

#define WIFI_SSID ""
#define WIFI_PASSWORD ""

// Data posting
#include <HTTPClient.h>

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

const int ir_receive_pin = 25;
int sensor_read_success = 0;
int debug_serial = 0;

char JSONoutput[48] = "{\"token\":\"00:00:00:00:00:00\",\"code\":\"00000000\"}";

void setToken(const char* token) {
  int t_start = 10;
  for (int i = 0; i < 17; i++) {
    JSONoutput[t_start+i] = token[i];
  }
}

void setCode(char* code) {
  int c_start = 37;
  for (int i = 0; i < 8; i++) {
    JSONoutput[c_start + i] = code[i];
  }
}

void initWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  USE_SERIAL.println(F("Connecting to WiFi"));
  while (WiFi.status() != WL_CONNECTED) {
    USE_SERIAL.print('.');
    delay(1000);
  }
  USE_SERIAL.println();
  USE_SERIAL.print(F("SSID: "));
  USE_SERIAL.println(WiFi.SSID());
  USE_SERIAL.print(F("IP: "));
  USE_SERIAL.println(WiFi.localIP());
  USE_SERIAL.print(F("MAC: "));
  USE_SERIAL.println(WiFi.macAddress());
}

char toHex(unsigned int nibble) {
  unsigned int part = nibble & 0xF;
  if (part < 10) {
    return (char) part + 48; // (0 = 48)
  } else {
    return (char) part + 55; // (A = 65)
  }
}

void uitoca(unsigned int ui, char* ca) {
  for (int i = 0; i < 8; i++) {
    ca[7u-i] = toHex(ui >> i*4);
  }
}

void setOutCode(unsigned long value) {
  char code[8];
  uitoca(value, code);
  setCode(code);
}

void setScreenText(const char* text) {
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println(text);
  display.display();
}

const char* screen_header = "IR code:";
unsigned int scan_code = 0;

void updateScanCode(unsigned int code) {
  if (code != scan_code) {
    scan_code = code;
    setScreenText(screen_header);
    display.setCursor(0, 18);
    display.setTextSize(2);
    display.println(scan_code, HEX);
    display.display();
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

  if (id == last_id) { // not a reflection
    last_id = id;
    return 1;

  } else {
    last_id = id;
    return 0;
  }
}

void scanIR() {
  if (IrReceiver.decode()) {
    unsigned int ir_signal = IrReceiver.decodedIRData.decodedRawData;
    if (ir_signal < 0x100 && ir_signal > 0.) ir_signal += 0x10000;

    if (ir_signal != 0) {
      if (!isValidSignal(ir_signal)) onInvalidIRSignal(ir_signal);
      else {
        setOutCode(ir_signal);
        sensor_read_success = 1;
        updateScanCode(ir_signal);
      }
    }
    IrReceiver.resume();
  }
}

const char* url = "http://<hostname>:<port>/ir";

void sendData() {
  scanIR();
  if (sensor_read_success) {
    HTTPClient http;

    http.begin(url); //HTTP
    http.addHeader(F("Content-Type"), F("application/json"));
    USE_SERIAL.print(F("[HTTP] POST..."));
    USE_SERIAL.printf(JSONoutput);
    int httpCode = http.POST(JSONoutput);
    if (httpCode > 0) {
      USE_SERIAL.printf("[HTTP] POST... code: %d\n", httpCode);
      if (httpCode == HTTP_CODE_OK) USE_SERIAL.println(http.getString());
    } else {
      USE_SERIAL.printf("[HTTP] POST... failed, error: %s\n", http.errorToString(httpCode).c_str());
    }

    http.end();
    sensor_read_success = 0; // clear flag, message sent
  }
}

void setup() {
  USE_SERIAL.begin(115200);
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3C for 128x32
    Serial.println(F("SSD1306 allocation failed"));
  } else {
    display.setTextColor(WHITE);
    const char* str = "ESP32_IR_OLED";
    setScreenText(str);
  }

  initWiFi();
  setToken(WiFi.macAddress().c_str());
  IrReceiver.begin(ir_receive_pin);

}

unsigned long last_time = millis();
unsigned long update_time_millis = 100;

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    if (millis() > last_time + update_time_millis) {
      sendData();
      last_time = millis();
    }

  } else {
    USE_SERIAL.println(F("No WiFi"));
    WiFi.disconnect();
    WiFi.reconnect();
  }

  delay(10);
}
