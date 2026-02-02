# Project Context

## Purpose
Glitchy (aka "CircuitBreakerGadget") is an open-source hardware hacking educational kit designed to teach hardware exploitation techniques. The project demonstrates two critical hardware attack methodologies:

1. **Power Glitching**: Introducing controlled power glitches to alter program execution flow
2. **Power Analysis**: Extracting secrets by monitoring power consumption patterns (side-channel attacks)

The project aims to be beginner-friendly while demonstrating real-world security vulnerabilities in embedded systems. It serves security researchers, embedded systems engineers, hardware enthusiasts, and students learning hardware exploitation.

## Tech Stack

### Hardware Platform
- **MCU**: ESP32-S3-DevKitC-1 (primary controller)
- **Power Control**: High-speed FET (Field Effect Transistor) for crowbar glitching
- **Analog Monitoring**: ADC circuits for power analysis
- **Communication**: WiFi connectivity
- **Target Device**: Arduino Uno R3 (for educational labs)

### Firmware Development
- **Build System**: PlatformIO
- **Framework**: Arduino
- **Language**: C/C++
- **IDE**: VS Code with PlatformIO extension (recommended)

### Core Libraries
- AsyncTCP - TCP/IP stack for ESP32
- ESPAsyncWebServer - Async web server for HTTP/WebSocket
- ArduinoJson (v7.3.1+) - JSON communication
- Custom Fast ADC driver (ESP32-S3-FastAnalogRead)

### Frontend
- Vue.js-based web application (separate repo: https://github.com/pkopysci/miswired-glitchy-vue-app)
- Served from SD card via async web server
- Real-time WebSocket communication

### PCB Design
- KiCad EDA for schematics and layouts

## Project Conventions

### Code Style
- C/C++ with Arduino conventions
- Function names: snake_case (e.g., `init_glitch()`, `setup_fast_adc()`)
- Constants/Macros: UPPER_SNAKE_CASE (e.g., `POWER_GLITCH_PIN`, `AMP_IN_PIN`)
- Structs: snake_case with `_t` suffix (e.g., `glitch_param_t`)
- Global state variables: prefixed with `g_` (e.g., `g_glitching_activate`)

### Architecture Patterns
- **Client-Controller Architecture**: ESP32-S3 board controls glitching, target device receives attacks
- **Event-Driven**: Timer interrupts for periodic tasks, async web server for non-blocking I/O
- **Modular Design**: Separate modules for glitching, analog, and web server functionality
- **State Management**: Global getter/setter pairs for cross-module state (e.g., `get_glitching_activate()`, `set_glitching_activate()`)

### File Organization
```
Code/Glitchy/
├── src/              # Implementation files (.cpp)
│   ├── main.cpp      # Entry point, WiFi, SD card init
│   ├── glitching.cpp # Glitch attack logic
│   ├── webserver.cpp # Web interface and WebSocket
│   └── analog_helpers.cpp # Power analysis ADC functions
├── include/          # Header files (.h)
│   ├── pins.h        # Pin definitions
│   ├── global.h      # Shared definitions
│   ├── glitching.h   # Glitch module interface
│   ├── web_server.h  # Web server interface
│   └── analog.h      # Fast ADC driver
└── platformio.ini    # Build configuration
```

### Testing Strategy
- Manual testing with physical hardware setup (Glitchy board + Arduino Uno target)
- Web UI testing via browser
- No automated unit tests currently (hardware-dependent codebase)

### Git Workflow
- Main branch contains stable releases
- Feature branches for development
- Version tags for releases (e.g., v2.0.0)

## Domain Context

### Hardware Security Concepts
- **Power Glitching**: Momentarily dropping power to a chip causes instruction skips or data corruption, potentially bypassing security checks
- **Crowbar Circuit**: Uses FET to short power rails momentarily, creating voltage dips
- **Timing Window**: Glitches must occur at precise moments (nanosecond precision) to be effective
- **Power Analysis**: Measuring power consumption can reveal information about data being processed (side-channel attack)

### Key Parameters
- **Glitch Width**: Duration of power dip (typically 200ns - 1500ns)
- **Glitch Timing**: When to trigger relative to target operation
- **Sweep Parameters**: Iteratively test timing ranges to find vulnerable windows

### Pin Mapping (ESP32-S3)
| Pin | Function | Description |
|-----|----------|-------------|
| 15  | POWER_GLITCH_PIN | FET control for glitching |
| 11  | ENTER_KEY_PIN | Trigger target device |
| 12  | GLITCH_SUCCESS_PIN | Monitor attack success |
| 4   | AMP_IN_PIN | Amplified power measurement |
| 2   | BIAS_IN_PIN | Bias voltage input |
| 5,6,7 | KEY_1/2/3_PIN | Key inputs for differential analysis |

## Important Constraints

### Hardware Constraints
- Timing precision limited by SPI bus speed (~18ns minimum glitch width)
- ADC resolution: 12-bit with 11dB attenuation
- SD card required for web GUI files
- WiFi connectivity required for web interface

### Safety Constraints
- Educational use only - not for malicious purposes
- Glitching can damage target devices if misconfigured
- Power analysis requires proper signal conditioning

### Compatibility
- Requires ESP32-S3 specifically (not ESP32 or ESP32-S2)
- Target examples designed for Arduino Uno R3
- Web GUI requires modern browser with WebSocket support

## External Dependencies

### Hardware
- ESP32-S3-DevKitC-1 development board
- Custom Glitchy PCB (KiCad files in PCB/ directory)
- Arduino Uno R3 (for target labs)
- SD card for web GUI storage

### Software/Services
- PlatformIO for firmware builds
- Web GUI: https://github.com/pkopysci/miswired-glitchy-vue-app
- No cloud services required - fully offline capable

### Documentation
- Wiki: https://github.com/miswired/glitchy/wiki
- Theory, quick start guides, and lab tutorials available

## Collaborators
- **Miswired** (https://github.com/miswired): PCB design and firmware
- **Philip J Kopyscinski** (https://github.com/pkopysci): Web GUI implementation
