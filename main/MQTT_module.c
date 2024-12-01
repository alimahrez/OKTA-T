#include <stdio.h>                 // Standard input/output functions
#include <stdbool.h>               // Standard boolean type
#include "freertos/FreeRTOS.h"     // FreeRTOS core definitions
#include "freertos/task.h"         // FreeRTOS task management
#include "freertos/event_groups.h" // FreeRTOS event group management
#include "mqtt_client.h"           // MQTT client API for ESP-IDF
#include "esp_event.h"             // Event handling in ESP-IDF
#include "esp_log.h"               // Logging module for ESP-IDF
#include "cJSON.h"                 // JSON parsing library
#include "MQTT_module.h"           // Custom MQTT module header (if any)

/// Callback function pointers for MQTT events
void static (*Connected_CallBack)(void);    // Called when MQTT connection is established
void static (*Data_Callback)(void);         // Called when MQTT data is received
void static (*Unsubscribe_callback)(void);  // Called when unsubscribed from a topic
void static (*Disconnected_callback)(void); // Called when MQTT connection is disconnected

esp_mqtt_client_handle_t client;       // MQTT client handle
esp_mqtt_event_handle_t General_event; // General MQTT event handle
static const char *MQTT_TAG = "MQTT";  // Logging tag for MQTT module

/**
 * @brief Set the callback for MQTT connection established event
 * @param Callback Function pointer for the event
 */
void MQTT_EventConnectedCallback(void (*Callback)(void))
{
    Connected_CallBack = Callback;
}

/**
 * @brief Set the callback for MQTT data reception event
 * @param Callback Function pointer for the event
 */
void MQTT_EventDataActionCallback(void (*Callback)(void))
{
    Data_Callback = Callback;
}

/**
 * @brief Set the callback for MQTT topic unsubscription event
 * @param callback Function pointer for the event
 */
void MQTT_EventUnsubscribedCallback(void (*callback)(void))
{
    Unsubscribe_callback = callback;
}

/**
 * @brief Set the callback for MQTT disconnection event
 * @param callback Function pointer for the event
 */
void MQTT_EventDisconnectedCallback(void (*callback)(void))
{
    Disconnected_callback = callback;
}

/**
 * @brief MQTT event handler to process events and invoke appropriate callbacks
 * @param handler_args Arguments for the handler
 * @param base Event base
 * @param event_id ID of the event
 * @param event_data Data related to the event
 */
static void mqtt_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data)
{
    esp_mqtt_event_handle_t event = event_data;
    General_event = event_data; // Store the general event for potential use
    switch (event_id)
    {
    case MQTT_EVENT_CONNECTED: // MQTT connection established
        Connected_CallBack();  // Invoke the connection callback
        break;

    case MQTT_EVENT_DATA: // MQTT data received
        Data_Callback();  // Invoke the data callback
        break;

    case MQTT_EVENT_UNSUBSCRIBED: // MQTT topic unsubscription
        Unsubscribe_callback();   // Invoke the unsubscription callback
        break;

    case MQTT_EVENT_DISCONNECTED: // MQTT connection disconnected
        Disconnected_callback();  // Invoke the disconnection callback
        break;

    default: // Any other MQTT event
        break;
    }
}

/**
 * @brief Connect to an MQTT broker with specified parameters
 * @param MQTT_Saved_Broker Broker URI (e.g., "mqtt://example.com")
 * @param MQTT_Saved_Port Pointer to the port number
 * @param MQTT_Username Username for authentication
 * @param MQTT_Saved_Password Password for authentication
 */
void MQTT_Connect(char *MQTT_Saved_Broker, int32_t *MQTT_Saved_Port, char *MQTT_Username, char *MQTT_Saved_Password)
{
    // MQTT configuration struct with broker URI, port, and credentials
    esp_mqtt_client_config_t mqtt_cfg = {
        .broker = {
            .address = {
                .uri = MQTT_Saved_Broker,          // MQTT broker URI
                .port = (uint32_t)MQTT_Saved_Port, // MQTT broker port
            }},
        .credentials = {.username = MQTT_Username, // MQTT username
                        .authentication = {
                            .password = MQTT_Saved_Password, // MQTT password
                        }},
    };

    client = esp_mqtt_client_init(&mqtt_cfg);                                           // Initialize MQTT client
    esp_mqtt_client_register_event(client, ESP_EVENT_ANY_ID, mqtt_event_handler, NULL); // Register event handler
    esp_mqtt_client_start(client);                                                      // Start the MQTT client
}

/**
 * @brief Publish a message to a specific MQTT topic
 * @param topic_Name Name of the topic
 * @param msg Message to publish
 * @param durationTime_InSec Delay after publishing (in seconds)
 */
void MQTT_Publish(char *topic_Name, char *msg, uint32_t durationTime_InSec)
{
    esp_mqtt_client_publish(client, topic_Name, msg, 0, 1, 0);        // Publish message to topic
    vTaskDelay(pdMS_TO_TICKS((uint32_t)(durationTime_InSec * 1000))); // Delay for the specified duration
}

/**
 * @brief Subscribe to an MQTT topic
 * @param topic_Name Name of the topic to subscribe to
 */
void MQTT_Subscribe(char *topic_Name)
{
    esp_mqtt_client_subscribe(client, topic_Name, 0);          // Subscribe to the topic
    ESP_LOGI(MQTT_TAG, "Subscribed to topic: %s", topic_Name); // Log the subscription
}
