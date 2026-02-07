# Smart Accident Detection & Alert System – ESP8266 IoT Project

## Overview
The **Smart Accident Detection and Alert System** is an IoT-based safety project that detects vehicle accidents in real time and instantly sends alert notifications with live location details.  
It uses an **ESP8266 microcontroller**, motion sensors, GPS, and a web dashboard built with **HTML, CSS, and JavaScript**.

This project aims to reduce emergency response time and improve road safety by automatically informing contacts or authorities when an accident occurs.

---

## Features
- Real-time accident detection using MPU6050 sensor
- Live GPS location tracking
- Instant alert message on accident detection
- Web dashboard for monitoring sensor data
- Lightweight and low-cost implementation
- Works over Wi-Fi (IoT enabled)

---

## Technologies Used
### Hardware
- ESP8266 (NodeMCU)
- MPU6050 Accelerometer & Gyroscope
- GPS Module (NEO-6M or similar)
- Power Supply / Battery

### Software
- **Arduino IDE** – ESP programming  
- **HTML** – Dashboard structure  
- **CSS** – Dashboard styling  
- **JavaScript** – Live data updates  
- **ESP Web Server** – Local hosting  

---

## How It Works
1. ESP8266 continuously reads motion and tilt data from MPU6050.
2. GPS module provides real-time coordinates.
3. If vibration/tilt exceeds threshold, system detects an accident.
4. ESP triggers:
   - Alert message
   - Location sharing
   - Dashboard warning popup
5. Data is displayed live on a local web dashboard.

---

## Project Structure
project-folder/
│
├── esp_code/
│ └── accident_detection.ino
│
├── web_dashboard/
│ ├── index.html
│ ├── style.css
│ └── script.js
│
└── README.md

---

## Setup Instructions
### ESP Setup
1. Install **Arduino IDE**
2. Add **ESP8266 Board Manager**
3. Install required libraries:
   - ESP8266WiFi
   - ESP8266WebServer
   - MPU6050
   - TinyGPS++
4. Upload the `.ino` code to ESP8266.

### Dashboard Setup
1. Place HTML, CSS, JS files in the ESP SPIFFS or host locally.
2. Connect ESP to Wi-Fi.
3. Open ESP IP address in browser.
4. View live dashboard.

---

## Future Improvements
- Cloud database integration
- SMS/Call alert system
- Mobile app support
- AI-based crash prediction
- Vehicle speed analytics

---

## Applications
- Smart vehicles
- Bike safety systems
- Fleet monitoring
- Emergency response automation

---


