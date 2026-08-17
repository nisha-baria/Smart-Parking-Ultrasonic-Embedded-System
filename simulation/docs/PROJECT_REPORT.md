# Technical Project Report: Smart Parking System using Ultrasonic Sensors

## 1. Abstract
Urban congestion and wasted commute time frequently arise from inefficient parking management. This project implements a real-time automated parking monitoring prototype using an ESP32 microcontroller, 4x HC-SR04 ultrasonic sensors, bi-color visual indicators, an SSD1306 OLED telemetry display, an automated SG90 servo entry gate, and a telemetry web dashboard. The system reduces cruising time, prevents overcapacity entry, and delivers space utilization telemetry.

## 2. Problem Statement & Objectives
### Problem Statement
Traditional parking infrastructures depend on manual tracking or static signage, leading to congestion at entry gates and increased fuel emissions while drivers search for empty spaces.

### Objectives
- Automate multi-slot occupancy detection using ultrasonic Time-of-Flight (ToF) principles.
- Provide visual feedback per bay (Green: Available, Red: Occupied).
- Implement barrier gate logic that opens automatically when spots exist and blocks entry on capacity limits.
- Broadcast real-time availability over I2C OLED screens and local network web dashboards.

## 3. System Architecture & Workflow

```text
[ Parking Bays 1 to 4 ]
          │
          ▼
 [ HC-SR04 Ultrasonic Sensors ]  ── (Time-of-Flight Echo Pulse)
          │
          ▼
   [ ESP32 Microcontroller ]
   ├── Outlier-Trimmed Averaging (5 Samples)
   ├── Software Debounce Filtering (800ms)
   └── Threshold Comparison (<= 35 cm: Occupied)
          │
  ┌───────┼──────────────────────────────┐
  ▼       ▼                              ▼
[LEDs] [SSD1306 OLED]      [SG90 Barrier & Buzzer]
(R/G)  (Live Counts/Slots) (Access / Overcapacity Alert)
```

## 4. Mathematical Formulation & Working Principle
The HC-SR04 emits a 40 kHz ultrasonic burst upon receiving a 10µs pulse on the trigger pin. The time duration ($t$) the echo pin stays HIGH determines the distance:

$$\text{Distance (cm)} = \frac{t \times v_{\text{sound}}}{2} = \frac{t \times 0.0343 \text{ cm/\mu s}}{2} \approx \frac{t}{58}$$

### Decision Logic:
- **Threshold Limit:** 35.0 cm
- **State Evaluation:**
  $$\text{State}_i = \begin{cases} \text{OCCUPIED}, & \text{if } \text{Distance}_i \le 35\text{ cm} \\ \text{FREE}, & \text{if } \text{Distance}_i > 35\text{ cm} \end{cases}$$
- **Debounce Implementation:** State changes require stable readings sustained beyond 800 ms to eliminate acoustic noise and false reflections.

## 5. Software Implementation Highlights
- **Outlier Filtering:** 5 sequential distance reads per sensor; maximum and minimum outliers are discarded, and the remaining 3 values are averaged.
- **Non-Blocking Architecture:** Timed polling using `millis()` prevents delay execution lockups.
- **Dual Display Telemetry:** Simultaneous updates via I2C SSD1306 OLED and HTTP-based WebServer.

## 6. Applications & Future Scope
- **Applications:** Smart city infrastructures, airport multi-tier parking, commercial malls, corporate office facilities, and university campuses.
- **Future Improvements:** ANPR (Automatic Number Plate Recognition) integration via ESP32-CAM, cloud analytics pipeline via MQTT/AWS IoT, and RFID automated billing.