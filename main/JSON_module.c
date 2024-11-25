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
#include "cJSON.h"
#include "JSON_module.h"

bool JSON_ExtractString(const char *json_str, const char *key, char *string, size_t max_len)
{
    if (json_str == NULL || key == NULL || string == NULL || max_len == 0)
    {
        printf("Invalid arguments\n");
        return false;
    }

    // Parse the JSON string
    cJSON *json = cJSON_Parse(json_str);
    if (json == NULL)
    {
        printf("Failed to parse JSON\n");
        return false;
    }

    // Get the value associated with the key
    cJSON *item = cJSON_GetObjectItem(json, key);
    if (!cJSON_IsString(item))
    {
        printf("Invalid or missing key '%s' in JSON\n", key);
        cJSON_Delete(json);
        return false;
    }

    // Copy the value to the output buffer
    strncpy(string, item->valuestring, max_len - 1);
    string[max_len - 1] = '\0'; // Ensure null-termination
    printf("JSON: { %s : %s }\n", key, string);

    // Clean up
    cJSON_Delete(json);
    return true;
}




bool JSON_ExtractInt32(const char *json_str, const char *key, int32_t *value)
{
    if (json_str == NULL || key == NULL || value == NULL)
    {
        printf("Invalid arguments\n");
        return false;
    }

    // Parse the JSON string
    cJSON *json = cJSON_Parse(json_str);
    if (json == NULL)
    {
        printf("Failed to parse JSON\n");
        return false;
    }

    // Get the value associated with the key
    cJSON *item = cJSON_GetObjectItem(json, key);
    if (!cJSON_IsNumber(item))
    {
        printf("Invalid or missing key '%s' in JSON\n", key);
        cJSON_Delete(json);
        return false;
    }

    // Retrieve the value
    *value = (int32_t)item->valueint;
    printf("JSON: { %s : %ld }\n", key, *value);

    // Clean up
    cJSON_Delete(json);
    return true;
}
