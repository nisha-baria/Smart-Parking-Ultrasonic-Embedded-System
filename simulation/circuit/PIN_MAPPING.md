# Circuit Schematic & Pin Mapping Guide

This document details the hardware connections and interfacing between the ESP32 microcontroller, ultrasonic sensors, status LEDs, I2C OLED display, and actuators.

## 📌 Master Pin Configuration Table

| Sub-System | Hardware Component | Component Pin | ESP32 GPIO Pin | Signal Type / Protocol | Operating Voltage |
| :--- | :--- | :--- | :--- | :--- | :--- |
| **I2C Display** | SSD1306 0.96" OLED | `SDA` | **GPIO 21** | I2C Serial Data | 3.3V |
| | | `SCL` | **GPIO 22** | I2C Serial Clock | 3.3V |
| | | `VCC` | `3V3` | Power | 3.3V |
| | | `GND` | `GND` | Ground | 0V |
| **Gate Control** | SG90 Micro Servo | `PWM (Orange)` | **GPIO 18** | Hardware PWM (50 Hz) | 5V |
| | | `V+ (Red)` | `5V / VIN` | Power Supply | 5V |
| | | `GND (Brown)` | `GND` | Common Ground | 0V |
| **Audio Alert** | 5V Active Piezo Buzzer | `Positive (+)` | **GPIO 19** | Digital Output | 3.3V / 5V |
| | | `Negative (-)` | `GND` | Ground | 0V |
| **Parking Slot 1**| HC-SR04 Ultrasonic | `TRIG` | **GPIO 5** | Digital Output Pulse | 5V |
| | | `ECHO` | **GPIO 17** | Digital Pulse In | 5V (Divided to 3.3V) |
| | Green LED | `Anode (+)` | **GPIO 12** | Digital Output (220Ω Resistor) | 3.3V |
| | Red LED | `Anode (+)` | **GPIO 2** | Digital Output (220Ω Resistor) | 3.3V |
| **Parking Slot 2**| HC-SR04 Ultrasonic | `TRIG` | **GPIO 16** | Digital Output Pulse | 5V |
| | | `ECHO` | **GPIO 4** | Digital Pulse In | 5V (Divided to 3.3V) |
| | Green LED | `Anode (+)` | **GPIO 14** | Digital Output (220Ω Resistor) | 3.3V |
| | Red LED | `Anode (+)` | **GPIO 13** | Digital Output (220Ω Resistor) | 3.3V |
| **Parking Slot 3**| HC-SR04 Ultrasonic | `TRIG` | **GPIO 27** | Digital Output Pulse | 5V |
| | | `ECHO` | **GPIO 26** | Digital Pulse In | 5V (Divided to 3.3V) |
| | Green LED | `Anode (+)` | **GPIO 32** | Digital Output (220Ω Resistor) | 3.3V |
| | Red LED | `Anode (+)` | **GPIO 23** | Digital Output (220Ω Resistor) | 3.3V |
| **Parking Slot 4**| HC-SR04 Ultrasonic | `TRIG` | **GPIO 25** | Digital Output Pulse | 5V |
| | | `ECHO` | **GPIO 33** | Digital Pulse In | 5V (Divided to 3.3V) |
| | Green LED | `Anode (+)` | **GPIO 15** | Digital Output (220Ω Resistor) | 3.3V |
| | Red LED | `Anode (+)` | **GPIO 0** | Digital Output (220Ω Resistor) | 3.3V |

## ⚡ Important Hardware Wiring Notes
1. **Voltage Level Protection (ECHO Pins):** HC-SR04 sensors output a 5V logic signal on the `ECHO` pin. In real hardware, use a voltage divider (1kΩ and 2kΩ resistors) to step down the signal to 3.3V before feeding it into the ESP32 GPIOs.
2. **Current Limiting:** Always place a 220Ω resistor in series with each LED anode to prevent over-current damage to the ESP32 GPIOs.
3. **Common Ground:** Ensure all components (Sensors, Servo, Display, Buzzer, ESP32) share a common `GND` rail to avoid reference floating.