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
        (State) ? gpio_set_level(RELAY_1_PIN, TURN_ON) : gpio_set_level(RELAY_1_PIN, TURN_OFF); break;
    case 2:
        (State) ? gpio_set_level(RELAY_2_PIN, TURN_ON): gpio_set_level(RELAY_2_PIN, TURN_OFF); break;
    case 3:
        (State) ? gpio_set_level(RELAY_3_PIN, TURN_ON) : gpio_set_level(RELAY_3_PIN, TURN_OFF);break;
    case 4:
        (State) ? gpio_set_level(RELAY_4_PIN, TURN_ON) : gpio_set_level(RELAY_4_PIN, TURN_OFF); break;
    case 5:
        (State) ? gpio_set_level(RELAY_5_PIN, TURN_ON): gpio_set_level(RELAY_5_PIN, TURN_OFF); break;
    case 6:
        (State) ? gpio_set_level(RELAY_6_PIN, TURN_ON) : gpio_set_level(RELAY_6_PIN, TURN_OFF); break;
    case 7:
        (State) ? gpio_set_level(RELAY_7_PIN, TURN_ON) : gpio_set_level(RELAY_7_PIN, TURN_OFF); break;
    case 8:
        (State) ? gpio_set_level(RELAY_8_PIN, TURN_ON) : gpio_set_level(RELAY_8_PIN, TURN_OFF);break;
    default:
        break;
    }

}