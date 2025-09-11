# Sustainable Air Quality Prediction and Alert System Using IoT

## Overview
This project implements a **real-time air quality prediction and alert system** leveraging IoT technologies. It monitors environmental factors: temperature, humidity, and air quality, using onboard sensors, uploads readings to Google Sheets via Wi-Fi, and enables alert-driven responses for better sustainability and public health action.

## Features
- **Continuous monitoring** of ambient air quality, temperature, and humidity
- **Cloud integration:** sends data to Google Sheets (via Apps Script web endpoint)
- **Wi-Fi communication** for seamless remote logging/monitoring
- **Real-time alert capability** when hazardous air quality is detected


## Hardware & Tech Stack
- **Microcontroller:** ESP8266 (Wi-Fi enabled)
- **Sensors:** DHT11 (temperature/humidity), MQ135 (air quality) or similar
- **Language:** C++ (Arduino)
- **Cloud storage:** Google Sheets (Google Apps Script API)
- **Connectivity:** Wi-Fi

## How It Works
1. **Reads sensor data** every 30 seconds.
2. **Connects to Wi-Fi** using provided credentials.
3. **Sends data** (temperature, humidity, air quality) to a Google Sheet via HTTPS POST.
4. **Enables alerting/action** based on thresholds, as seen in cloud logs.

## Getting Started

### 1. Hardware wiring
- Wire DHT11/MQ135 sensors to the microcontroller per their datasheets.

### 2. Code setup
- Edit Wi-Fi credentials & Google Apps Script URL in `airQ_IDE.ino`.
- Clone the repo:
git clone https://github.com/little151/Sustainable-Air-Quality-Prediction-and-Alert-System-Using-IoT.git

- Open `airQ_IDE.ino` in Arduino IDE/PlatformIO.
- Install dependencies (`DHT`, `ESP8266WiFi`, etc.).
- Upload to board.

### 3. Google Apps Script
- Deploy a Google Apps Script set to receive POSTs and append data to a linked Sheet.
- Paste your script URL into the code.

### 4. Monitor
- Use Serial Monitor for debug.
- View data live in your Google Sheet.

## Applications
- Urban and industrial air monitoring
- Home/office health automation
- Classroom IoT examples

---

*Created for sustainable communities and open IoT innovation!*
