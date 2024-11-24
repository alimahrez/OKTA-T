/**
 * @file        Memory_module.c
 * @author      Ali Mahrez
 * @date        November 19, 2024
 * @version     1.0
 *
 * @details
 * This C file provides utility functions for saving and retrieving data in the Non-Volatile Storage (NVS) of an ESP32. 
 * Specifically, the file includes functions to handle int32_t values, 
 * allowing you to persist integers across device reboots or power cycles. 
 * These functions simplify interactions with the NVS by abstracting low-level API calls and providing clear error handling.
 */

#include <stdio.h>     // For printf and basic input/output functions
#include <stdint.h>    // For standard integer types like int32_t
#include "esp_err.h"   // For ESP32 error codes and error handling
#include "nvs_flash.h" // For initializing and managing the NVS subsystem
#include "nvs.h"       // For working with NVS handles and API functions
#include "Memory_module.h"

void Memory_SaveString(const char * nameSpace, const char * key,const char *string)
{

    esp_err_t err;
    nvs_handle_t Store_Handle;

    // Open the NVS storage with the specified namespace in read/write mode
    err = nvs_open(nameSpace, NVS_READWRITE, &Store_Handle);
    if (err != ESP_OK)
    {
        // Log error if the namespace cannot be opened
        printf("Error (%s) opening NVS handle!\n", esp_err_to_name(err));
        return;
    }
    
    else
    {
        printf("Saving (%s) credentials...\n", string);
        // Write the string to NVS using the specified key
        err = nvs_set_str(Store_Handle, key, string);
        if (err != ESP_OK)
        {
            printf("Failed to write (%s) to NVS!\n", string);
        }

        // Commit changes to NVS to ensure data is stored
        err = nvs_commit(Store_Handle);
        if (err != ESP_OK)
        {
            printf("Failed to commit changes!\n");
        }
        printf("String saved.\n");

        // Close the NVS handle to free resources
        nvs_close(Store_Handle);
    }
}


void Memory_LoadString(const char *nameSpace, const char * key , char *stringOut, size_t stringSize)
{
    nvs_handle_t Ret_handle;
    esp_err_t err = nvs_open(nameSpace, NVS_READONLY, &Ret_handle);

    // Open the NVS storage with the specified namespace in read-only mode
    if (err != ESP_OK)
    {
        // Log error if the namespace cannot be opened
        printf("Error (%s) opening NVS handle!\n", esp_err_to_name(err));
        return;
    }

    else
    {
        printf("Loading (%s) data...\n", key);
        // Retrieve the string from NVS using the specified key
        err = nvs_get_str(Ret_handle, key, stringOut, &stringSize);
        if (err != ESP_OK)
        {
            printf("Failed to read (%s) from NVS!\n", key);
        }
        // Close the NVS handle to free resources
        nvs_close(Ret_handle);
    }
}


void Memory_SaveInt32(const char *nameSpace, const char *key, int32_t value)
{
    esp_err_t err;
    nvs_handle_t Store_Handle;

    // Open the NVS storage with the specified namespace in read/write mode
    err = nvs_open(nameSpace, NVS_READWRITE, &Store_Handle);
    if (err != ESP_OK)
    {
        // Log error if the namespace cannot be opened
        printf("Error (%s) opening NVS handle!\n", esp_err_to_name(err));
        return;
    }
    else
    {
        // Log saving process
        printf("Saving integer (%ld) under key (%s)...\n", value, key);

        // Write the int32_t value to NVS using the specified key
        err = nvs_set_i32(Store_Handle, key, value);
        if (err != ESP_OK)
        {
            // Log error if writing to NVS fails
            printf("Failed to write (%ld) to NVS under key (%s)!\n", value, key);
        }

        // Commit changes to NVS to ensure data is stored
        err = nvs_commit(Store_Handle);
        if (err != ESP_OK)
        {
            // Log error if committing changes fails
            printf("Failed to commit changes!\n");
        }
        else
        {
            // Log success message
            printf("Integer saved.\n");
        }

        // Close the NVS handle to free resources
        nvs_close(Store_Handle);
    }
}


void Memory_LoadInt32(const char *nameSpace, const char *key, int32_t *valueOut)
{
    nvs_handle_t Ret_handle;
    esp_err_t err;

    // Open the NVS storage with the specified namespace in read-only mode
    err = nvs_open(nameSpace, NVS_READONLY, &Ret_handle);
    if (err != ESP_OK)
    {
        // Log error if the namespace cannot be opened
        printf("Error (%s) opening NVS handle!\n", esp_err_to_name(err));
        return;
    }
    else
    {
        // Log loading process
        printf("Loading integer data for key (%s)...\n", key);

        // Retrieve the int32_t value from NVS using the specified key
        err = nvs_get_i32(Ret_handle, key, valueOut);
        if (err != ESP_OK)
        {
            // Log error if reading from NVS fails
            printf("Failed to read integer value for key (%s) from NVS!\n", key);
        }
        else
        {
            // Log success message
            printf("Integer loaded: %ld\n", *valueOut);
        }

        // Close the NVS handle to free resources
        nvs_close(Ret_handle);
    }
}
