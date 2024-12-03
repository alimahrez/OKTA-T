/******************************************************************************
 * @file        BLE_module.h
 * @brief       BLE module header for managing ESP32 Bluetooth Low Energy (BLE) functionality.
 *
 * @author      Ali Mahrez
 * @company     Smart Egat
 * @email       a.mahrez@smart-egat.com
 * @date        Nov 24, 2024
 * @version     Xbeta
 *
 * @details
 * This header file declares the APIs for initializing, configuring, and managing
 * Bluetooth Low Energy (BLE) communication on the ESP32 device. It includes functions
 * for setting up BLE advertising, handling GAP events, and controlling the BLE stack
 * to allow communication with BLE clients. The module leverages the NimBLE stack for
 * BLE functionality and integrates with FreeRTOS for task management.
 ******************************************************************************/

#ifndef BLE_MODULE_H
#define BLE_MODULE_H

// BLE configuration constants
#define BLE_CONFIG_GPIO GPIO_NUM_35 // GPIO pin for BLE configuration button
#define PRESSED_CONFIG_TIME 3000    // Minimum button press time in microseconds (3 seconds)
#define BLE_NAME "OKTA-T"           // BLE device name

// UUIDs for BLE services and characteristics
#define PIN_SERVICE_UUID 0xD4C3
#define PIN_READ_CHARA_UUID 0xD4C2
#define PIN_WRITE_CHARA_UUID 0xD4C1
#define SERVICE_UUID 0xA8F7
#define READ_CHARA_UUID 0xA8F6
#define WRITE_CHARA_UUID 0xA8F5

// Function declarations

/**
 * @brief Initializes BLE advertising.
 *
 * @details
 * This function configures the BLE advertising parameters for the ESP32 device.
 * It sets up the advertising fields (e.g., device name) and starts BLE advertising
 * to allow BLE clients to discover and connect to the ESP32 device.
 */
static void ble_app_advertise(void);

/**
 * @brief Handles GAP (Generic Access Profile) events.
 *
 * @details
 * The `ble_gap_event` function processes GAP events generated during BLE communication,
 * such as connections, disconnections, and advertising completions. It ensures that the
 * BLE device behaves as expected under different scenarios (e.g., re-advertising after disconnection).
 *
 * @param event (struct ble_gap_event *): The event structure containing details about the GAP event.
 * @param arg (void *): A user-defined argument passed to the event handler.
 *
 * @return int: Return value indicating the status of event handling (typically 0 for success).
 */
static int ble_gap_event(struct ble_gap_event *event, void *arg);

/**
 * @brief Called when the BLE stack has been synchronized.
 *
 * @details
 * This callback function is triggered once the BLE stack has been fully synchronized
 * and the BLE device is ready to start advertising or handle connections. It is typically
 * used to begin advertising or perform other initialization tasks once the BLE stack is ready.
 */
void ble_app_on_sync(void);

/**
 * @brief The host task for BLE operations.
 *
 * @details
 * This function runs the FreeRTOS task that drives the NimBLE stack on the ESP32.
 * It is responsible for managing BLE operations such as advertising, connecting,
 * and handling incoming BLE events. This task runs indefinitely and processes
 * BLE events like advertising and connections.
 *
 * @param param (void *): Task parameter passed during task creation.
 */
void host_task(void *param);

/**
 * @brief Configures and starts the BLE server on the ESP32.
 *
 * @details
 * This function sets up the BLE environment on the ESP32, configuring it to act as a BLE
 * server. It performs necessary initializations such as setting the device name, initializing
 * the GAP (Generic Access Profile) and GATT (Generic Attribute Profile) services, and starting
 * the BLE stack. This function prepares the ESP32 to handle BLE advertising and communication
 * with clients.
 */
void connect_ble(void);

/**
 * @brief Monitors a GPIO pin and starts BLE advertising when triggered.
 *
 * @details
 * This function runs in a FreeRTOS task and monitors a specific GPIO pin (likely connected to a button).
 * If the button is pressed for at least the configured threshold time (e.g., 2 seconds), it triggers
 * BLE advertising. It ensures that BLE advertising persists and restarts if necessary.
 *
 * @return void: This function runs indefinitely, polling the GPIO state and handling BLE advertising.
 */
void BLE_Task(void);

#endif // BLE_MODULE_H
