# 🚗 Smart Parking System using Ultrasonic Sensors & ESP32

An IoT-enabled Embedded Systems project designed for real-time automated parking bay occupancy monitoring, automated barrier control, local OLED telemetry, and interactive web dashboards.

## 📌 Overview
Urban congestion and wasted commute time frequently arise from inefficient parking spot discovery. This prototype uses an **ESP32 microcontroller** along with **4x HC-SR04 ultrasonic sensors** to detect free and occupied parking bays in real-time. It features bi-color LED indicators, an I2C OLED display, an automated servo barrier gate, a full-capacity buzzer alert, and an interactive telemetry dashboard.

## 📸 Visual Demonstration & Proof of Work

| State | Hardware / Simulation Preview |
| :--- | :--- |
| **All Slots Free** | `screenshots/all_slots_free.png` |
| **Slot Occupied Detection** | `screenshots/slot_occupied_led.png` |
| **Parking Full Alert** | `screenshots/parking_full_alert.png` |
| **Telemetry Dashboard** | `screenshots/web_dashboard.png` |

## ⚙️ Key Features
- **Real-Time Occupancy Detection:** 4 independent ultrasonic channels running outlier-trimmed averaging.
- **Anti-Jitter Debounce:** 800 ms software debounce prevents erratic sensor state switching.
- **Dual Visual Telemetry:** Instant bay-level status via bi-color LEDs and an I2C SSD1306 OLED screen.
- **Automated Access Control:** SG90 micro-servo barrier opens only when free bays exist and locks upon capacity limits.
- **Audio Overcapacity Alarm:** Active buzzer alert triggers when all slots are occupied.
- **Live Web Telemetry:** Streamlit and HTTP-based dashboards for remote parking monitoring.

## 🛠️ Hardware & Software Architecture

### Hardware Components
- **Microcontroller:** ESP32 DevKit V1
- **Sensors:** 4x HC-SR04 Ultrasonic Transceivers
- **Actuators:** SG90 Micro Servo Motor & 5V Active Piezo Buzzer
- **Visual Display:** SSD1306 0.96" I2C OLED (128x64)
- **Indicators:** 4x Green LEDs (Available) & 4x Red LEDs (Occupied)

### Software & Frameworks
- **Embedded C++ / Arduino Core**
- **Wokwi Simulator** (Virtual validation)
- **Python 3.x & Streamlit** (Interactive monitoring dashboard)

## 📐 Mathematical Formulation & Logic

### Distance Calculation
The HC-SR04 emits a 40 kHz ultrasonic burst upon receiving a 10 µs pulse on the trigger pin. Distance is calculated based on time-of-flight:

$$\text{Distance (cm)} = \frac{\text{Echo Duration (}\mu\text{s)} \times 0.0343}{2} \approx \frac{\text{Echo Duration}}{58}$$

### Decision Rule
$$\text{Slot State} = \begin{cases} \text{OCCUPIED}, & \text{if } \text{Distance} \le 35\text{ cm} \\ \text{FREE}, & \text{if } \text{Distance} > 35\text{ cm} \end{cases}$$

## 🔌 Pin Mapping Configuration

| Component | Pin Function | ESP32 GPIO |
| :--- | :--- | :--- |
| **SSD1306 OLED** | SDA / SCL | GPIO 21 / GPIO 22 |
| **SG90 Servo** | PWM Signal | GPIO 18 |
| **Piezo Buzzer** | Positive (+) | GPIO 19 |
| **Slot 1 (TRIG / ECHO)** | HC-SR04 | GPIO 5 / GPIO 17 |
| **Slot 2 (TRIG / ECHO)** | HC-SR04 | GPIO 16 / GPIO 4 |
| **Slot 3 (TRIG / ECHO)** | HC-SR04 | GPIO 27 / GPIO 26 |
| **Slot 4 (TRIG / ECHO)** | HC-SR04 | GPIO 25 / GPIO 33 |
| **Slot LEDs (Green / Red)** | S1 to S4 | `(12/2), (14/13), (32/23), (15/0)` |

## 🚀 How to Run

### 1. Embedded Firmware (ESP32)
1. Open `firmware/smart_parking_system.ino` in **Arduino IDE**.
2. Install required libraries from Library Manager:
   - `Adafruit SSD1306`
   - `Adafruit GFX Library`
   - `ESP32Servo`
3. Select board **ESP32 Dev Module** and upload the code.

### 2. Interactive Python Dashboard
1. Install dependencies:
   ```bash
   pip install streamlit pandas

   Run the application: streamlit run dashboard/app.py

### 📁 Repository Structure
```
   Smart-Parking-Ultrasonic-Embedded-System/
├── firmware/
│   └── smart_parking_system.ino
├── dashboard/
│   └── app.py
├── simulation/
│   ├── diagram.json
│   └── wokwi.toml
├── circuit/
│   ├── PIN_MAPPING.md
├── docs/
│   ├── PROJECT_REPORT.md
│   └── INTERVIEW_QA.md
├── screenshots/
│   ├── all_slots_free.png
│   ├── slot_occupied_led.png
│   ├── parking_full_alert.png
│   └── web_dashboard.png
├── .gitignore
├── LICENSE
└── README.md
```