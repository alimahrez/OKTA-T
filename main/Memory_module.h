/******************************************************************************
 * @file        MEMORY_module.h
 * @brief       Memory module header for managing ESP32 Non-Volatile Storage (NVS).
 *
 * @author      Ali Mahrez
 * @company     Smart Egat
 * @email       a.mahrez@smart-egat.com
 * @date        Nov 24, 2024
 * @version     Xbeta
 *
 * @details
 * This header file declares the APIs for saving and loading data to and from
 * the Non-Volatile Storage (NVS) of the ESP32. It provides functions to store
 * and retrieve both string and integer data, enabling persistent storage across
 * device reboots or power cycles.
 ******************************************************************************/

#ifndef MEMORY_MODULE_H
#define MEMORY_MODULE_H

/**
 * @brief Saves a text to the NVS (Non-Volatile Storage) under a given namespace and key.
 *
 * @param nameSpace The namespace under which the data will be stored.
 * @param key The key associated with the string to save.
 * @param string The string to save to NVS.
 */
void Memory_SaveString(const char * nameSpace, const char * key,const char *string);

/**
 * @brief Loads a string from the NVS (Non-Volatile Storage) using a given namespace and key.
 *
 * @param nameSpace The namespace under which the data is stored.
 * @param key The key associated with the string to retrieve.
 * @param stringOut The output buffer to store the retrieved string.
 * @param stringSize The size of the output buffer.
 */
void Memory_LoadString(const char *nameSpace, const char * key , char *stringOut, size_t stringSize);

/**
 * @brief Saves an int32_t value to the NVS (Non-Volatile Storage) under a given namespace and key.
 *
 * @param nameSpace The namespace under which the data will be stored.
 * @param key The key associated with the integer to save.
 * @param value The int32_t value to save to NVS.
 */
void Memory_SaveInt32(const char *nameSpace, const char *key, int32_t value);

/**
 * @brief Loads an int32_t value from the NVS (Non-Volatile Storage) using a given namespace and key.
 *
 * @param nameSpace The namespace under which the data is stored.
 * @param key The key associated with the integer to retrieve.
 * @param valueOut Pointer to an int32_t variable where the retrieved value will be stored.
 */
void Memory_LoadInt32(const char *nameSpace, const char *key, int32_t *valueOut);




#endif // MEMORY_MODULE_H
