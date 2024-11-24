#ifndef BLE_MODULE_H
#define BLE_MODULE_H


#define BLE_CONFIG_GPIO         GPIO_NUM_5
#define PRESSED_CONFIG_TIME     3000 // IN MICRO SEC
#define BLE_NAME                "OKTA-T"

static void ble_app_advertise(void);

/**
 * @brief   .
 * @details The ble_gap_event function handles GAP events generated during BLE communication, 
 * such as connections, disconnections, and advertising completions. 
 * This ensures that the BLE device behaves as expected under different scenarios.
 **/
static int ble_gap_event(struct ble_gap_event *event, void *arg);

void ble_app_on_sync(void);

void host_task(void *param);

/**
 * @brief   .
 * @details The connect_ble function sets up the BLE environment for the ESP32, 
 * configuring it to act as a BLE server. 
 * It performs all necessary initializations and 
 * prepares the BLE stack for advertising and communication with clients.
 * @return  void
 **/

void connect_ble(void);

/**
 * @details (This function executed in a FreeRTOS task)
 * This function monitors a specific GPIO pin (likely connected to a button)
 * to initiate BLE advertising when the button is pressed for at least 2 seconds.
 * It also ensures BLE advertising persists and restarts if necessary.
 * @return  void
 **/
void BLE_Task();



#endif // BLE_MODULE_H
