# PET Bottle WiFi Vendo System

This project is an Arduino-based **PET bottle recycling vending machine** integrated with an **Orange Pi running Adopisoft (WiFi vendo system)**. Users can insert PET bottles and receive **free WiFi access** as a reward.

The system promotes environmental awareness by encouraging recycling while providing internet access as an incentive.

---

## Project Overview

The system consists of two main parts:

1. **Arduino Controller (Bottle Detection & Validation)**
2. **Orange Pi (WiFi Vendo using Adopisoft)**

### Workflow:
- User inserts a PET bottle  
- System validates the bottle using sensors and weight  
- If valid → system sends pulses to Orange Pi  
- Orange Pi credits WiFi time using Adopisoft  
- User receives **free internet access**  

---

## Features

### ♻️ PET Bottle Detection
- Detects inserted objects using:
  - IR sensors  
  - Photoelectric sensor  
- Ensures object is a valid PET bottle  

---

### ⚖️ Weight-Based Validation
- Uses **HX711 Load Cell**
- Accepts only bottles within valid weight range:
  - Rejects too light (empty/invalid)
  - Rejects too heavy (non-bottle objects)

---

### 🚫 Rejection System
- Invalid objects:
  - trigger buzzer alert  
  - display rejection message on LCD  

---

### ✅ Automatic Acceptance
- Valid bottle:
  - opens lid via servo  
  - accepts bottle  
  - sends reward signal  

---

### 📶 WiFi Reward System (Adopisoft)

- Integrated with **Orange Pi WiFi Vendo**
- Arduino sends **pulse signals**
- Each pulse corresponds to:
  - WiFi credits (time or data)

---

### 📺 LCD Display Interface
- 16x2 I2C LCD shows:
  - "Insert PET Bottle"
  - "Accepted"
  - "Rejected"
  - "Bin Full"
  - System status  

---

### 🔊 Buzzer Alerts
- Activated when:
  - invalid object detected  
  - system warning  

---

### 🗑️ Bin Full Detection
- Uses IR sensor to detect bin capacity  
- Stops accepting bottles when full  

---

### 🤖 Servo-Controlled Lid
- Opens when bottle is accepted  
- Closes automatically after insertion  

---

## System Workflow

### 1. Idle State
- Displays: PLEASE INSERT PET BOTTLE

---

### 2. Bottle Detection
- Detects object using sensors  
- Waits for stable reading  

---

### 3. Validation

#### Checks:
- Object presence  
- Bottle type (photoelectric sensor)  
- Weight range  

---

### 4. Decision

#### ❌ Invalid Bottle:
- Displays: PET BOTTLE REJECTED
- Activates buzzer  

---

#### ✅ Valid Bottle:
- Displays: PET BOTTLE ACCEPTED
- Opens lid  
- Sends pulses to Orange Pi  

---

### 5. WiFi Credit

- Arduino sends pulse signals:
- 1–4 pulses depending on bottle weight  
- Orange Pi (Adopisoft):
- converts pulses into WiFi credits  

---

### 6. Reset
- Closes lid  
- Resets scale  
- Returns to idle state  

---

## Pin Configuration

| Component              | Arduino Pin |
|----------------------|------------|
| Pulse Output         | 9          |
| Orange Pi Ready      | 10         |
| Servo Motor          | 11         |
| Buzzer               | 8          |
| Photoelectric Sensor | 5          |
| IR Bin Sensor        | 4          |
| HX711 DT             | 3          |
| HX711 SCK            | 2          |
| Object Sensor 1      | 6          |
| Object Sensor 2      | 7          |
| LCD (I2C)            | SDA/SCL    |

---

## Wiring Diagram

📄 Refer to wiring diagram:  
:contentReference[oaicite:0]{index=0}  

### Key Highlights (from diagram):

- Arduino Uno controls:
- sensors  
- servo  
- load cell  
- HX711 connected to load cell  
- IR sensors detect bin and object  
- Photoelectric sensor validates bottle  
- Relay modules interface with Orange Pi  
- Orange Pi handles WiFi vending (Adopisoft)  
- LCD connected via I2C  

---

## Hardware Components

- Arduino Uno  
- Orange Pi (Adopisoft WiFi Vendo)  
- HX711 Load Cell + Load Cell Sensor  
- Servo Motor  
- IR Sensors (bin + object detection)  
- Photoelectric Sensor  
- 16x2 LCD I2C  
- Buzzer  
- Relay Modules  
- Power Supply  

---

## Software Components

### Arduino
- Controls:
- bottle detection  
- validation  
- servo  
- pulse output  

📄 Source Code:  
:contentReference[oaicite:1]{index=1}  

---

### Orange Pi (Adopisoft)
- Handles:
- WiFi vending system  
- credit management  
- hotspot access  

---

## Reward Logic

| Weight Range | Pulses | WiFi Reward |
|-------------|--------|------------|
| ≤ 14        | 1      | Low        |
| ≤ 17        | 2      | Medium     |
| ≤ 21        | 3      | High       |
| > 21        | 4      | Premium    |

---

## Notes

- Load cell requires calibration (`set_scale`)  
- Bottle must pass all checks to be accepted  
- Orange Pi must be ready before operation  
- Pulse timing is critical for credit accuracy  

---

## Limitations

- No mobile or web dashboard  
- No cloud monitoring  
- Weight-based validation only (no image recognition)  
- Requires stable power supply  

---

## Summary

This project demonstrates a **smart recycling incentive system** that combines:

- embedded systems (Arduino)  
- sensor-based validation  
- automation (servo + logic)  
- WiFi vending integration (Orange Pi + Adopisoft)  

It is suitable for:

- recycling stations  
- smart cities projects  
- environmental awareness systems  
- IoT-based vending solutions  
