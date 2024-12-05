/******************************************************************************
 * @file        Relay_module.c
 * @brief       Controls relay modules in an IoT system.
 *
 * @author      Eng. Ali Mahrez
 * @company     Smart Egat
 * @email       a.mahrez@smart-egat.com
 * @date        Nov 27, 2024
 * @version     Xbeta
 * @copyright   © 2024 Smart Egat. All rights reserved.
 *
 * @details
 * This module provides functions to initialize and control up to 8 relays connected
 * to the ESP32. It supports setting individual or group relay states and retrieves
 * the last saved states from non-volatile storage.
 ******************************************************************************/

#include <stdio.h>
#include <stdbool.h>
#include "esp_event.h"
#include "nvs_flash.h"
#include "esp_log.h"
#include "driver/gpio.h"
#include "Memory_module.h"
#include "JSON_module.h"
#include "Relay_module.h"

void Relay_Init()
{
    // Define an array of relay pins
    const gpio_num_t relayPins[] = {
        RELAY_1_PIN, RELAY_2_PIN, RELAY_3_PIN, RELAY_4_PIN,
        RELAY_5_PIN, RELAY_6_PIN, RELAY_7_PIN, RELAY_8_PIN};

    // Iterate through the array and set each pin as output
    for (size_t i = 0; i < sizeof(relayPins) / sizeof(relayPins[0]); i++)
    {
        gpio_set_direction(relayPins[i], GPIO_MODE_OUTPUT);
    }
}

void Relay_Set(uint8_t relayNumber, bool State)
{
    // Define an array of relay pins
    const gpio_num_t relayPins[] = {
        RELAY_1_PIN, RELAY_2_PIN, RELAY_3_PIN, RELAY_4_PIN,
        RELAY_5_PIN, RELAY_6_PIN, RELAY_7_PIN, RELAY_8_PIN};

    // Ensure the relay number is within valid range
    if (relayNumber < 1 || relayNumber > sizeof(relayPins) / sizeof(relayPins[0]))
    {
        return; // Invalid relay number
    }

    // Set the GPIO level
    gpio_set_level(relayPins[relayNumber - 1], State ? TURN_ON : TURN_OFF);

    // Construct the storage key dynamically
    char storageKey[4];
    snprintf(storageKey, sizeof(storageKey), "R%u", relayNumber);

    // Save the state to storage
    Memory_SaveInt32("storage", storageKey, State);
}

void Relay_SetGroup(bool State)
{
    // Define an array of relay pins
    const gpio_num_t relayPins[] = {
        RELAY_1_PIN, RELAY_2_PIN, RELAY_3_PIN, RELAY_4_PIN,
        RELAY_5_PIN, RELAY_6_PIN, RELAY_7_PIN, RELAY_8_PIN};

    // Iterate through the relay pins and set the level for each
    for (size_t i = 0; i < sizeof(relayPins) / sizeof(relayPins[0]); i++)
    {
        gpio_set_level(relayPins[i], State ? TURN_ON : TURN_OFF);

        // Dynamically construct the storage key for each relay
        char storageKey[4];
        snprintf(storageKey, sizeof(storageKey), "R%zu", i + 1);

        // Save the state to storage
        Memory_SaveInt32("storage", storageKey, State);
    }
}

void Relay_RetDataState()
{
    const gpio_num_t relayPins[] = {
        RELAY_1_PIN, RELAY_2_PIN, RELAY_3_PIN, RELAY_4_PIN,
        RELAY_5_PIN, RELAY_6_PIN, RELAY_7_PIN, RELAY_8_PIN};

    int32_t state[8];
    char storageKey[4];

    for (size_t i = 0; i < sizeof(relayPins) / sizeof(relayPins[0]); i++)
    {
        // Construct the storage key dynamically
        snprintf(storageKey, sizeof(storageKey), "R%zu", i + 1);

        // Load the relay state from memory
        Memory_LoadInt32("storage", storageKey, &state[i]);

        // Set the relay pin level
        gpio_set_level(relayPins[i], state[i]);
    }
}