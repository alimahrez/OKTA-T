/**
 * @file        RELAY_module.c
 * @brief
 * @author      Ali Mahrez
 * @date        November 27, 2024
 * @version     1.0
 *
 * @details

 */

#include <stdio.h>
#include <stdbool.h>
#include "esp_event.h"
#include "nvs_flash.h"
#include "esp_log.h"
#include "driver/gpio.h"
#include "Memory_module.h"
#include "JSON_module.h"
#include "Relay_module.h"



void Rleay_Init()
{
    gpio_set_direction(RELAY_1_PIN, GPIO_MODE_OUTPUT);
    gpio_set_direction(RELAY_2_PIN, GPIO_MODE_OUTPUT);
    gpio_set_direction(RELAY_3_PIN, GPIO_MODE_OUTPUT);
    gpio_set_direction(RELAY_4_PIN, GPIO_MODE_OUTPUT);
    gpio_set_direction(RELAY_5_PIN, GPIO_MODE_OUTPUT);
    gpio_set_direction(RELAY_6_PIN, GPIO_MODE_OUTPUT);
    gpio_set_direction(RELAY_7_PIN, GPIO_MODE_OUTPUT);
    gpio_set_direction(RELAY_8_PIN, GPIO_MODE_OUTPUT);
}

void Relay_Set(uint8_t relayNumber, bool State)
{
    switch (relayNumber)
    {
    case 1:
        (State) ? gpio_set_level(RELAY_1_PIN, TURN_ON) : gpio_set_level(RELAY_1_PIN, TURN_OFF);
        Memory_SaveInt32("storage","R1",State);
        break;
    case 2:
        (State) ? gpio_set_level(RELAY_2_PIN, TURN_ON) : gpio_set_level(RELAY_2_PIN, TURN_OFF);
        Memory_SaveInt32("storage", "R2", State);
        break;
    case 3:
        (State) ? gpio_set_level(RELAY_3_PIN, TURN_ON) : gpio_set_level(RELAY_3_PIN, TURN_OFF);
        Memory_SaveInt32("storage", "R3", State);
        break;
    case 4:
        (State) ? gpio_set_level(RELAY_4_PIN, TURN_ON) : gpio_set_level(RELAY_4_PIN, TURN_OFF);
        Memory_SaveInt32("storage", "R4", State);
        break;
    case 5:
        (State) ? gpio_set_level(RELAY_5_PIN, TURN_ON) : gpio_set_level(RELAY_5_PIN, TURN_OFF);
        Memory_SaveInt32("storage", "R5", State);
        break;
    case 6:
        (State) ? gpio_set_level(RELAY_6_PIN, TURN_ON) : gpio_set_level(RELAY_6_PIN, TURN_OFF);
        Memory_SaveInt32("storage", "R6", State);
        break;
    case 7:
        (State) ? gpio_set_level(RELAY_7_PIN, TURN_ON) : gpio_set_level(RELAY_7_PIN, TURN_OFF);
        Memory_SaveInt32("storage", "R7", State);
        break;
    case 8:
        (State) ? gpio_set_level(RELAY_8_PIN, TURN_ON) : gpio_set_level(RELAY_8_PIN, TURN_OFF);
        Memory_SaveInt32("storage", "R8", State);
        break;
    default:
        break;
    }
}

void Relay_SetGroup(bool State)
{
    switch (State)
    {
    case 1:
        gpio_set_level(RELAY_1_PIN, TURN_ON);
        gpio_set_level(RELAY_2_PIN, TURN_ON);
        gpio_set_level(RELAY_3_PIN, TURN_ON);
        gpio_set_level(RELAY_4_PIN, TURN_ON);
        gpio_set_level(RELAY_5_PIN, TURN_ON);
        gpio_set_level(RELAY_6_PIN, TURN_ON);
        gpio_set_level(RELAY_7_PIN, TURN_ON);
        gpio_set_level(RELAY_8_PIN, TURN_ON);
        Memory_SaveInt32("storage", "R1", State);
        Memory_SaveInt32("storage", "R2", State);
        Memory_SaveInt32("storage", "R3", State);
        Memory_SaveInt32("storage", "R4", State);
        Memory_SaveInt32("storage", "R5", State);
        Memory_SaveInt32("storage", "R6", State);
        Memory_SaveInt32("storage", "R7", State);
        Memory_SaveInt32("storage", "R8", State);
        break;

    case 0:
        gpio_set_level(RELAY_1_PIN, TURN_OFF);
        gpio_set_level(RELAY_2_PIN, TURN_OFF);
        gpio_set_level(RELAY_3_PIN, TURN_OFF);
        gpio_set_level(RELAY_4_PIN, TURN_OFF);
        gpio_set_level(RELAY_5_PIN, TURN_OFF);
        gpio_set_level(RELAY_6_PIN, TURN_OFF);
        gpio_set_level(RELAY_7_PIN, TURN_OFF);
        gpio_set_level(RELAY_8_PIN, TURN_OFF);
        Memory_SaveInt32("storage", "R1", State);
        Memory_SaveInt32("storage", "R2", State);
        Memory_SaveInt32("storage", "R3", State);
        Memory_SaveInt32("storage", "R4", State);
        Memory_SaveInt32("storage", "R5", State);
        Memory_SaveInt32("storage", "R6", State);
        Memory_SaveInt32("storage", "R7", State);
        Memory_SaveInt32("storage", "R8", State);
        break;

    default:
        break;
    }
}


void Relay_RetDataState()
{
    int32_t state [8];
    Memory_LoadInt32("storage", "R1", &state[0]);
    Memory_LoadInt32("storage", "R2", &state[1]);
    Memory_LoadInt32("storage", "R3", &state[2]);
    Memory_LoadInt32("storage", "R4", &state[3]);
    Memory_LoadInt32("storage", "R5", &state[4]);
    Memory_LoadInt32("storage", "R6", &state[5]);
    Memory_LoadInt32("storage", "R7", &state[6]);
    Memory_LoadInt32("storage", "R8", &state[7]);
    gpio_set_level(RELAY_1_PIN, state[0]);
    gpio_set_level(RELAY_2_PIN, state[1]);
    gpio_set_level(RELAY_3_PIN, state[2]);
    gpio_set_level(RELAY_4_PIN, state[3]);
    gpio_set_level(RELAY_5_PIN, state[4]);
    gpio_set_level(RELAY_6_PIN, state[5]);
    gpio_set_level(RELAY_7_PIN, state[6]);
    gpio_set_level(RELAY_8_PIN, state[7]);
}