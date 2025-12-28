# Smart Navigation Cane – IoT Project

## Overview
The Smart Navigation Cane is an IoT-based safety and navigation aid for visually impaired individuals.
It detects nearby obstacles using ultrasonic sensors and provides vibration alerts.
In emergency situations, an SOS button sends the user's real-time GPS location to a web dashboard.

## Features
- Obstacle detection using ultrasonic sensor
- Vibration motor alert when an object is within 30 cm range
- SOS button for emergency assistance
- Real-time location tracking using GPS (NEO-6M)
- Location data sent to server using Flask
- Live location visualization using ThingSpeak and Google Maps API

## Hardware Components
- ESP32 Microcontroller
- Ultrasonic Sensor
- GPS Module (NEO-6M)
- Vibration Motor
- SOS Push Button
- Battery and Power Module

## Software & Technologies
- Arduino IDE
- ESP32 Firmware
- Flask (Python Backend)
- ThingSpeak Cloud Platform
- Google Maps API
- HTTP / REST APIs

## Working
1. Ultrasonic sensor continuously measures distance.
2. If an object comes within 30 cm, the vibration motor is activated.
3. GPS module continuously fetches real-time location.
4. When SOS button is pressed:
   - Location data is sent to Flask server
   - Data is stored and visualized using ThingSpeak
   - Location is shown on Google Maps

## Use Case
- Assists visually impaired users in safe navigation
- Enables emergency location sharing
- Useful for elderly and physically challenged individuals

## Project Type
IoT-Based Assistive Device  
Academic / Prototype Project
