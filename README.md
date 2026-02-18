# 🏎️ Autonomous Smart-Escape Rover

![C++](https://img.shields.io/badge/Language-C++-blue.svg)
![Platform](https://img.shields.io/badge/Platform-Arduino-00979D.svg)
![Status](https://img.shields.io/badge/Status-Active-success.svg)

An advanced, multi-sensor autonomous vehicle built on the Arduino platform. Unlike basic obstacle-avoiding robots that get trapped in corners, this rover features **Active-Escape Logic**, dynamic braking, and a synchronized 4-LED lighting system to visually communicate its state machine.

## 🎯 Problem Statement & Solution
Standard obstacle-avoiding robots suffer from "deadlock" in narrow spaces and lack momentum control, causing them to crash before sensors can react. 

**The Solution:** This project implements a multi-layered reflex system:
1. **IR "Whiskers" (Layer 1):** Dual infrared sensors act as high-speed reflexes to instantly dodge side-on collisions without stopping to scan.
2. **Ultrasonic Radar (Layer 2 & 3):** A servo-mounted ultrasonic sensor provides both strict emergency braking (<16cm) and dynamic speed mapping (16cm-50cm), smoothly decelerating the rover as it approaches an obstacle.
3. **State-Aware UI:** Synchronized headlights, brake lights, turn signals, and acoustic warnings (buzzer) provide real-time diagnostic feedback to the observer.

## ⚙️ Hardware Architecture

### Components Used
* Arduino Uno R3
* L298N Motor Driver Module
* HC-SR04 Ultrasonic Distance Sensor
* 180° Micro Servo Motor (SG90)
* 2x Infrared (IR) Proximity Sensors
* 1x Active Buzzer
* 4x LEDs (2x White for Front, 2x Red for Rear)
* 4x DC Gear Motors & Chassis

### Pin Configuration
| Component | Arduino Pin | Description |
| :--- | :--- | :--- |
| **Motors (L298N)** | 5, 6, 7, 8, 9, 4 | ENA, ENB, IN1, IN2, IN3, IN4 |
| **Ultrasonic** | 2, 3 | Trig, Echo |
| **IR Sensors** | A0, A1 | Left IR, Right IR |
| **Servo Motor** | 13 | Rotating Sensor Head |
| **Headlights** | 10, 11 | White LED Left, White LED Right |
| **Taillights** | A2, A3 | Red LED Left, Red LED Right |
| **Buzzer** | 12 | Audio Alerts |

## 🚀 Key Features
* **Dynamic Braking:** Uses the `map()` function to proportionally reduce motor PWM speed based on ultrasonic proximity, acting like physical brakes.
* **Hazard Light Sync:** Taillights flicker at a dynamically increasing rate as the rover approaches a wall, mimicking a parking sensor.
* **Anti-Yo-Yo Escape:** If trapped in a dead end, the rover executes a synchronized reverse and pivot maneuver, preventing it from looping the same actions.
* **Audio-Visual Profiles:** Distinct melodies and LED patterns for Startup, Cruising, Dodging, and Trapped states.

## 💻 Installation & Usage
1. Clone this repository: `git clone https://github.com/PradhyotKumar/Autonomous-Smart-Escape-Rover.git`
2. Open the `.ino` file in the Arduino IDE.
3. Ensure the `<Servo.h>` library is installed (included by default in the Arduino IDE).
4. Verify your IR sensors are calibrated (adjust the onboard potentiometer so the LED only triggers when an obstacle is <10cm away).
5. Compile and upload the code to your Arduino. 

## 🔮 Future Roadmap
This hardware and base logic serve as a foundation for implementing higher-level control systems:
* **AI/ML Integration:** Replacing if/else logic with a Reinforcement Learning (RL) agent to allow the rover to "learn" the most efficient escape routes.
* **Computer Vision:** Integrating a Python-based dashboard via serial/Bluetooth to stream telemetry and process basic camera feeds.
* **PID Control:** Implementing Proportional-Integral-Derivative control for smoother, curved trajectories rather than stop-and-spin turning.

---
*Developed by PARVEEN KUMAR DHAMI,POOJA UMESH HINDULE, Pradhyot Kumar*
