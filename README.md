# 🚒 ObstacleFire-Robot
An intelligent autonomous system for real-time fire detection and suppression, featuring obstacle avoidance.

## 🧠 Smart Features
* **Tri-Directional Flame Sensing:** Uses 3 IR sensors with an analog smoothing algorithm for precise fire localization.
* **Dynamic Servo Spraying:** Implements a `shakeServo()` function to oscillate the water hose for maximum coverage.
* **Intelligent Obstacle Avoidance:** Integrated Ultrasonic HC-SR04 logic to navigate safely within 20cm of obstacles.

## 📸 Project Preview
![Robot Image](rproject.png)

## 🛠️ Hardware Setup
* **Microcontroller:** Arduino Uno.
* **Sensors:** Ultrasonic HC-SR04, 3x IR Flame Sensors.
* **Drive:** L298N Motor Driver with dual DC motors.
* **Suppression:** Water Pump via Relay and Servo-mounted hose.
