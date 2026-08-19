# IoT-Based Smart Cold Storage Monitoring System #

## 1. Project Overview

This project is an IoT-based Smart Cold Storage Monitoring System developed using ESP32.
The system monitors temperature, humidity, and gas levels inside a storage environment. When the monitored parameters exceed predefined threshold values, the system activates a relay and buzzer and sends an alert notification.
The sensor data is also uploaded to ThingSpeak for remote monitoring.

## 2. Objective

The main objective of this project is to:

- Monitor temperature and humidity continuously.
- Monitor gas levels using the MQ-135 sensor.
- Detect abnormal environmental conditions.
- Automatically control the relay based on temperature and gas conditions.
- Provide local alerts using a buzzer.
- Display sensor values using a 16x2 I2C LCD.
- Upload sensor data to ThingSpeak.
- Send alerts through Pushover.
- Send a heartbeat value to indicate system activity.

---

## 3. Hardware Components

- ESP32 Development Board
- DHT11 Temperature and Humidity Sensor
- MQ-135 Gas Sensor
- 16x2 I2C LCD
- Relay Module
- Buzzer
- Jumper Wires
- Power Supply

---

## 4. Software and Technologies

- Arduino IDE
- Embedded C/C++
- ESP32
- Wi-Fi
- ThingSpeak
- Pushover
- I2C Communication

---

## 5. Pin Configuration

| Component | ESP32 Pin |
|---|---:|
| DHT11 Data | GPIO 4 |
| MQ-135 Analog Output | GPIO 34 |
| Buzzer | GPIO 25 |
| Relay | GPIO 26 |
| I2C LCD SDA | I2C SDA |
| I2C LCD SCL | I2C SCL |

---

## 6. System Working

The ESP32 reads data from the DHT11 and MQ-135 sensors.

The DHT11 provides:

- Temperature
- Humidity

The MQ-135 provides an analog gas sensor value.

The ESP32 compares the sensor values with predefined threshold values.

If an abnormal condition is detected, the corresponding output is activated.

The sensor values and system status are also uploaded to ThingSpeak.

---

## 7. Threshold Configuration

| Parameter | Threshold |
|---|---:|
| Temperature | 37 °C |
| Gas Level | 1500 |
| Low Humidity | 40 % |

These values can be modified in the Arduino code according to the application requirements.

---

## 8. Relay Logic

The relay is activated when either of the following conditions occurs:

- Temperature exceeds the temperature threshold.
- Gas value exceeds the gas threshold.

Logic:

`Relay = Temperature High OR Gas Alert`

The relay remains OFF when both conditions are normal.

---

## 9. Buzzer Logic

The buzzer is activated when any abnormal condition occurs:

- Gas level is high.
- Temperature is high.
- Humidity is below the configured limit.

Logic:

`Buzzer = Gas Alert OR Temperature High OR Low Humidity`

---

## 10. LCD Display

The 16x2 I2C LCD displays:

- Temperature
- Humidity
- Gas value
- Current alert status

Example status messages:

- OK
- GAS!
- TEMP!
- HUM!

---

## 11. ThingSpeak Cloud Monitoring

The system sends sensor and system status information to ThingSpeak.

The following fields are used:

| ThingSpeak Field | Data |
|---|---|
| Field 1 | Temperature |
| Field 2 | Humidity |
| Field 3 | Gas Value |
| Field 4 | Buzzer Status |
| Field 5 | Relay Status |
| Field 6 | Heartbeat |

This allows the sensor data to be monitored remotely.

---

## 12. Pushover Alert System

Pushover is used to send notifications when an abnormal condition is detected.

The system can generate alerts for:

- High gas level
- High temperature
- Low humidity

An alert interval is implemented to prevent notifications from being sent continuously.

---

## 13. Heartbeat Monitoring

A heartbeat value is sent to ThingSpeak to indicate that the ESP32 system is active and communicating with the cloud platform.

The heartbeat value used in the current implementation is:
`1`

---

## 14. How to Run the Project

### Step 1

Install Arduino IDE.

### Step 2

Install the required ESP32 board support.

### Step 3

Install the required libraries:

- WiFi
- Wire
- LiquidCrystal_I2C
- DHT
- ThingSpeak
- Pushover

### Step 4

Open:

`Code/cold_storage_monitoring.ino`

### Step 5

Configure your own Wi-Fi credentials.

### Step 6

Configure your own ThingSpeak channel and API key.

### Step 7

Configure your own Pushover credentials.

### Step 8

Connect the hardware according to the pin configuration.

### Step 9

Select the correct ESP32 board and COM port in Arduino IDE.

### Step 10

Upload the program to the ESP32.

### Step 11

Open Serial Monitor at:

`115200 baud`

### Step 12

Verify sensor readings, relay operation, buzzer operation, LCD display, ThingSpeak data and notifications.

---

## 15. Security Note

This repository does not contain real Wi-Fi passwords, ThingSpeak API keys, or Pushover credentials.

Users must configure their own credentials before running the project.

Never publish private credentials or API keys in a public repository.

---

## 16. Future Improvements

Possible future improvements include:

- Add more environmental sensors.
- Add automatic cooling control improvements.
- Add mobile application monitoring.
- Add data logging and historical analysis.
- Add sensor fault detection.
- Add automated testing.
- Add firmware update capability.
- Improve power management.

---

## 17. Project Status

Current status: Working prototype.

The system has been Developed using ESP32 for IoT-based cold storage monitoring.
