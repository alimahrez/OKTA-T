/**
 * @file        BLE_module.c
 * @brief       BLE (Bluetooth Low Energy) server using ESP32 and the NimBLE stack.
 * @author      Ali Mahrez
 * @date        November 19, 2024
 * @version     1.0
 *
 * @details
 * This code implements a BLE (Bluetooth Low Energy) server
 * using ESP32 and the NimBLE stack.
 * It defines the BLE characteristics for reading and writing,
 * handles BLE events (e.g., connection, disconnection),
 */

#include <stdio.h>
#include "esp_event.h"
#include "nvs_flash.h"
#include "esp_log.h"
#include "esp_nimble_hci.h"
#include "host/ble_hs.h"
#include "nimble/nimble_port.h"
#include "nimble/nimble_port_freertos.h"
#include "services/gap/ble_svc_gap.h"
#include "services/gatt/ble_svc_gatt.h"
#include "esp_mac.h"
#include "driver/gpio.h"
#include "BLE_module.h"


char *TAG = "BLE-Server";
uint8_t ble_addr_type;

struct ble_gap_adv_params adv_params;
bool status = false;

static int device_write(uint16_t conn_handle, uint16_t attr_handle, struct ble_gatt_access_ctxt *ctxt, void *arg)
{
    // printf("Data from the client: %.*s\n", ctxt->om->om_len, ctxt->om->om_data);

    char *data = (char *)ctxt->om->om_data;
    printf("%d %s\n", strcmp(data, (char *)"LIGHT ON") == 0, data);
    if (strcmp(data, (char *)"LIGHT ON\0") == 0)
    {
        printf("LIGHT ON\n");
        gpio_set_level(GPIO_NUM_2, 1);
    }
    else if (strcmp(data, (char *)"LIGHT OFF\0") == 0)
    {
        printf("LIGHT OFF\n");
        gpio_set_level(GPIO_NUM_2, 0);
    }
    else if (strcmp(data, (char *)"LED ON\0") == 0)
    {
        printf("LED ON\n");
        gpio_set_level(GPIO_NUM_4, 1);
    }
    else if (strcmp(data, (char *)"LED OFF\0") == 0)
    {
        printf("LED OFF\n");
        gpio_set_level(GPIO_NUM_4, 0);
    }
    // else
    // {
    //     printf("Data from the client: %.*s\n", ctxt->om->om_len, ctxt->om->om_data);
    // }
    memset(data, 0, strlen(data));

    return 0;
}

static int device_read(uint16_t con_handle, uint16_t attr_handle, struct ble_gatt_access_ctxt *ctxt, void *arg)
{
    os_mbuf_append(ctxt->om, "Data from the server", strlen("Data from the server"));
    return 0;
}

static int device_read2(uint16_t con_handle, uint16_t attr_handle, struct ble_gatt_access_ctxt *ctxt, void *arg)
{
    os_mbuf_append(ctxt->om, "Message from Innovate Yourself", strlen("Message from Innovate Yourself"));
    return 0;
}

static const struct ble_gatt_svc_def gatt_svcs[] = {
    {.type = BLE_GATT_SVC_TYPE_PRIMARY,
     .uuid = BLE_UUID16_DECLARE(0x180), // Define UUID for device type
     .characteristics = (struct ble_gatt_chr_def[]){
         {.uuid = BLE_UUID16_DECLARE(0xFEF4), // Define UUID for reading
          .flags = BLE_GATT_CHR_F_READ,
          .access_cb = device_read},
         {.uuid = BLE_UUID16_DECLARE(0xDEAD), // Define UUID for writing
          .flags = BLE_GATT_CHR_F_WRITE,
          .access_cb = device_write},
         {.uuid = BLE_UUID16_DECLARE(0xDEAD), // Define UUID for writing
          .flags = BLE_GATT_CHR_F_READ,
          .access_cb = device_read2},
         {0}}},
    {0}};

static int ble_gap_event(struct ble_gap_event *event, void *arg)
{
    // Handle different BLE GAP (Generic Access Profile) events
    switch (event->type)
    {
    
    // Case when a connection event occurs ----------------------------------------------------------
    case BLE_GAP_EVENT_CONNECT:
        ESP_LOGI("GAP", "BLE GAP EVENT CONNECT %s", event->connect.status == 0 ? "OK!" : "FAILED!");
        // If the connection attempt fails, start advertising again
        if (event->connect.status != 0)
        {
            ble_app_advertise();
        }
        break;

    // Case when a disconnection event occurs -------------------------------------------------------
    case BLE_GAP_EVENT_DISCONNECT:
        ESP_LOGI("GAP", "BLE GAP EVENT DISCONNECTED");
        // Start advertising again after disconnection
        if (event->connect.status != 0)
        {
            ble_app_advertise();
        }
        break;

    // Case when advertising completes --------------------------------------------------------------
    case BLE_GAP_EVENT_ADV_COMPLETE:
        ESP_LOGI("GAP", "BLE GAP EVENT");
        // Restart advertising
        ble_app_advertise();
        break;
    default:
        break;
    }
    return 0;
}

static void ble_app_advertise(void)
{
    // GAP - device name definition
    struct ble_hs_adv_fields fields;
    const char *device_name;
    memset(&fields, 0, sizeof(fields));
    device_name = ble_svc_gap_device_name(); // Read the BLE device name
    fields.name = (uint8_t *)device_name;
    fields.name_len = strlen(device_name);
    fields.name_is_complete = 1;
    ble_gap_adv_set_fields(&fields);

    // GAP - device connectivity definition
    memset(&adv_params, 0, sizeof(adv_params));
}

void ble_app_on_sync(void)
{
    ble_hs_id_infer_auto(0, &ble_addr_type); // Determines the best address type automatically
    ble_app_advertise();                     // Define the BLE connection
}

void host_task(void *param)
{

    /*
     * Start the NimBLE host stack task, which handles BLE operations such as
     * advertising, connections, GATT server/client actions, and event processing.
     * This function will run indefinitely unless explicitly stopped using nimble_port_stop().
    */
    nimble_port_run();
}

void connect_ble(void)
{
    // Set the direction of the GPIO pin connected to the BLE configuration button as input.
    gpio_set_direction(BLE_CONFIG_GPIO, GPIO_MODE_INPUT);

    // Initialize the NimBLE host stack, which manages BLE connections and interactions.
    nimble_port_init();

    // Set the BLE device name (defined by the macro BLE_NAME) that will appear to other devices.
    ble_svc_gap_device_name_set(BLE_NAME);

    // Initialize the GAP (Generic Access Profile) service for BLE.
    ble_svc_gap_init();
    // Initialize the GATT (Generic Attribute Profile) service for BLE.
    ble_svc_gatt_init();

    // Configure the GATT server by registering the defined services and characteristics.
    ble_gatts_count_cfg(gatt_svcs);

    // Add the GATT services defined in the `gatt_svcs` array to the BLE server.
    ble_gatts_add_svcs(gatt_svcs);

    // Set the synchronization callback function to handle BLE stack synchronization.
    ble_hs_cfg.sync_cb = ble_app_on_sync;

    // Initialize the FreeRTOS task for the NimBLE host stack.
    nimble_port_freertos_init(host_task);
}

void BLE_Task()
{
    // Get the current time in microseconds
    int64_t m = esp_timer_get_time();

    // Infinite loop for monitoring the GPIO button and handling BLE advertising
    while (1)
    {
        // Check if the button connected to BLE_CONFIG_GPIO is pressed
        if (!gpio_get_level(BLE_CONFIG_GPIO))
        {
            // Get the current time in microseconds
            int64_t n = esp_timer_get_time();
            
            // Check if the button has been pressed
            if ((n - m) / 1000 >= PRESSED_CONFIG_TIME)
            {
                ESP_LOGI("BOOT BUTTON:", "Button Pressed FOR 3 SECOND\n");

                // Set BLE advertising parameters:
                // - BLE_GAP_CONN_MODE_UND: Device is connectable
                // - BLE_GAP_DISC_MODE_GEN: Device is generally discoverable
                adv_params.conn_mode = BLE_GAP_CONN_MODE_UND; 
                adv_params.disc_mode = BLE_GAP_DISC_MODE_GEN;

                // Start BLE advertising with the defined parameters
                ble_gap_adv_start(ble_addr_type, NULL, BLE_HS_FOREVER, &adv_params, ble_gap_event, NULL);
                // Set the status flag to true to indicate advertising has started
                status = true;
                // Delay to debounce the button press
                vTaskDelay(100);
                // Update the last button press timestamp
                m = esp_timer_get_time();
            }
        }
        else
        {
            // If the button is not pressed, update the last timestamp
            m = esp_timer_get_time();
        }

        // Short delay to prevent excessive CPU usage in the loop
        vTaskDelay(10);

        // If advertising has been started, continue advertising to keep the device discoverable
        if (status)
        {
            // Set advertising parameters again (redundant but ensures persistence)
            adv_params.conn_mode = BLE_GAP_CONN_MODE_UND;
            adv_params.disc_mode = BLE_GAP_DISC_MODE_GEN;
            // Restart BLE advertising
            ble_gap_adv_start(ble_addr_type, NULL, BLE_HS_FOREVER, &adv_params, ble_gap_event, NULL);
        }
    }
}
