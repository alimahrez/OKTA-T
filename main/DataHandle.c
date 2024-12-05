/******************************************************************************
 * @file        DataHandle.c
 * @brief       Handles configuration and operation of a configurable IoT system.
 *
 * @author      Eng. Ali Mahrez
 * @company     Smart Egat
 * @email       a.mahrez@smart-egat.com
 * @date        Nov 27, 2024
 * @version     Xbeta
 * @copyright   © 2024 Smart Egat. All rights reserved.
 *
 * @details
 * This file processes JSON-based runtime configurations for BLE, Wi-Fi, and MQTT.
 * It saves and retrieves configuration data from non-volatile storage and manages
 * operations such as connecting to Wi-Fi, publishing sensor data, and controlling relays
 * based on MQTT messages.
 ******************************************************************************/

#include <stdio.h>
#include <stdbool.h>
#include "esp_log.h"       // Logging
#include "esp_mac.h"       // MAC address handling
#include "driver/gpio.h"   // GPIO control for ESP32
#include "Memory_module.h" // For saving and retrieving configuration data
#include "JSON_module.h"   // For parsing JSON
#include "DataHandle.h"    // Header for this module

static const char *DATA_HANDLE_TAG = "DATA_HANDLE"; // Tag for logging

// Function to process runtime JSON configuration and update the credentialConfig struct
DataErrorHandle GetDataAtRunTime(char *js_string, credentialConfig *config)
{
    // Extract the configuration type from the JSON string
    if (!JSON_ExtractInt32(js_string, "configtype", &config->configType))
    {
        return JS_CONFIG_TYPE_ERROR;
    }

    // Handle configuration based on configType
    switch (config->configType)
    {
    case WIFI_CONFIG_TYPE:
        if (!(
                JSON_ExtractString(js_string, "wifissid", config->wifiSSID, sizeof(config->wifiSSID)) &&
                JSON_ExtractString(js_string, "wifipassword", config->wifiPassword, sizeof(config->wifiPassword))))
        {
            return JS_WIFI_CRD_ERROR;
        }
        // Save Wi-Fi credentials
        Memory_SaveString("storage", "ssid", config->wifiSSID);
        Memory_SaveString("storage", "password", config->wifiPassword);
        break;

    case MQTT_CONFIG_TYPE:
        if (!(
                JSON_ExtractString(js_string, "mqttbroker", config->mqttBroker, sizeof(config->mqttBroker)) &&
                JSON_ExtractInt32(js_string, "mqttport", &config->mqttPort) &&
                JSON_ExtractString(js_string, "mqttusername", config->mqttUsername, sizeof(config->mqttUsername)) &&
                JSON_ExtractString(js_string, "mqttpassword", config->mqttPassword, sizeof(config->mqttPassword))))
        {
            return JS_MQTT_CRD_ERROR;
        }
        // Save MQTT credentials
        Memory_SaveString("storage", "mqttbroker", config->mqttBroker);
        Memory_SaveInt32("storage", "mqttport", config->mqttPort);
        Memory_SaveString("storage", "mqttusername", config->mqttUsername);
        Memory_SaveString("storage", "mqttpassword", config->mqttPassword);
        break;

    case TOPIC_CONFIG_TYPE:
        if (!JSON_ExtractInt32(js_string, "tconfigtype", &config->topicConfigType))
        {
            return JS_TOPIC_CONFIG_ERROR;
        }

        const struct
        {
            int topicType;
            const char *jsonKey;
            char *storage;
            size_t storageSize;
            DataErrorHandle errorCode;
        } topicConfigMap[] = {
            {TOPIC_RELAY_TYPE, "relay_topic", config->relay, sizeof(config->relay), JS_TOPIC_ERROR},
            {TOPIC_TEMP_TYPE, "temp_topic", config->tempSensor, sizeof(config->tempSensor), JS_TOPIC_TEMP_ERROR},
            {TOPIC_LIGHT_TYPE, "light_topic", config->lightSensor, sizeof(config->lightSensor), JS_TOPIC_LIGHT_ERROR},
            {TOPIC_DOOR_TYPE, "door_topic", config->doorSensor, sizeof(config->doorSensor), JS_TOPIC_DOOR_ERROR},
        };

        for (size_t i = 0; i < sizeof(topicConfigMap) / sizeof(topicConfigMap[0]); i++)
        {
            if (topicConfigMap[i].topicType == config->topicConfigType)
            {
                if (!JSON_ExtractString(js_string, topicConfigMap[i].jsonKey, topicConfigMap[i].storage, topicConfigMap[i].storageSize))
                {
                    return topicConfigMap[i].errorCode;
                }
                Memory_SaveString("storage", topicConfigMap[i].jsonKey, topicConfigMap[i].storage);
                break;
            }
        }
        break;

    default:
        return ALL_IS_OK; // Return success for unsupported configType
    }

    return ALL_IS_OK;
}

// Function to display error messages based on error code
void DisplyGetError(DataErrorHandle getError)
{
    // Map of error codes to their corresponding messages
    const struct
    {
        DataErrorHandle errorCode;
        const char *errorMessage;
    } errorMap[] = {
        {JS_CONFIG_TYPE_ERROR, "JS_CONFIG_TYPE_ERROR"},
        {JS_WIFI_CRD_ERROR, "JS_WIFI_CRD_ERROR"},
        {JS_MQTT_CRD_ERROR, "JS_MQTT_CRD_ERROR"},
        {JS_TOPIC_CONFIG_ERROR, "JS_TOPIC_CONFIG_ERROR"},
        {JS_TOPIC_ERROR, "JS_TOPIC_ERROR"},
        {JS_TOPIC_TEMP_ERROR, "JS_TOPIC_TEMP_ERROR"},
        {JS_TOPIC_LIGHT_ERROR, "JS_TOPIC_LIGHT_ERROR"},
        {JS_TOPIC_DOOR_ERROR, "JS_TOPIC_DOOR_ERROR"}};

    // Find and log the error message
    for (size_t i = 0; i < sizeof(errorMap) / sizeof(errorMap[0]); i++)
    {
        if (errorMap[i].errorCode == getError)
        {
            ESP_LOGE(DATA_HANDLE_TAG, "%s", errorMap[i].errorMessage);
            return;
        }
    }

    // Default case: No error or unrecognized error code
    ESP_LOGI(DATA_HANDLE_TAG, "ALL_IS_OK");
}

// Function to retrieve configuration data from non-volatile storage
void RetrieveConfigFromStorage(credentialConfig *config)
{
    // Define a mapping of storage keys to config structure members
    const struct
    {
        const char *storageKey;
        char *configMember;
        size_t memberSize;
    } stringMappings[] = {
        {"ssid", config->wifiSSID, sizeof(config->wifiSSID)},
        {"password", config->wifiPassword, sizeof(config->wifiPassword)},
        {"mqttbroker", config->mqttBroker, sizeof(config->mqttBroker)},
        {"mqttusername", config->mqttUsername, sizeof(config->mqttUsername)},
        {"mqttpassword", config->mqttPassword, sizeof(config->mqttPassword)},
        {"relay_topic", config->relay, sizeof(config->relay)},
        {"temp_topic", config->tempSensor, sizeof(config->tempSensor)},
        {"light_topic", config->lightSensor, sizeof(config->lightSensor)},
        {"door_topic", config->doorSensor, sizeof(config->doorSensor)}};

    // Load all string values into the config structure
    for (size_t i = 0; i < sizeof(stringMappings) / sizeof(stringMappings[0]); i++)
    {
        Memory_LoadString("storage", stringMappings[i].storageKey, stringMappings[i].configMember, stringMappings[i].memberSize);
    }

    // Load integer values separately
    Memory_LoadInt32("storage", "mqttport", &config->mqttPort);
}
