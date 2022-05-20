#include <Arduino.h>
#include <WiFi.h>
#include "network.hpp"
// Data posting
#include <HTTPClient.h>


void network_init() {
    WiFi.mode(WIFI_STA);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    Serial.println(F("Connecting to WiFi"));
    while (WiFi.status() != WL_CONNECTED) {
        Serial.print('.');
        delay(1000);
    }

    Serial.println();
    Serial.print(F("SSID: "));
    Serial.println(WiFi.SSID());
    Serial.print(F("IP: "));
    Serial.println(WiFi.localIP());
    Serial.print(F("MAC: "));
    Serial.println(WiFi.macAddress());
}

char t_ssid[32];
const char* network_ssid() {
    strcpy(t_ssid, WiFi.SSID().c_str()); 
    return t_ssid;
}

char t_ip[16];
const char* network_ip() {
    strcpy(t_ip, WiFi.localIP().toString().c_str());
    return t_ip;
}

const char* network_mac() {
    return WiFi.macAddress().c_str();
}

int network_is_connected() {
  return (WiFi.status() == WL_CONNECTED);
}

void network_reconnect() {
    WiFi.disconnect();
    WiFi.reconnect();
}

int network_post_data(const char* url, const char* body, const char* content_type) {
    HTTPClient http;
    http.begin(url);
    http.addHeader(F("Content-Type"), content_type);

    Serial.print(F("[HTTP] POST..."));
    Serial.printf(body);
    int httpCode = http.POST(body);
    if (httpCode > 0) {
      Serial.printf("[HTTP] POST... code: %d\n", httpCode);
      if (httpCode == HTTP_CODE_OK) Serial.println(http.getString());
    } else Serial.printf("[HTTP] POST... failed, error: %s\n", http.errorToString(httpCode).c_str());

    http.end();
    return httpCode;
}
