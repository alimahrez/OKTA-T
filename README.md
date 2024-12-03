# OKTA-T Beta IoT KIT

## Overview

The **OKTA-T** is a configurable IoT system designed for managing and controlling relays and sensors through Bluetooth Low Energy (BLE), Wi-Fi, and MQTT. It supports real-time configuration via JSON and persistent storage, enabling flexibility for different IoT applications.

This project is built on the ESP32 platform and uses the ESP-IDF framework for development. The system allows communication with BLE clients and provides configuration settings for Wi-Fi, MQTT, and topics for relays and sensors. It is intended for educational and industrial applications, focusing on ease of use and flexibility.

## Features

- **Wi-Fi Configuration**: Allows dynamic configuration of Wi-Fi SSID and password.
- **MQTT Configuration**: Connects to MQTT brokers with configurable credentials (broker address, port, username, password).
- **Topic Management**: Supports configuration for multiple topics related to relays and sensors.
- **JSON-Based Configuration**: The system accepts runtime configuration via JSON, allowing dynamic updates.
- **Persistent Storage**: All configuration data is saved to non-volatile storage for resilience across reboots.
- **BLE Server**: The ESP32 device acts as a BLE server to interact with BLE clients and provides access to the system's configuration.

## Components

- **Wi-Fi Module**: Manages the Wi-Fi connection setup.
- **MQTT Module**: Handles communication with the MQTT broker for message publishing and subscription.
- **BLE Module**: Provides BLE server functionality for communication with clients.
- **Memory Module**: Handles the saving and loading of configuration data to non-volatile storage.
- **JSON Module**: Parses and processes JSON strings for configuration management.
- **Data Handling Module**: Processes configuration data and manages the operation of the system.

## Requirements

- **ESP32 Development Board** (e.g., ESP-WROOM-32)
- **ESP-IDF Framework**: v4.x or higher
- **FreeRTOS**: For task management
- **VSCode** (for development)
- **Libraries**:
  - `nvs_flash`: For persistent storage (non-volatile storage)
  - `cJSON`: For JSON parsing
  - `driver/gpio`: For GPIO control
  - `esp_wifi` and `esp_http_client`: For Wi-Fi connection management
  - `mqtt_client`: For MQTT communication

## Installation

### 1. Set up ESP-IDF

Follow the [official ESP-IDF setup guide](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/get-started/) to install the ESP-IDF on your development machine.

### 2. Clone the repository

Clone the project repository to your local machine:

```bash
git clone https://github.com/alimahrez/OKTA-T
cd OKTA-T
