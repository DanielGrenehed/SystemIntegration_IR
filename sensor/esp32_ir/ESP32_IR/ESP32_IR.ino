#include <Arduino.h>
#include <WiFi.h>
#include <Wire.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <IRremote.h>
#include <IRremoteInt.h>
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

#define WIFI_SSID ""
#define WIFI_PASSWORD ""


void initWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  USE_SERIAL.println("Connecting to WiFi ..");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(1000);
  }
  USE_SERIAL.print("IP: ");
  USE_SERIAL.println(WiFi.localIP());
}

DynamicJsonDocument doc(1024);
JsonObject object = doc.to<JsonObject>();
String url = "http://<hostname>:<port>/ir";

unsigned long last_code = 0;

void setOutCode(unsigned long value) {
  object["code"] = String(value, HEX);
}

const char* screen_header = "IR code:";

void setScreenText(const char* text) {
    display.clearDisplay();
    display.setTextSize(1);
    display.setCursor(0,0);
    display.println(text);
    display.display();
}

unsigned long scan_code = 0;
void updateScanCode(unsigned long code) {
  if (code != scan_code) {
    scan_code = code;
    setScreenText(screen_header);
    display.setCursor(0, 10);
    display.setTextSize(2);
    display.println(String(scan_code, HEX));
    display.display();
  }
}
void onInvalidIRCode(unsigned long code) {
  USE_SERIAL.println("BAD CODE");
}

const unsigned long bad_code_mask = 0x000000FF;
void scanIR() {
  if (IrReceiver.decode()) {
    unsigned long ir_value = IrReceiver.decodedIRData.decodedRawData;
    if (ir_value != 0) {
      // fix invalid codes 
      if (ir_value & bad_code_mask) onInvalidIRCode(ir_value);
      else {
        setOutCode(ir_value);
        last_code = ir_value;
        sensor_read_success = 1;
        updateScanCode(ir_value);
      //USE_SERIAL.println(ir_value, HEX);
      }
      
    }
    IrReceiver.resume();
  }
}

void sendData() {
  scanIR();
  if (sensor_read_success) {
    HTTPClient http;

    http.begin(url); //HTTP
    http.addHeader("Content-Type", "application/json");
    USE_SERIAL.print("[HTTP] POST...");
    String body;
    serializeJson(doc, body);
    USE_SERIAL.printf(body.c_str());
    int httpCode = http.POST(body);
    if(httpCode > 0) {
      USE_SERIAL.printf("[HTTP] POST... code: %d\n", httpCode);
      if(httpCode == HTTP_CODE_OK) USE_SERIAL.println(http.getString());
    } else {
      USE_SERIAL.printf("[HTTP] POST... failed, error: %s\n", http.errorToString(httpCode).c_str());
    }

    http.end();
    sensor_read_success = 0; // clear flag, message sent
  }
}

void setup() {
    pinMode(oled_power_pin, OUTPUT);
    digitalWrite(oled_power_pin, HIGH);
    USE_SERIAL.begin(115200);
    if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3C for 128x32
      Serial.println(F("SSD1306 allocation failed"));
    } else {
      display.setTextColor(WHITE);
      char* str = "ESP32_IR_OLED";
      setScreenText(str);
    }
    
    USE_SERIAL.println();
    USE_SERIAL.println();
    USE_SERIAL.println();

    for(uint8_t t = 4; t > 0; t--) {
        USE_SERIAL.printf("[SETUP] WAIT %d...\n", t);
        USE_SERIAL.flush();
        delay(1000);
    }
    initWiFi();
    object["token"] = WiFi.macAddress();
    USE_SERIAL.printf(WiFi.macAddress().c_str());
    IrReceiver.begin(ir_receive_pin);
    
}

unsigned long last_time = millis(); 
unsigned long update_time_millis = 100;

void loop() {
    if(WiFi.status() == WL_CONNECTED) {
      if (millis() > last_time + update_time_millis) {
         sendData();
         last_time = millis();
      }
      
    } else {
      USE_SERIAL.println("No WiFi");
      WiFi.disconnect();
      WiFi.reconnect();
    }
    
    delay(10);
}
