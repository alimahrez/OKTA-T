#ifndef MQTT_MODULE_H
#define MQTT_MODULE_H

/**
 * @brief Registers a callback function to handle the MQTT connection established event.
 *
 * @param Callback Function pointer to the callback function to execute upon connection.
 */
void MQTT_EventConnectedCallback(void (*Callback)(void));

/**
 * @brief Registers a callback function to handle MQTT data reception events.
 *
 * @param Callback Function pointer to the callback function to execute upon receiving data.
 */
void MQTT_EventDataActionCallback(void (*Callback)(void));

/**
 * @brief Registers a callback function to handle MQTT topic unsubscription events.
 *
 * @param callback Function pointer to the callback function to execute upon unsubscription.
 */
void MQTT_EventUnsubscribedCallback(void (*callback)(void));

/**
 * @brief Registers a callback function to handle MQTT disconnection events.
 *
 * @param callback Function pointer to the callback function to execute upon disconnection.
 */
void MQTT_EventDisconnectedCallback(void (*callback)(void));

/**
 * @brief Connects to an MQTT broker with specified connection parameters.
 *
 * @param MQTT_Saved_Broker The URI of the MQTT broker to connect to (e.g., "mqtt://broker.example.com").
 * @param MQTT_Saved_Port Pointer to the port number to use for the connection.
 * @param MQTT_Username Username for MQTT authentication.
 * @param MQTT_Saved_Password Password for MQTT authentication.
 */
void MQTT_Connect(char *MQTT_Saved_Broker, int32_t *MQTT_Saved_Port, char *MQTT_Username, char *MQTT_Saved_Password);

/**
 * @brief Publishes a message to a specified MQTT topic.
 *
 * @param topic_Name The name of the MQTT topic to publish to.
 * @param msg The message to publish.
 * @param durationTime_InSec The delay in seconds after publishing the message.
 */
void MQTT_Publish(char *topic_Name, char *msg, uint32_t durationTime_InSec);

/**
 * @brief Subscribes to a specified MQTT topic.
 *
 * @param topic_Name The name of the MQTT topic to subscribe to.
 */
void MQTT_Subscribe(char *topic_Name);

#endif // MQTT_MODULE_H
