## ⚙️ Autonomous State Machine Algorithm

```mermaid
graph TD
    Start([Poll Sensors]) --> Layer1{IR Whiskers Blocked?}
    
    Layer1 -->|Yes: Both| Esc[Layer 1: Trapped U-Turn]
    Layer1 -->|Yes: Left/Right| Dodge[Layer 1: Instant Dodge]
    
    Layer1 -->|No| Layer2{Ultrasonic < 16cm?}
    
    Layer2 -->|Yes| Stop[Layer 2: Magnetic Brake & Scan]
    
    Layer2 -->|No| Layer3{Ultrasonic 16cm - 60cm?}
    
    Layer3 -->|Yes| Dynamic[Layer 3: Dynamic Braking & Hazard UI]
    
    Layer3 -->|No| Layer4[Layer 4: Full Speed Cruising]
