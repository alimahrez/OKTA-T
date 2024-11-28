
#ifndef WIFI_MODULE_H
#define WIFI_MODULE_H

void WIFI_Init(char * SSID, char * PASS);
void WIFI_StartConnection();
bool WIFI_IsInternetConnected();

#endif // WIFI_MODULE_H