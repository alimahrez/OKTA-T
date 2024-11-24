#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "nvs_flash.h"
#include "esp_log.h"
#include "BLE_module.h"
#include "Memory_module.h"

void Task_ConfigMode(void *param)
{
    BLE_Task();
}

void app_main()
{
    char name[32]={0};
    int32_t pass;
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        // NVS partition was truncated and needs to be erased
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK(err);

    Memory_LoadString("storage", "name", name, sizeof(name));
    Memory_LoadInt32("storage","ble_pass",&pass);
    // Memory_SaveString("storage","name","Ali Mahrez");
    // Memory_SaveInt32("storage", "ble_pass",1234);

    printf("%s\n", name);
    printf("%ld\n",pass);

    connect_ble();
    xTaskCreate(Task_ConfigMode, "Task_ConfigMode", 2048, NULL, 5, NULL);
}
