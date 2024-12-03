/******************************************************************************
 * @file        DATAHANDLE.h
 * @brief       Data handling module for managing runtime configuration and persistent storage.
 *
 * @author      Ali Mahrez
 * @company     Smart Egat
 * @email       a.mahrez@smart-egat.com
 * @date        Nov 27, 2024
 * @version     Xbeta
 *
 * @details
 * This header file declares the APIs for extracting and updating runtime configuration
 * from a JSON string, storing and retrieving configurations from persistent memory,
 * and managing error handling related to configuration data. It provides a set of functions
 * to handle Wi-Fi, MQTT, and topic configuration for relays and sensors.
 ******************************************************************************/

#ifndef DATAHANDLE_H
#define DATAHANDLE_H

// Definitions for configuration lengths
#define WIFI_CRED_LENGTH 32  // Maximum length for Wi-Fi credentials (SSID and password)
#define MQTT_CRED_LENGTH 32  // Maximum length for MQTT credentials (broker, username, password)
#define MQTT_TOPIC_LENGTH 16 // Maximum length for MQTT topic names

// Configuration type identifiers
#define WIFI_CONFIG_TYPE 0
#define MQTT_CONFIG_TYPE 1
#define TOPIC_CONFIG_TYPE 2

// Topic type identifiers
#define TOPIC_RELAY_TYPE_1 1
#define TOPIC_RELAY_TYPE_2 2
#define TOPIC_RELAY_TYPE_3 3
#define TOPIC_RELAY_TYPE_4 4
#define TOPIC_RELAY_TYPE_5 5
#define TOPIC_RELAY_TYPE_6 6
#define TOPIC_RELAY_TYPE_7 7
#define TOPIC_RELAY_TYPE_8 8
#define TOPIC_TEMP_TYPE 9
#define TOPIC_LIGHT_TYPE 10
#define TOPIC_DOOR_TYPE 11

/**
 * @brief Structure to hold configuration data for Wi-Fi, MQTT, and relay topics.
 *
 * @details
 * This structure holds all configuration data required for setting up the system, including
 * Wi-Fi credentials, MQTT credentials, and topic configurations for relays and sensors.
 * Each field in this structure corresponds to a specific configuration value used by the system.
 */
typedef struct Configuration
{
    int32_t mqttPort;        // MQTT port
    int32_t configType;      // Configuration type (Wi-Fi, MQTT, or Topic)
    int32_t topicConfigType; // Topic configuration type (for relays or sensors)

    char wifiSSID[WIFI_CRED_LENGTH];     // Wi-Fi SSID
    char wifiPassword[WIFI_CRED_LENGTH]; // Wi-Fi password
    char mqttUsername[MQTT_CRED_LENGTH]; // MQTT username
    char mqttPassword[MQTT_CRED_LENGTH]; // MQTT password
    char mqttBroker[MQTT_CRED_LENGTH];   // MQTT broker URI

    char relay1[MQTT_TOPIC_LENGTH];      // Relay 1 topic
    char relay2[MQTT_TOPIC_LENGTH];      // Relay 2 topic
    char relay3[MQTT_TOPIC_LENGTH];      // Relay 3 topic
    char relay4[MQTT_TOPIC_LENGTH];      // Relay 4 topic
    char relay5[MQTT_TOPIC_LENGTH];      // Relay 5 topic
    char relay6[MQTT_TOPIC_LENGTH];      // Relay 6 topic
    char relay7[MQTT_TOPIC_LENGTH];      // Relay 7 topic
    char relay8[MQTT_TOPIC_LENGTH];      // Relay 8 topic
    char lightSensor[MQTT_TOPIC_LENGTH]; // Light sensor topic
    char tempSensor[MQTT_TOPIC_LENGTH];  // Temperature sensor topic
    char doorSensor[MQTT_TOPIC_LENGTH];  // Door sensor topic
} credentialConfig;

/**
 * @brief Enum for error handling during the data extraction process.
 *
 * @details
 * This enum defines different error codes used to represent various issues encountered
 * during the extraction and parsing of configuration data from a JSON string.
 */
typedef enum DATA_ERROR_HANDLING
{
    JS_CONFIG_TYPE_ERROR,  // Error: Invalid configuration type
    JS_WIFI_CRD_ERROR,     // Error: Invalid Wi-Fi credentials
    JS_MQTT_CRD_ERROR,     // Error: Invalid MQTT credentials
    JS_TOPIC_CONFIG_ERROR, // Error: Invalid topic configuration
    JS_TOPIC_1_ERROR,      // Error: Invalid topic for relay 1
    JS_TOPIC_2_ERROR,      // Error: Invalid topic for relay 2
    JS_TOPIC_3_ERROR,      // Error: Invalid topic for relay 3
    JS_TOPIC_4_ERROR,      // Error: Invalid topic for relay 4
    JS_TOPIC_5_ERROR,      // Error: Invalid topic for relay 5
    JS_TOPIC_6_ERROR,      // Error: Invalid topic for relay 6
    JS_TOPIC_7_ERROR,      // Error: Invalid topic for relay 7
    JS_TOPIC_8_ERROR,      // Error: Invalid topic for relay 8
    JS_TOPIC_TEMP_ERROR,   // Error: Invalid topic for temperature sensor
    JS_TOPIC_LIGHT_ERROR,  // Error: Invalid topic for light sensor
    JS_TOPIC_DOOR_ERROR,   // Error: Invalid topic for door sensor
    ALL_IS_OK,             // No errors, all data is valid
} DataErrorHandle;

/**
 * @brief Extracts and processes data from the provided JSON string.
 *
 * @param js_string (char *): The JSON string containing configuration data.
 * @param config (credentialConfig *): Pointer to the configuration structure to be updated.
 *
 * @return DataErrorHandle: Returns a value from the DataErrorHandle enum indicating success or failure.
 *
 * @details
 * This function processes a JSON string representing runtime configuration for Wi-Fi,
 * MQTT, and topics. It updates the provided configuration structure with the extracted data.
 * If an error occurs during data extraction, the function returns the appropriate error code.
 */
DataErrorHandle GetDataAtRunTime(char *js_string, credentialConfig *config);

/**
 * @brief Logs appropriate error messages based on the provided error code.
 *
 * @param getError (DataErrorHandle): The error code indicating the type of error.
 *
 * @details
 * This function logs the appropriate error message for the given error code. It uses
 * ESP-IDF's logging functions to output error messages, allowing the user to diagnose
 * configuration issues.
 */
void DisplyGetError(DataErrorHandle getError);

/**
 * @brief Retrieves configuration data from persistent storage.
 *
 * @param config (credentialConfig *): Pointer to the configuration structure to be updated with data from storage.
 *
 * @details
 * This function loads the stored configuration data (such as Wi-Fi credentials, MQTT
 * settings, and relay topics) from non-volatile storage and updates the provided configuration
 * structure with the retrieved values.
 */
void RetrieveConfigFromStorage(credentialConfig *config);

#endif // DATAHANDLE_H
