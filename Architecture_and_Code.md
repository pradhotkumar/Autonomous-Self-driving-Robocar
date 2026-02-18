## 1. High-Level System Architecture

The following diagram illustrates the data flow between the physical hardware, the telemetry bridge, and the Python server.

```mermaid
graph TD
    subgraph PhysicalLayer [Physical Layer: Rover]
        A[Ultrasonic Scanner] -->|Front Distance| C(Arduino Uno R3)
        B[Dual IR Sensors] -->|Lateral Proximity| C
        C -->|Motor PWM Signals| D[L298N Motor Driver]
        C -->|UI Sync| E[4-LED System & Buzzer]
        D --> F[4x DC Gear Motors]
    end

    subgraph TelemetryBridge [Telemetry Bridge: HC-05]
        C <-->|9600 Baud Serial Stream| G((Bluetooth TX/RX))
    end

    subgraph ComputationalLayer [Computational Layer: AI Server]
        G <-->|PySerial Connection| H[telemetry_dashboard.py]
        H -->|State Tensor| I[DQN RL Model]
        I -->|Discrete Action Command| H
    end
