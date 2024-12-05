/******************************************************************************
 * @file        main.c
 * @brief       Configurable IoT system using ESP32 with BLE, Wi-Fi, and MQTT.
 *
 * @author      Eng. Ali Mahrez
 * @company     Smart Egat
 * @email       a.mahrez@smart-egat.com
 * @date        Dec 3, 2024
 * @version     Xbeta
 * @copyright   © 2024 Smart Egat. All rights reserved.
 *
 * @details
 * This file contains the main program logic for configuring and operating
 * an IoT system using the ESP32. The system is capable of communicating via
 * BLE for configuration purposes, establishing a Wi-Fi connection for internet
 * access, and interacting with an MQTT broker for message publishing and
 * subscription. The program facilitates the dynamic configuration of Wi-Fi,
 * MQTT, and relay topics via BLE, ensuring that configuration data is persisted
 * in non-volatile memory. The application extracts configuration data from
 * a JSON string and saves it to the device's memory for persistent storage.
 * The ESP32 can also control relay devices based on received MQTT messages
 * and periodically publish sensor data to the broker.
 ******************************************************************************/
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

// Global configuration structure to hold saved settings
credentialConfig getData;

/************************************************************************************************
 * @brief MQTT callback: Called when connected to the broker
 */
void connectedToBroker()
{
    ESP_LOGI(MQTT_TAG, "Connected to MQTT broker");
    MQTT_Subscribe(getData.relay); // Subscribe to the relay topic
}

/************************************************************************************************
 * @brief MQTT callback: Called when a message is received
 */
void RecivedMsg()
{
    int32_t relayNumber, relayState;

    // Log received message details
    ESP_LOGI(MQTT_TAG, "Received message on topic: %.*s", General_event->topic_len, General_event->topic);
    ESP_LOGI(MQTT_TAG, "Message: %.*s", General_event->data_len, General_event->data);

    // Extract relay information from JSON message
    JSON_ExtractInt32(General_event->data, "relayNo", &relayNumber);
    JSON_ExtractInt32(General_event->data, "state", &relayState);

    // Set relay state based on received information
    if (relayNumber >= 1 && relayNumber <= 8)
    {
        Relay_Set((uint8_t)relayNumber, (bool)relayState);
        ESP_LOGI(MQTT_TAG, "Relay %ld is %s", relayNumber, relayState ? "ON" : "OFF");
    }
    else if (relayNumber == 16)
    {
        Relay_SetGroup((bool)relayState);
        ESP_LOGI(MQTT_TAG, "All relays set to %s", relayState ? "ON" : "OFF");
    }
    else
    {
        ESP_LOGW(MQTT_TAG, "Invalid relay number: %ld", relayNumber);
    }
}

/************************************************************************************************
 * @brief MQTT callback: Called when unsubscribed from a topic
 */
void UnsubscribedFromTopic()
{
    ESP_LOGI(MQTT_TAG, "Unsubscribed from the topic");
}

/************************************************************************************************
 * @brief MQTT callback: Called when disconnected from the broker
 */
void DisconnectedToBroker()
{
    ESP_LOGI(MQTT_TAG, "Disconnected from MQTT broker");
}

/************************************************************************************************
 * @brief BLE task for configuration mode
 * @param param Task parameter (unused)
 */
void Task_ConfigMode(void *param)
{
    BLE_Task(); // Handle BLE configuration in this task
}

/************************************************************************************************
 * @brief MQTT task for Publish topics for the broker
 * @param param Task parameter (unused)
 */
void Task_MQTTPublish(void *param)
{
    while (1)
    {
        MQTT_Publish(getData.tempSensor, "Temp = ", 10);
        MQTT_Publish(getData.lightSensor,"Light = ", 5);
        MQTT_Publish(getData.doorSensor, "Door State = ", 60);
    }
}
/************************************************************************************************
 * @brief Application entry point
 */
void app_main()
{
    // Initialize NVS for storing configuration data
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK(err);

    // Initialize relays and retrieve saved states
    Relay_Init();
    Relay_RetDataState();

    // Retrieve configuration from non-volatile storage
    RetrieveConfigFromStorage(&getData);

    // Initialize BLE for configuration
    connect_ble();

    // Initialize Wi-Fi with retrieved credentials
    WIFI_Init(getData.wifiSSID, getData.wifiPassword);

    // Start BLE configuration mode task
    xTaskCreate(Task_ConfigMode, "Task_ConfigMode", 2048, NULL, 5, NULL);

    // Start Wi-Fi connection and wait until connected
    WIFI_StartConnection();
    while (!WIFI_IsInternetConnected())
        ;

    // Register MQTT event callbacks
    MQTT_EventConnectedCallback(connectedToBroker);
    MQTT_EventDataActionCallback(RecivedMsg);
    MQTT_EventUnsubscribedCallback(UnsubscribedFromTopic);
    MQTT_EventDisconnectedCallback(DisconnectedToBroker);

    // Connect to MQTT broker with retrieved credentials
    MQTT_Connect(getData.mqttBroker, getData.mqttPort, getData.mqttUsername, getData.mqttPassword);

    // Start BLE configuration mode task
    xTaskCreate(Task_MQTTPublish, "Task_MQTTPublish", 2048, NULL, 5, NULL);
}
