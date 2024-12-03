/******************************************************************************
 * @file        RELAY_module.h
 * @brief       Relay module header for controlling GPIO pins connected to relays.
 *
 * @author      Ali Mahrez
 * @company     Smart Egat
 * @email       a.mahrez@smart-egat.com
 * @date        Nov 27, 2024
 * @version     Xbeta
 *
 * @details
 * This header file declares the APIs for controlling relay states using GPIO pins.
 * It provides functions for initializing the GPIO pins, controlling individual and
 * grouped relay states, and restoring relay states from non-volatile storage.
 * This module is intended for use with the ESP32 platform.
 *
 * @copyright
 * © 2024 Smart Egat. All rights reserved.
 ******************************************************************************/
#ifndef RELAY_MODULE_H
#define RELAY_MODULE_H

#define RELAY_1_PIN         GPIO_NUM_19
#define RELAY_2_PIN         GPIO_NUM_18
#define RELAY_3_PIN         GPIO_NUM_2
#define RELAY_4_PIN         GPIO_NUM_27
#define RELAY_5_PIN         GPIO_NUM_26
#define RELAY_6_PIN         GPIO_NUM_25
#define RELAY_7_PIN         GPIO_NUM_33
#define RELAY_8_PIN         GPIO_NUM_32

#define TURN_ON             1
#define TURN_OFF            0

/**
 * @brief Initializes the GPIO pins for relay control.
 *
 * @details
 * Configures the GPIO pins connected to relays as output pins. This function
 * should be called during the system initialization to prepare the relays for
 * control operations.
 */
void Relay_Init();

/**
 * @brief Sets the state of a specific relay.
 *
 * @param relayNumber (uint8_t): The relay number (1-8) to control.
 * @param State (bool): The desired state of the relay.
 * - `true` to turn the relay ON.
 * - `false` to turn the relay OFF.
 *
 * @details
 * Changes the state of the specified relay and saves the new state to
 * non-volatile storage for persistence across system reboots.
 */
void Relay_Set(uint8_t relayNumber, bool State);

/**
 * @brief Sets the state of all relays as a group.
 *
 * @param State (bool): The desired state for all relays.
 * - `true` to turn all relays ON.
 * - `false` to turn all relays OFF.
 *
 * @details
 * Controls the state of all relays simultaneously and saves the group
 * state to non-volatile storage.
 */
void Relay_SetGroup(bool State);

/**
 * @brief Restores the state of all relays from non-volatile storage.
 *
 * @details
 * Retrieves the saved state of each relay from non-volatile storage and
 * applies the states to the corresponding GPIO pins. This function ensures
 * that the relay states are consistent after a system restart.
 */
void Relay_RetDataState();

#endif // RELAY_MODULE_H
