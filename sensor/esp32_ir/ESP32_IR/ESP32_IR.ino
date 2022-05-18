#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <IRremote.h>
#include <IRremoteInt.h>

#define USE_SERIAL Serial

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

void scanIR() {
  if (IrReceiver.decode()) {
    unsigned long ir_value = IrReceiver.decodedIRData.decodedRawData;
    if (ir_value != 0) {
      setOutCode(ir_value);
      last_code = ir_value;
      sensor_read_success = 1;
      //USE_SERIAL.println(ir_value, HEX);
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

    USE_SERIAL.begin(115200);

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
