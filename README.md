
# 🧊 Predictive Maintenance Cold Storage ❄️
---

## 📌 Project Overview

**Cold storage failures** can lead to spoilage of dairy and perishable goods. Our system monitors environmental parameters using IoT sensors and leverages Machine Learning for **real-time anomaly detection and predictive maintenance**.

---

## 🎯 Objectives

- Monitor critical cold storage parameters (temperature, humidity).
- Detect anomalies using machine learning models.
- Provide real-time alerts to prevent spoilage.
- Enable predictive maintenance through trend analysis.

---
## Working

![alt text](photo/Working.jpeg)

---
## 🛠️ Tech Stack

### ⚙️ Hardware
- **ESP32** – Microcontroller with Wi-Fi
- **DHT11** – Temperature & Humidity sensor
- **OLED Display** – For real-time sensor data
- **Jumper wires, Breadboard, USB cable**

### 💻 Software
- **Arduino IDE** – For firmware development
- **Python (scikit-learn, pandas)** – For ML model training and inference
- **Flask** – Backend for ML integration
- **Thingspeak** - For cloud analytics

---

## 🧠 Machine Learning

- **Model Used**: Random Forest 
- **Target**: Binary classification of "Normal" vs "At Risk"
- **Training Data**: Simulated + real sensor logs
- **Features**: Temperature, Humidity, Gas Index, Time-of-Day
- **Output**: Risk score + alert flag

---

## 📈 Dashboard
![alt text](photo/Ui.jpeg)

---

## 📷 Prototype Snapshot

![alt text](photo/Prototype.jpeg)

---

## 🚀 Getting Started

### 1. Clone the Repository
```bash
git clone https://github.com/Subhashbisnoi/predictive_maintenance_cold_storage.git
cd predictive_maintenance_cold_storage
```

### 2. Upload Arduino Code
- Connect ESP32 to your system
- Open `cold_storage_iot.ino` in **Arduino IDE**
- Install necessary libraries: `DHT`, `Adafruit_Sensor`, `Wire`, `Adafruit_GFX`, `Adafruit_SSD1306`
- Upload the code

### 3. Train & Test ML Model
```bash
cd ml_model
pip install -r requirements.txt
python train_model.py
python test_model.py
```

### 4. Start Flask Backend (Optional)
```bash
cd flask_backend
python app.py
```

---

## 🧪 Features

- ✅ Real-time sensor data acquisition
- ✅ ML-based anomaly detection
- ✅ On-device display of readings
- ✅ Expandable backend with cloud integration
- ✅ Risk alerts for preventive action

---

## 📜 License

MIT License © 2025 [Subhash Bishnoi](https://www.linkedin.com/in/subhash-bishnoi-a068a42b1/)

---

## 🙌 Acknowledgements

- IIITDM Kancheepuram
- IoT Lab & Faculty Mentors

