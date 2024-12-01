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
#include "MQTT_module.c"
#include "JSON_module.h"

credentialConfig getData;

void connectedToBroker()
{
    ESP_LOGI(MQTT_TAG, "Connected to MQTT broker");
    MQTT_Subscribe(getData.relay1);
    MQTT_Subscribe(getData.relay2);
    MQTT_Subscribe(getData.relay3);
    MQTT_Subscribe(getData.relay4);
    MQTT_Subscribe(getData.relay5);
    MQTT_Subscribe(getData.relay6);
    MQTT_Subscribe(getData.relay7);
    MQTT_Subscribe(getData.relay8);
}

void RecivedMsg()
{
    int32_t relayNumber,relayState;

    ESP_LOGI(MQTT_TAG, "recived msg");
    ESP_LOGI(MQTT_TAG, "Received message on topic: %.*s", General_event->topic_len, General_event->topic);
    ESP_LOGI(MQTT_TAG, "Message: %.*s", General_event->data_len, General_event->data);

    JSON_ExtractInt32(General_event->data, "relayNo", &relayNumber);
    JSON_ExtractInt32(General_event->data, "state", &relayState);

    switch (relayNumber)
    {
    case 1:
        if (relayState == 1)
        {
            ESP_LOGI(MQTT_TAG, "Relay No %ld Is Open", relayNumber);
            Relay_Set((uint8_t)relayNumber, (bool)relayState);
        }
        else
        {
            ESP_LOGI(MQTT_TAG, "Relay No %ld Is Close", relayNumber);
            Relay_Set((uint8_t)relayNumber, (bool)relayState);
        }
        
        break;

    case 2:
        if (relayState == 1)
        {
            ESP_LOGI(MQTT_TAG, "Relay No %ld Is Open", relayNumber);
            Relay_Set((uint8_t)relayNumber, (bool)relayState);
        }
        else
        {
            ESP_LOGI(MQTT_TAG, "Relay No %ld Is Close", relayNumber);
            Relay_Set((uint8_t)relayNumber, (bool)relayState);
        }

        break;

    case 3:
        if (relayState == 1)
        {
            ESP_LOGI(MQTT_TAG, "Relay No %ld Is Open", relayNumber);
            Relay_Set((uint8_t)relayNumber, (bool)relayState);
        }
        else
        {
            ESP_LOGI(MQTT_TAG, "Relay No %ld Is Close", relayNumber);
            Relay_Set((uint8_t)relayNumber, (bool)relayState);
        }

        break;

    case 4:
        if (relayState == 1)
        {
            ESP_LOGI(MQTT_TAG, "Relay No %ld Is Open", relayNumber);
            Relay_Set((uint8_t)relayNumber, (bool)relayState);
        }
        else
        {
            ESP_LOGI(MQTT_TAG, "Relay No %ld Is Close", relayNumber);
            Relay_Set((uint8_t)relayNumber, (bool)relayState);
        }

        break;

    case 5:
        if (relayState == 1)
        {
            ESP_LOGI(MQTT_TAG, "Relay No %ld Is Open", relayNumber);
            Relay_Set((uint8_t)relayNumber, (bool)relayState);
        }
        else
        {
            ESP_LOGI(MQTT_TAG, "Relay No %ld Is Close", relayNumber);
            Relay_Set((uint8_t)relayNumber, (bool)relayState);
        }

        break;

    case 6:
        if (relayState == 1)
        {
            ESP_LOGI(MQTT_TAG, "Relay No %ld Is Open", relayNumber);
            Relay_Set((uint8_t)relayNumber, (bool)relayState);
        }
        else
        {
            ESP_LOGI(MQTT_TAG, "Relay No %ld Is Close", relayNumber);
            Relay_Set((uint8_t)relayNumber, (bool)relayState);
        }

        break;

    case 7:
        if (relayState == 1)
        {
            ESP_LOGI(MQTT_TAG, "Relay No %ld Is Open", relayNumber);
            Relay_Set((uint8_t)relayNumber, (bool)relayState);
        }
        else
        {
            ESP_LOGI(MQTT_TAG, "Relay No %ld Is Close", relayNumber);
            Relay_Set((uint8_t)relayNumber, (bool)relayState);
        }

        break;

    case 8:
        if (relayState == 1)
        {
            ESP_LOGI(MQTT_TAG, "Relay No %ld Is Open", relayNumber);
            Relay_Set((uint8_t)relayNumber, (bool)relayState);
        }
        else
        {
            ESP_LOGI(MQTT_TAG, "Relay No %ld Is Close", relayNumber);
            Relay_Set((uint8_t)relayNumber, (bool)relayState);
        }

        break;

    case 16:
            Relay_SetGroup((bool)relayState);
        break;
    default:
        break;
    }
}

void UnsubscribedFromTopic()
{
    ESP_LOGI(MQTT_TAG, "unsubscribed from the topic");
}

void DisconnectedToBroker()
{
    ESP_LOGI(MQTT_TAG, "Disconnected to MQTT broker");
}

void Task_ConfigMode(void *param)
{
    BLE_Task();
}

void app_main()
{
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        // NVS partition was truncated and needs to be erased
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK(err);
    Rleay_Init();
    Relay_RetDataState();
    RetrieveConfigFromStorage(&getData);
    connect_ble();
    WIFI_Init(getData.wifiSSID, getData.wifiPassword);
    xTaskCreate(Task_ConfigMode, "Task_ConfigMode", 2048, NULL, 5, NULL);
    WIFI_StartConnection();
    while (!(WIFI_IsInternetConnected()));
    MQTT_EventConnectedCallback(connectedToBroker);
    MQTT_EventDataActionCallback(RecivedMsg);
    MQTT_EventUnsubscribedCallback(UnsubscribedFromTopic);
    MQTT_EventDisconnectedCallback(DisconnectedToBroker);
    MQTT_Connect(getData.mqttBroker, getData.mqttPort, getData.mqttUsername, getData.mqttPassword);
    
}
