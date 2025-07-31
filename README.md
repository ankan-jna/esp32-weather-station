# Mini ESP32 Weather Station

A self-hosted, Wi-Fi-AP–based weather station using an ESP32 and DHT11 sensor.  
Delivers a dark-mode dashboard with color-coded donut charts.

## 📋 Hardware
- ESP32 NodeMCU-32S board
- DHT11 (3-pin) temperature & humidity sensor
- Jumper wires
- USB-Micro cable

## 🔧 Wiring
- DHT11 VCC → 3V3
- DHT11 GND → GND
- DHT11 DATA → GPIO 5

## 🛠️ Software
- Arduino IDE
- Board package: `esp32 by Espressif Systems`
- Libraries: Adafruit DHT sensor, Adafruit Unified Sensor

## 🚀 Usage
1. Clone repo  
   ```bash
   git clone https://github.com/ankan-jna/mini-weather-station.git
   cd mini-weather-station
