```markdown
# 🌱 Moisture Minder: IoT Smart Irrigation System

An automated IoT-based smart gardening and irrigation system built using an **ESP32 microcontroller**, **Firebase Realtime Database**, and a custom **Kodular Android mobile application**[cite: 1, 3, 7]. The system continuously monitors soil moisture and reservoir water levels to automate water pump control, preventing overwatering and dry-running[cite: 1, 3].

---

## 🚀 Key Features

* **Real-Time Soil Moisture Sensing:** Continuously measures soil conductivity to determine moisture levels.


* **Dry-Run Protection:** Uses an ultrasonic sensor (`HC-SR04`) to check reservoir water distance before operating the pump.


* **Automated Pump Control:** Actuates a relay module to toggle the water pump automatically based on threshold limits.


* **Cloud Sync & Mobile Dashboard:** Logs telemetry to Firebase Realtime Database, monitored via a custom Kodular mobile app.



---

## 🛠️ Tech Stack & Components

* **Microcontroller:** ESP32 (GISMO-VI Board)


* **Sensors:** Soil Moisture Sensor, HC-SR04 Ultrasonic Sensor


* **Actuator:** Single-Channel Relay Module & Water Pump


* **Firmware:** Arduino IDE (C++, `WiFi.h`, `FirebaseESP32.h`)


* **Cloud & App:** Google Firebase Realtime Database, Kodular Creator



---

## 🔌 Pin Configuration

![alt text](<pin config.png>)

---

## ⚙️ How It Works

1. **Data Acquisition:** The ESP32 reads analog moisture data from `GPIO 34` and calculates water level distance via the ultrasonic sensor.


2. **Threshold Logic:**
* **Dry Soil (`smvalue < 10`):** Relay turns the pump ON (`val = -1`).


* **Optimal Soil (`10 <= smvalue <= 300`):** Pump stays OFF (`val = 0`).


* **Excess Water (`smvalue > 300`):** Pump stays OFF (`val = 1`).




3. **Cloud Sync:** Status and sensor readings update Firebase every second (`IOTLAB/smart_garden/`), syncing directly to the mobile app.



---

## 📸 Mobile App Preview 

###  Kodular Mobile App Interface & Logic

![alt text](<designer view.png>)
![alt text](<block view.png>)

## Hardware Prototype & Live Testing
![alt text](<deployement.png>)


```
