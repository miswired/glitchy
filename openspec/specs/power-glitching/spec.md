# Power Glitching Capability

## Purpose
The power glitching module provides controlled voltage glitch attacks against target devices. It uses an SPI-driven FET to create precise nanosecond-scale power disruptions that can cause instruction skips or data corruption in target microcontrollers.

## Requirements

### Requirement: Glitch Parameter Configuration
The system SHALL allow configuration of glitch timing parameters including shortest delay, longest delay, step size, pause time between attempts, and number of attempts per step.

#### Scenario: Configure glitch parameters via web interface
- **WHEN** user submits new glitch parameters through the web UI
- **THEN** the system updates the internal glitch_param_t structure with the new values

#### Scenario: Default parameter initialization
- **WHEN** the system starts up
- **THEN** glitch parameters are initialized to safe defaults (200ns shortest, 1500ns longest, 10ns step, 3 attempts)

### Requirement: SPI-Driven Glitch Execution
The system SHALL use the FSPI bus to drive the power glitch FET with nanosecond-level timing precision.

#### Scenario: Execute single glitch pulse
- **WHEN** a glitch is triggered
- **THEN** the system sends an SPI transaction to create a precise voltage dip on the target power rail

#### Scenario: Glitch width control
- **WHEN** executing a glitch with a specified width
- **THEN** the FET remains active for the duration specified (within SPI timing constraints)

### Requirement: Automated Timing Sweep
The system SHALL automatically sweep through timing parameters to find vulnerable glitch windows.

#### Scenario: Parameter sweep execution
- **WHEN** user activates glitching via web interface
- **THEN** the system iterates from shortest_delay_ns to longest_delay_ns in step_size increments, executing num_of_attempts glitches at each timing

#### Scenario: Success detection during sweep
- **WHEN** the GLITCH_SUCCESS_PIN goes high during a sweep
- **THEN** the system records the successful timing parameters and reports via WebSocket

### Requirement: Target Trigger Synchronization
The system SHALL synchronize glitch timing with target device operations using the ENTER_KEY_PIN.

#### Scenario: Trigger target before glitch
- **WHEN** executing a glitch sequence
- **THEN** the system pulses ENTER_KEY_PIN to initiate the target operation before applying the glitch

### Requirement: Glitch Status Reporting
The system SHALL report glitch progress and results via WebSocket to the web interface.

#### Scenario: Progress updates during sweep
- **WHEN** a glitch sweep is in progress
- **THEN** the system sends periodic status updates including current timing and attempt count

#### Scenario: Success notification
- **WHEN** a glitch successfully bypasses target security
- **THEN** the system sends a success message with the effective timing parameters
