#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "nvs_flash.h"
#include "esp_log.h"
#include "BLE_module.h"
#include "Memory_module.h"
#include "DataHandle.h"
#include "Relay_module.h"
#include "WIFI_module.h"

void Task_ConfigMode(void *param)
{
    BLE_Task();
}

void app_main()
{
    credentialConfig getData;


    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        // NVS partition was truncated and needs to be erased
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK(err);

    RetrieveConfigFromStorage(&getData);

    // printf("%s\n", getData.wifiSSID);
    // printf("%s\n", getData.wifiPassword);

    // printf("%s\n", getData.mqttBroker);
    // printf("%ld\n", getData.mqttPort);
    // printf("%s\n", getData.mqttUsername);
    // printf("%s\n", getData.mqttPassword);

    // printf("%s\n", getData.relay1);
    // printf("%s\n", getData.relay2);
    // printf("%s\n", getData.relay3);
    // printf("%s\n", getData.relay4);
    // printf("%s\n", getData.relay5);
    // printf("%s\n", getData.relay6);
    // printf("%s\n", getData.relay7);
    // printf("%s\n", getData.relay8);
    // printf("%s\n", getData.tempSensor);
    // printf("%s\n", getData.lightSensor);
    // printf("%s\n", getData.doorSensor);

    connect_ble();
    xTaskCreate(Task_ConfigMode, "Task_ConfigMode", 2048, NULL, 5, NULL);

    // WIFI_Init(getData.wifiSSID, getData.wifiPassword);
    // WIFI_StartConnection();
    // while (!(WIFI_IsInternetConnected()));
    
}
