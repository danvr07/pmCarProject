# 🚗 ESP32 Smart Car with Bluetooth & Obstacle Avoidance

## 📌 Project Overview
This project is an **ESP32-based smart car** controlled via the **Dabble mobile app (Bluetooth)**.  
It supports both **manual driving** and an **automatic obstacle & cliff avoidance mode**.

### 🔧 Hardware Used
- ESP32  
- L298N motor driver (or similar)  
- Ultrasonic sensor (HC-SR04)  
- IR sensor for cliff/edge detection  
- Servo motor  
- I2C LCD 16x2  
- DC motors with wheels  

---

## ✨ Features
- **Manual control** using the Dabble gamepad  
- **Automatic mode**: avoids obstacles and cliffs  
- **LCD feedback**: shows connection, mode, and alerts  
- **Movements**: forward, backward, left, right  

---

## 🎮 Controls (Dabble Gamepad)
- 🔼 **Up** → Move forward  
- 🔽 **Down** → Move backward  
- ◀️ **Left** → Turn left  
- ▶️ **Right** → Turn right  
- 🔺 **Triangle** → Enable obstacle + cliff avoidance mode  
- ❌ **Cross** → Disable automatic mode (manual control)  

---

## 🖼️ Demo
_Add here some photos or a GIF with your robot in action._

---

## 📜 License
This project is open-source. You can use and modify it freely.
