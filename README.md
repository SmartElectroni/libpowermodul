# libmodul

Copyright (c) 2025 SmartElectroni

### English

#### Overview
This C++ library provides functionality for generating and parsing CAN frames for different power supply protocols (UUgreen and MMeet). It implements the Strategy pattern to support multiple protocols with a unified interface.

#### Features
- CAN frame generation for various commands:
  - Temperature/voltage/current readings
  - Mode control (low/high/auto)
  - Voltage/current settings
  - Power on/off commands
- Support for multiple protocols:
  - UUgreen
  - MMeet
- CAN frame parsing with protocol detection
- Cross-platform (requires C++17)

#### Usage
1. Include the header in your project:
```cpp
#include "libmodul.h"
```

2. Create protocol manager:
```cpp
CanProtocolManager manager(ProtocolType::MMeet); // or ProtocolType::UUgreen
```

3. Generate frames:
```cpp
auto frame = manager.generateVoltageRequest(0x01); // Request voltage from device 0x01
```

4. Parse incoming frames:
```cpp
CanParser parser;
auto [data, result] = parser.parse(received_frame, ProtocolType::MMeet);
if (result == ParseResult::OK && data) {
    // Use parsed data
}
```

#### Building
The library is header-only. Just include the header file in your project.

#### Protocol Support
| Feature           | UUgreen | MMeet |
|-------------------|---------|-------|
| Temperature read  | ✔       | ✔     |
| Voltage read      | ✔       | ✔     |
| Current read      | ✔       | ✔     |
| Voltage set       | ✔       | ✔     |
| Current set       | ✔       | ✔     |
| Low/High mode     | ✔       | ✔     |
| Auto mode         | ✖       | ✔     |
| Power control     | ✔       | ✔     |

#### License
MIT License
