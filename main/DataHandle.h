#ifndef DATAHANDLE_H
#define DATAHANDLE_H

#define WIFI_CRED_LENGTH 32
#define MQTT_CRED_LENGTH 32
#define MQTT_TOPIC_LENGTH 16

#define WIFI_CONFIG_TYPE    0
#define MQTT_CONFIG_TYPE    1
#define TOPIC_CONFIG_TYPE   2

#define TOPIC_RELAY_TYPE_1  1
#define TOPIC_RELAY_TYPE_2  2
#define TOPIC_RELAY_TYPE_3  3
#define TOPIC_RELAY_TYPE_4  4
#define TOPIC_RELAY_TYPE_5  5
#define TOPIC_RELAY_TYPE_6  6
#define TOPIC_RELAY_TYPE_7  7
#define TOPIC_RELAY_TYPE_8  8
#define TOPIC_TEMP_TYPE     9
#define TOPIC_LIGHT_TYPE    10
#define TOPIC_DOOR_TYPE 11

typedef struct Configuration
{
    int32_t mqttPort;        // 4 bytes
    int32_t configType;      // 4 bytes
    int32_t topicConfigType; // 4 bytes

    char wifiSSID[WIFI_CRED_LENGTH];     // 32 bytes
    char wifiPassword[WIFI_CRED_LENGTH]; // 32 bytes
    char mqttUsername[MQTT_CRED_LENGTH]; // 32 bytes
    char mqttPassword[MQTT_CRED_LENGTH]; // 32 bytes
    char mqttBroker[MQTT_CRED_LENGTH];   // 32 bytes

    char relay1[MQTT_TOPIC_LENGTH];      // 16 bytes
    char relay2[MQTT_TOPIC_LENGTH];      // 16 bytes
    char relay3[MQTT_TOPIC_LENGTH];      // 16 bytes
    char relay4[MQTT_TOPIC_LENGTH];      // 16 bytes
    char relay5[MQTT_TOPIC_LENGTH];      // 16 bytes
    char relay6[MQTT_TOPIC_LENGTH];      // 16 bytes
    char relay7[MQTT_TOPIC_LENGTH];      // 16 bytes
    char relay8[MQTT_TOPIC_LENGTH];      // 16 bytes
    char lightSensor[MQTT_TOPIC_LENGTH]; // 16 bytes
    char tempSensor[MQTT_TOPIC_LENGTH];  // 16 bytes
    char doorSensor[MQTT_TOPIC_LENGTH];  // 16 bytes
} credentialConfig;

typedef enum DATA_ERROR_HANDLING
{
    JS_CONFIG_TYPE_ERROR,
    JS_WIFI_CRD_ERROR,
    JS_MQTT_CRD_ERROR,
    JS_TOPIC_CONFIG_ERROR,
    JS_TOPIC_1_ERROR,
    JS_TOPIC_2_ERROR,
    JS_TOPIC_3_ERROR,
    JS_TOPIC_4_ERROR,
    JS_TOPIC_5_ERROR,
    JS_TOPIC_6_ERROR,
    JS_TOPIC_7_ERROR,
    JS_TOPIC_8_ERROR,
    JS_TOPIC_TEMP_ERROR,
    JS_TOPIC_LIGHT_ERROR,
    JS_TOPIC_DOOR_ERROR,
    ALL_IS_OK,

} DataErrorHandle;

DataErrorHandle GetDataAtRunTime(char *js_string, credentialConfig *config);
void DisplyGetError(DataErrorHandle getError);
void RetrieveConfigFromStorage(credentialConfig *config);

#endif // DATAHANDLE_H
