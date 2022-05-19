#ifndef NETWORK_HPP
#define NETWORK_HPP

#define WIFI_SSID "TP-LINK_4449"
#define WIFI_PASSWORD "Pl1sJustWORk45"

void network_init();

const char* network_ssid();
const char* network_ip();
const char* network_mac();

int network_is_connected();
void network_reconnect();

int network_post_data(const char* url, const char* data, const char* content_type);

#endif /* ifndef NETWORK_HPP */
