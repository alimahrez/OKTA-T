/******************************************************************************
 * @file        WIFI_module.h
 * @brief       Wi-Fi module header for managing ESP32 Wi-Fi functionality.
 *
 * @author      Ali Mahrez
 * @company     Smart Egat
 * @email       a.mahrez@smart-egat.com
 * @date        Nov 28, 2024
 * @version     Xbeta
 *
 * @details
 * This header file declares the APIs for initializing the Wi-Fi subsystem,
 * establishing a connection to a Wi-Fi network, and verifying internet connectivity
 * on an ESP32 device. It provides a simple interface for setting up Wi-Fi in station
 * mode and checking connectivity status.
 ******************************************************************************/
#ifndef WIFI_MODULE_H
#define WIFI_MODULE_H

/**
 * @brief Initializes the Wi-Fi subsystem in station mode.
 *
 * @param SSID (char *): The SSID (name) of the Wi-Fi network to connect to.
 * @param PASS (char *): The password for the Wi-Fi network.
 *
 * @details
 * Configures the ESP32 Wi-Fi subsystem to operate in station mode. The provided
 * SSID and password are used to prepare the device for connecting to the specified
 * access point. This function must be called before attempting to establish a connection.
 */
void WIFI_Init(char *SSID, char *PASS);

/**
 * @brief Starts the Wi-Fi connection process.
 *
 * @details
 * This function begins the process of connecting the ESP32 to the Wi-Fi network
 * specified during initialization. It starts the Wi-Fi subsystem and attempts to
 * establish a connection with the configured SSID and password.
 */
void WIFI_StartConnection();

/**
 * @brief Checks if the device is connected to the internet.
 *
 * @return bool
 * - `true`: If the device is connected to the internet.
 * - `false`: If the device is not connected to the internet.
 *
 * @details
 * This function performs an HTTP request to a known URL (e.g., `http://www.google.com`)
 * to verify internet connectivity. It returns `true` if the request is successful,
 * indicating that the internet is accessible.
 */
bool WIFI_IsInternetConnected();

#endif // WIFI_MODULE_H