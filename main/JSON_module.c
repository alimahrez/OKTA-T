/**
 * @file        JOSN_module.c
 * @author      Ali Mahrez
 * @date        November 19, 2024
 * @version     1.0
 *
 * @details
 * The C file implements utility functions to parse JSON strings 
 * and extract specific data types (string and integer values). 
 * These functions are designed to simplify the process of retrieving key-value pairs from JSON data in embedded systems, 
 * ensuring robust error handling and efficient memory management.
 */

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>
#include "esp_log.h"
#include "cJSON.h"
#include "JSON_module.h"

static const char *JSON_TAG = "JSON";

bool JSON_ExtractString(const char *json_str, const char *key, char *string, size_t max_len)
{
    if (json_str == NULL || key == NULL || string == NULL || max_len == 0)
    {
        ESP_LOGE(JSON_TAG, "Invalid Arguments");
        return false;
    }

    // Parse the JSON string
    cJSON *json = cJSON_Parse(json_str);
    if (json == NULL)
    {
        ESP_LOGE(JSON_TAG, "Failed to Parse JSON");
        return false;
    }

    // Get the value associated with the key
    cJSON *item = cJSON_GetObjectItem(json, key);
    if (!cJSON_IsString(item))
    {
        ESP_LOGE(JSON_TAG, "Invalid Or Missing Key '%s' In JSON", key);
        cJSON_Delete(json);
        return false;
    }

    // Copy the value to the output buffer
    strncpy(string, item->valuestring, max_len - 1);
    string[max_len - 1] = '\0'; // Ensure null-termination
    ESP_LOGI(JSON_TAG, "{ %s : %s }", key, string);

    // Clean up
    cJSON_Delete(json);
    return true;
}




bool JSON_ExtractInt32(const char *json_str, const char *key, int32_t *value)
{
    if (json_str == NULL || key == NULL || value == NULL)
    {
        ESP_LOGE(JSON_TAG, "Invalid Arguments");
        return false;
    }

    // Parse the JSON string
    cJSON *json = cJSON_Parse(json_str);
    if (json == NULL)
    {
        ESP_LOGE(JSON_TAG, "Failed to Parse JSON");
        return false;
    }

    // Get the value associated with the key
    cJSON *item = cJSON_GetObjectItem(json, key);
    if (!cJSON_IsNumber(item))
    {
        ESP_LOGE(JSON_TAG, "Invalid Or Missing Key '%s' In JSON", key);
        cJSON_Delete(json);
        return false;
    }

    // Retrieve the value
    *value = (int32_t)item->valueint;
    ESP_LOGI(JSON_TAG, "{ %s : %ld }", key, *value);

    // Clean up
    cJSON_Delete(json);
    return true;
}
