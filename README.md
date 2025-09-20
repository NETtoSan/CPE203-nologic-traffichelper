# 🚦 Pedestrian Traffic Light

## Overview
This project is a **traffic light system with pedestrian crossing support**, built with an **ESP32**, an **OLED display**, ultrasonic sensor and photoresistor

- **Pedestrians press a button** to request crossing.  
- An **ultrasonic sensor** detects nearby cars (to delay crossing if a car is too close).  
- An **LDR sensor** detects day/night to adjust crossing duration.  
- An **OLED screen** shows traffic light states and countdowns.  

This makes it behave more like a real-world smart pedestrian crossing.

---

## Hardware Used
- ESP32 (WROOM32 or similar)
- OLED Display
- Ultrasonic Sensor (HC-SR04)
- LDR (light-dependent resistor)
- Push button (for pedestrian request)
- 3 LEDs (Red, Yellow, Green) + resistors
- Jumper wires, breadboard

---
##  Getting Started
1. Install **Arduino IDE** with ESP32 board support.  
2. Install required libraries:  
   - `Adafruit_GFX`  
   - `Adafruit_SSD1306`  
3. Upload the code to ESP32.  
4. Wire components according to the table above.  
5. Open **Serial Monitor** to debug sensor readings.  

## Pin Connections

| Component       | ESP32 Pin |
|-----------------|-----------|
| **Button**      | GPIO 15   |
| **Ultrasonic TRIG** | GPIO 16 |
| **Ultrasonic ECHO** | GPIO 17 |
| **LDR** (Analog) | GPIO 34   |
| **Green LED**   | GPIO 5    |
| **Yellow LED**  | GPIO 18   |
| **Red LED**     | GPIO 19   |
| **OLED (I2C)**  | SDA: 21, SCL: 22 |

---

## How It Works
1. **Normal Mode** → Cars have **Green light**.  
2. **Pedestrian presses button**:  
   - If a car is very close (<30 cm), green light stays on for 3 seconds.  
   - Then **Yellow light** turns on for 3 seconds.  
   - Then **Red light** for cars.  
3. **Crossing Mode** → Pedestrians can cross:  
   - **Daytime (bright)**: 15 seconds crossing time.  
   - **Nighttime (dark)**: 5 seconds crossing time.  
   - Countdown is shown on OLED (*Cross: X*).  
4. **After countdown ends** → Back to **Green light for cars**, OLED shows *Go Car*.  

---

## OLED Display
The OLED shows:  
- Initial splash screen (*Traffic* with a car drawing).  
- Current phase (Green countdown, Yellow countdown, Cross countdown).  
- Message *Go Car* when crossing ends.

---

## Demo Flow
- **Idle state:** Green light ON for cars.  
- **Pedestrian presses button → system checks car distance.**  
- **Yellow light countdown → Red light ON → Pedestrian crossing countdown.**  
- **Back to Green for cars.**

---

## Configuration
You can tweak timings in the code:
```cpp
const int distanceThreshold = 100;  // cm, detect nearby cars
const int ldrThreshold = 1000;      // Bright/dark threshold
const int crossDayTime = 15;        // Crossing time (day)
const int crossNightTime = 5;       // Crossing time (night)
const int globalTime = 3;           // Yellow/Green delay
```

---


