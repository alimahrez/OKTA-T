#include <stdio.h>
#include <stdbool.h>
#include "esp_log.h"
#include "esp_mac.h"
#include "driver/gpio.h"
#include "Memory_module.h"
#include "JSON_module.h"
#include "DataHandle.h"
static const char *DATA_HANDLE_TAG = "DATA_HANDLE";

DataErrorHandle GetDataAtRunTime(char *js_string, credentialConfig *config)
{
    if (!(JSON_ExtractInt32(js_string, "configtype", &config->configType)))
    {
        return JS_CONFIG_TYPE_ERROR;
    }
    switch (config->configType)
    {

    case WIFI_CONFIG_TYPE:
        if (!(

                JSON_ExtractString(js_string, "wifissid", config->wifiSSID, sizeof(config->wifiSSID)) &&
                JSON_ExtractString(js_string, "wifipassword", config->wifiPassword, sizeof(config->wifiPassword))))
        {
            return JS_WIFI_CRD_ERROR;
        }
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
        Memory_SaveString("storage", "mqttbroker", config->mqttBroker);
        Memory_SaveInt32("storage", "mqttport", config->mqttPort);
        Memory_SaveString("storage", "mqttusername", config->mqttUsername);
        Memory_SaveString("storage", "mqttpassword", config->mqttPassword);
        break;

    case TOPIC_CONFIG_TYPE:
        if (!(JSON_ExtractInt32(js_string, "tconfigtype", &config->topicConfigType)))
        {
            return JS_TOPIC_CONFIG_ERROR;
        }
        switch (config->topicConfigType)
        {
        case TOPIC_RELAY_TYPE_1:
            if (!(JSON_ExtractString(js_string, "relay1_topic", config->relay1, sizeof(config->relay1))))
            {
                return JS_TOPIC_1_ERROR;
            }
            Memory_SaveString("storage", "relay1_topic", config->relay1);
            break;

        case TOPIC_RELAY_TYPE_2:
            if (!(JSON_ExtractString(js_string, "relay2_topic", config->relay2, sizeof(config->relay2))))
            {
                return JS_TOPIC_2_ERROR;
            }
            Memory_SaveString("storage", "relay2_topic", config->relay2);
            break;

        case TOPIC_RELAY_TYPE_3:
            if (!(JSON_ExtractString(js_string, "relay3_topic", config->relay3, sizeof(config->relay3))))
            {
                return JS_TOPIC_3_ERROR;
            }
            Memory_SaveString("storage", "relay3_topic", config->relay3);
            break;

        case TOPIC_RELAY_TYPE_4:
            if (!(JSON_ExtractString(js_string, "relay4_topic", config->relay4, sizeof(config->relay4))))
            {
                return JS_TOPIC_4_ERROR;
            }
            Memory_SaveString("storage", "relay4_topic", config->relay4);
            break;

        case TOPIC_RELAY_TYPE_5:
            if (!(JSON_ExtractString(js_string, "relay5_topic", config->relay5, sizeof(config->relay5))))
            {
                return JS_TOPIC_5_ERROR;
            }
            Memory_SaveString("storage", "relay5_topic", config->relay5);
            break;

        case TOPIC_RELAY_TYPE_6:
            if (!(JSON_ExtractString(js_string, "relay6_topic", config->relay6, sizeof(config->relay6))))
            {
                return JS_TOPIC_6_ERROR;
            }
            Memory_SaveString("storage", "relay6_topic", config->relay6);
            break;

        case TOPIC_RELAY_TYPE_7:
            if (!(JSON_ExtractString(js_string, "relay7_topic", config->relay7, sizeof(config->relay7))))
            {
                return JS_TOPIC_7_ERROR;
            }
            Memory_SaveString("storage", "relay7_topic", config->relay7);
            break;

        case TOPIC_RELAY_TYPE_8:
            if (!(JSON_ExtractString(js_string, "relay8_topic", config->relay8, sizeof(config->relay8))))
            {
                return JS_TOPIC_8_ERROR;
            }
            Memory_SaveString("storage", "relay8_topic", config->relay8);
            break;

        case TOPIC_TEMP_TYPE:
            if (!(JSON_ExtractString(js_string, "temp_topic", config->tempSensor, sizeof(config->tempSensor))))
            {
                return JS_TOPIC_TEMP_ERROR;
            }
            Memory_SaveString("storage", "temp_topic", config->tempSensor);
            break;

        case TOPIC_LIGHT_TYPE:
            if (!(JSON_ExtractString(js_string, "light_topic", config->lightSensor, sizeof(config->lightSensor))))
            {
                return JS_TOPIC_LIGHT_ERROR;
            }
            Memory_SaveString("storage", "light_topic", config->lightSensor);
            break;

        case TOPIC_DOOR_TYPE:
            if (!(JSON_ExtractString(js_string, "door_topic", config->doorSensor, sizeof(config->doorSensor))))
            {
                return JS_TOPIC_DOOR_ERROR;
            }
            Memory_SaveString("storage", "door_topic", config->doorSensor);
            break;

        default:
            break;
        }

        /* code */
        break;

    default:
        break;
    }

    return ALL_IS_OK;
}

void DisplyGetError(DataErrorHandle getError)
{
    switch (getError)
    {
    case JS_CONFIG_TYPE_ERROR:
        ESP_LOGE(DATA_HANDLE_TAG, "JS_CONFIG_TYPE_ERROR");

        break;
    case JS_WIFI_CRD_ERROR:
        ESP_LOGE(DATA_HANDLE_TAG, "JS_WIFI_CRD_ERROR");

        break;
    case JS_MQTT_CRD_ERROR:
        ESP_LOGE(DATA_HANDLE_TAG, "JS_MQTT_CRD_ERROR");

        break;
    case JS_TOPIC_CONFIG_ERROR:
        ESP_LOGE(DATA_HANDLE_TAG, "JS_TOPIC_CONFIG_ERROR");

        break;
    case JS_TOPIC_1_ERROR:
        ESP_LOGE(DATA_HANDLE_TAG, "JS_TOPIC_1_ERROR");

        break;
    case JS_TOPIC_2_ERROR:
        ESP_LOGE(DATA_HANDLE_TAG, "JS_TOPIC_2_ERROR");

        break;
    case JS_TOPIC_3_ERROR:
        ESP_LOGE(DATA_HANDLE_TAG, "JS_TOPIC_3_ERROR");

        break;
    case JS_TOPIC_4_ERROR:
        ESP_LOGE(DATA_HANDLE_TAG, "JS_TOPIC_4_ERROR");

        break;
    case JS_TOPIC_5_ERROR:
        ESP_LOGE(DATA_HANDLE_TAG, "JS_TOPIC_5_ERROR");

        break;
    case JS_TOPIC_6_ERROR:
        ESP_LOGE(DATA_HANDLE_TAG, "JS_TOPIC_6_ERROR");

        break;
    case JS_TOPIC_7_ERROR:
        ESP_LOGE(DATA_HANDLE_TAG, "JS_TOPIC_7_ERROR");

        break;
    case JS_TOPIC_8_ERROR:
        ESP_LOGE(DATA_HANDLE_TAG, "JS_TOPIC_8_ERROR");

        break;
    case JS_TOPIC_TEMP_ERROR:
        ESP_LOGE(DATA_HANDLE_TAG, "JS_TOPIC_TEMP_ERROR");

        break;
    case JS_TOPIC_LIGHT_ERROR:
        ESP_LOGE(DATA_HANDLE_TAG, "JS_TOPIC_LIGHT_ERROR");

        break;
    case JS_TOPIC_DOOR_ERROR:
        ESP_LOGE(DATA_HANDLE_TAG, "JS_TOPIC_DOOR_ERROR");

        break;

    default:
        ESP_LOGI(DATA_HANDLE_TAG, "ALL_IS_OK");
        break;
    }
}

void RetrieveConfigFromStorage(credentialConfig *config)
{
    Memory_LoadInt32("storage", "mqttport", &config->mqttPort);
    Memory_LoadString("storage", "ssid", config->wifiSSID, sizeof(config->wifiSSID));
    Memory_LoadString("storage", "password", config->wifiPassword, sizeof(config->wifiPassword));
    Memory_LoadString("storage", "mqttbroker", config->mqttBroker, sizeof(config->mqttBroker));
    Memory_LoadString("storage", "mqttusername", config->mqttUsername, sizeof(config->mqttUsername));
    Memory_LoadString("storage", "mqttpassword", config->mqttPassword, sizeof(config->mqttPassword));
    Memory_LoadString("storage", "relay1_topic", config->relay1, sizeof(config->relay1));
    Memory_LoadString("storage", "relay2_topic", config->relay2, sizeof(config->relay2));
    Memory_LoadString("storage", "relay3_topic", config->relay3, sizeof(config->relay3));
    Memory_LoadString("storage", "relay4_topic", config->relay4, sizeof(config->relay4));
    Memory_LoadString("storage", "relay5_topic", config->relay5, sizeof(config->relay5));
    Memory_LoadString("storage", "relay6_topic", config->relay6, sizeof(config->relay6));
    Memory_LoadString("storage", "relay7_topic", config->relay7, sizeof(config->relay7));
    Memory_LoadString("storage", "relay8_topic", config->relay8, sizeof(config->relay8));
    Memory_LoadString("storage", "temp_topic", config->tempSensor, sizeof(config->tempSensor));
    Memory_LoadString("storage", "light_topic", config->lightSensor, sizeof(config->lightSensor));
    Memory_LoadString("storage", "door_topic", config->doorSensor, sizeof(config->doorSensor));
}
