#include <stdio.h>
#include <stdbool.h>
#include "esp_log.h"
#include "esp_wifi.h"
#include "esp_http_client.h"
#include "esp_http_server.h"
#include "DataHandle.h"
#include "WIFI_module.h"

// static const char *WIFI_TAG = "WIFI CONN";
static const char *INTERNET_TAG = "NET-CONN";

void WIFI_Init(char * SSID, char * PASS)
{
    esp_netif_init();
    esp_event_loop_create_default();
    esp_netif_create_default_wifi_sta();

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    wifi_config_t wifi_config;
    memset(&wifi_config, 0, sizeof(wifi_config_t));
    strncpy((char *)wifi_config.sta.ssid, SSID, sizeof(wifi_config.sta.ssid) - 1);
    strncpy((char *)wifi_config.sta.password, PASS, sizeof(wifi_config.sta.password) - 1);
    wifi_config.sta.threshold.authmode = WIFI_AUTH_WPA2_PSK;

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_config));
}


void WIFI_StartConnection()
{
    ESP_ERROR_CHECK(esp_wifi_start());
    ESP_ERROR_CHECK(esp_wifi_connect());
}


bool WIFI_IsInternetConnected()
{
    esp_http_client_config_t config = {
        .url = "http://www.google.com", // Testing connection to Google
    };
    esp_http_client_handle_t client = esp_http_client_init(&config);

    esp_err_t err = esp_http_client_perform(client);
    esp_http_client_cleanup(client);

    if (err == ESP_OK)
    {
        ESP_LOGI(INTERNET_TAG,"Connected to the internet");
        return true;
    }
    else
    {
        ESP_LOGW(INTERNET_TAG,"Not connected to the internet");
        return false;
    }
}