/******************************************************************************
 * @file        BLE_module.c
 * @brief       BLE (Bluetooth Low Energy) server using ESP32 and the NimBLE stack.
 *
 * @author      Eng. Ali Mahrez
 * @company     Smart Egat
 * @email       a.mahrez@smart-egat.com
 * @date        Nov 24, 2024
 * @version     Xbeta
 * @copyright   © 2024 Smart Egat. All rights reserved.
 *
 * @details
 * This code implements a BLE (Bluetooth Low Energy) server
 * using ESP32 and the NimBLE stack.
 * It defines the BLE characteristics for reading and writing,
 * handles BLE events (e.g., connection, disconnection),
 ******************************************************************************/

#include <stdio.h>
#include <stdbool.h>
#include "esp_event.h"                   // For event handling
#include "nvs_flash.h"                   // For Non-Volatile Storage (NVS)
#include "esp_log.h"                     // For logging
#include "esp_nimble_hci.h"              // For NimBLE HCI (Host Controller Interface)
#include "host/ble_hs.h"                 // For BLE Host stack
#include "nimble/nimble_port.h"          // For NimBLE port definitions
#include "nimble/nimble_port_freertos.h" // For FreeRTOS integration
#include "services/gap/ble_svc_gap.h"    // For Generic Access Profile (GAP)
#include "services/gatt/ble_svc_gatt.h"  // For Generic Attribute Profile (GATT)
#include "esp_mac.h"                     // For MAC address management
#include "driver/gpio.h"                 // For GPIO control
#include "BLE_module.h"                  // Custom BLE module header
#include "Memory_module.h"               // For memory operations
#include "JSON_module.h"                 // For JSON parsing
#include "DataHandle.h"                  // For handling configuration data

static const char *TAG = "BLE-Server"; // Logging tag for the BLE module
uint8_t ble_addr_type;                 // BLE address type
struct ble_gap_adv_params adv_params;  // Advertising parameters for BLE
uint32_t BLE_PASSWORD = 0;             // Password for BLE access (initially 0)
bool status = false;                   // Advertising status flag

// Structure to hold response status
struct responseStatus
{
    bool IsPasswordTrue; // Flag to check if the password is correct
    bool IsJosnOk;       // Flag to check if the JSON configuration is valid
    bool IsChagable;     // Flag to check if the configuration can be changed
    bool IsAccesable;    // Flag to check if access is granted
} resState;

// Function to get the PIN (Not used in this example)
static int BLE_GetPIN(uint16_t conn_handle, uint16_t attr_handle, struct ble_gatt_access_ctxt *ctxt, void *arg)
{
    return 0;
}

// Function to notify the client with PIN data
static int BLE_NotifyPIN(uint16_t con_handle, uint16_t attr_handle, struct ble_gatt_access_ctxt *ctxt, void *arg)
{
    os_mbuf_append(ctxt->om, "Data from the server", strlen("Data from the server")); // Sending data to client
    return 0;
}

// Function to get configuration data from the client
static int BLE_GetConfigData(uint16_t conn_handle, uint16_t attr_handle, struct ble_gatt_access_ctxt *ctxt, void *arg)
{
    credentialConfig configBleData;
    resState.IsPasswordTrue = true; // Assuming the password is correct
    DataErrorHandle getError;

    char *data = (char *)ctxt->om->om_data; // Get the data from the received buffer

    // If password is incorrect, deny access
    if (!resState.IsPasswordTrue)
    {
        resState.IsAccesable = false;
    }
    else
    {
        resState.IsAccesable = true;
        getError = GetDataAtRunTime(data, &configBleData); // Extract and validate configuration data
        DisplyGetError(getError);                          // Display any errors from the data extraction
    }

    memset(data, 0, strlen(data)); // Clear the received data buffer
    return 0;
}

// Function to notify the client with configuration data status
static int BLE_NotifyConfigData(uint16_t con_handle, uint16_t attr_handle, struct ble_gatt_access_ctxt *ctxt, void *arg)
{
    if (!resState.IsAccesable)
    {
        os_mbuf_append(ctxt->om, "Sorry You Cann't Access the Kit", strlen("Sorry You Cann't Access the Kit")); // Notify client that access is denied
    }
    else
    {
        os_mbuf_append(ctxt->om, "Open to access the kit", strlen("Open to access the kit")); // Notify client that access is granted
    }
    return 0;
}

// GATT service definition
static const struct ble_gatt_svc_def gatt_svcs[] = {
    {.type = BLE_GATT_SVC_TYPE_PRIMARY,
     .uuid = BLE_UUID16_DECLARE(PIN_SERVICE_UUID), // UUID for PIN service
     .characteristics = (struct ble_gatt_chr_def[]){
         {.uuid = BLE_UUID16_DECLARE(PIN_READ_CHARA_UUID), // UUID for read characteristic
          .flags = BLE_GATT_CHR_F_READ,
          .access_cb = BLE_NotifyPIN}, // Callback for read

         {.uuid = BLE_UUID16_DECLARE(PIN_WRITE_CHARA_UUID), // UUID for write characteristic
          .flags = BLE_GATT_CHR_F_WRITE | BLE_GATT_CHR_F_NOTIFY,
          .access_cb = BLE_GetPIN}, // Callback for write
         {0}}},

    {.type = BLE_GATT_SVC_TYPE_PRIMARY,
     .uuid = BLE_UUID16_DECLARE(SERVICE_UUID),                                                    // UUID for the main service
     .characteristics = (struct ble_gatt_chr_def[]){{.uuid = BLE_UUID16_DECLARE(READ_CHARA_UUID), // UUID for read characteristic
                                                     .flags = BLE_GATT_CHR_F_READ,
                                                     .access_cb = BLE_NotifyConfigData}, // Callback for read

                                                    {.uuid = BLE_UUID16_DECLARE(WRITE_CHARA_UUID), // UUID for write characteristic
                                                     .flags = BLE_GATT_CHR_F_WRITE | BLE_GATT_CHR_F_NOTIFY,
                                                     .access_cb = BLE_GetConfigData}, // Callback for write
                                                    {0}}},
    {0}}; // End of service definitions

// BLE GAP (Generic Access Profile) event handler
static int ble_gap_event(struct ble_gap_event *event, void *arg)
{
    // Handle different BLE GAP events
    switch (event->type)
    {
    case BLE_GAP_EVENT_CONNECT: // Event for connection
        ESP_LOGI("GAP", "BLE GAP EVENT CONNECT %s", event->connect.status == 0 ? "OK!" : "FAILED!");
        if (event->connect.status != 0)
        {
            ble_app_advertise(); // Restart advertising if connection fails
        }
        break;

    case BLE_GAP_EVENT_DISCONNECT: // Event for disconnection
        ESP_LOGI("GAP", "BLE GAP EVENT DISCONNECTED");
        if (event->connect.status != 0)
        {
            ble_app_advertise(); // Restart advertising after disconnection
        }
        break;

    case BLE_GAP_EVENT_ADV_COMPLETE: // Event when advertising completes
        ESP_LOGI("GAP", "BLE GAP EVENT");
        ble_app_advertise(); // Restart advertising
        break;

    default:
        break;
    }
    return 0;
}

// Function to start BLE advertising
static void ble_app_advertise(void)
{
    struct ble_hs_adv_fields fields;
    const char *device_name;
    memset(&fields, 0, sizeof(fields)); // Initialize advertising fields

    device_name = ble_svc_gap_device_name(); // Get the device name
    fields.name = (uint8_t *)device_name;
    fields.name_len = strlen(device_name);
    fields.name_is_complete = 1; // Set the advertising name

    ble_gap_adv_set_fields(&fields); // Set the advertising fields

    memset(&adv_params, 0, sizeof(adv_params)); // Clear advertising parameters
}

// BLE synchronization callback after BLE stack initialization
void ble_app_on_sync(void)
{
    ble_hs_id_infer_auto(0, &ble_addr_type); // Automatically infer BLE address type
    ble_app_advertise();                     // Start advertising
}

// Host task to run NimBLE stack on FreeRTOS
void host_task(void *param)
{
    nimble_port_run(); // Run the NimBLE host stack
}

// Function to initialize the BLE server and stack
void connect_ble(void)
{
    gpio_set_direction(BLE_CONFIG_GPIO, GPIO_MODE_INPUT); // Set GPIO pin for button input

    nimble_port_init(); // Initialize NimBLE host stack

    ble_svc_gap_device_name_set(BLE_NAME); // Set the BLE device name

    ble_svc_gap_init();  // Initialize GAP service
    ble_svc_gatt_init(); // Initialize GATT service

    ble_gatts_count_cfg(gatt_svcs); // Count GATT services

    ble_gatts_add_svcs(gatt_svcs); // Add GATT services

    ble_hs_cfg.sync_cb = ble_app_on_sync; // Set sync callback for BLE stack

    nimble_port_freertos_init(host_task); // Start FreeRTOS task for BLE stack
}

// BLE task to handle GPIO button press and start advertising
void BLE_Task()
{
    int64_t m = esp_timer_get_time(); // Get the current time in microseconds

    while (1)
    {
        if (!gpio_get_level(BLE_CONFIG_GPIO))
        { // Check if button is pressed
            int64_t n = esp_timer_get_time();
            if ((n - m) / 1000 >= PRESSED_CONFIG_TIME)
            { // Check if button was pressed for long enough
                ESP_LOGI("BOOT BUTTON:", "Button Pressed FOR 3 SECOND\n");

                adv_params.conn_mode = BLE_GAP_CONN_MODE_UND; // Set connectable mode
                adv_params.disc_mode = BLE_GAP_DISC_MODE_GEN; // Set discoverable mode

                ble_gap_adv_start(ble_addr_type, NULL, BLE_HS_FOREVER, &adv_params, ble_gap_event, NULL); // Start advertising
                status = true;                                                                            // Set status to true (advertising started)

                vTaskDelay(100);          // Short delay to debounce button press
                m = esp_timer_get_time(); // Update timestamp
            }
        }
        else
        {
            m = esp_timer_get_time(); // Update timestamp if button is not pressed
        }

        vTaskDelay(10); // Small delay to avoid high CPU usage

        if (status)
        {
            ble_gap_adv_start(ble_addr_type, NULL, BLE_HS_FOREVER, &adv_params, ble_gap_event, NULL); // Continue advertising
        }
    }
}
