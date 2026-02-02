# Power Analysis Capability

## Purpose
The power analysis module provides real-time monitoring of target device power consumption using fast ADC sampling. This enables side-channel attacks where cryptographic secrets can be extracted by analyzing power consumption patterns during cryptographic operations.

## Requirements

### Requirement: Fast ADC Initialization
The system SHALL initialize a fast ADC driver on the AMP_IN_PIN with 12-bit resolution and 11dB attenuation for power measurement.

#### Scenario: ADC setup on boot
- **WHEN** the system starts up
- **THEN** the fast ADC is configured on pin 4 (AMP_IN_PIN) with calibration applied

#### Scenario: ADC calibration
- **WHEN** ADC is initialized
- **THEN** hardware calibration coefficients are applied for accurate voltage readings

### Requirement: Real-Time ADC Streaming
The system SHALL stream ADC samples to the web interface via WebSocket for real-time power consumption visualization.

#### Scenario: Enable streaming mode
- **WHEN** user enables amp bias streaming via web interface
- **THEN** the system continuously samples ADC and sends values over WebSocket

#### Scenario: Streaming rate control
- **WHEN** streaming is active
- **THEN** samples are sent at a configurable rate (default 100ms intervals)

### Requirement: Sample Buffering
The system SHALL maintain sample buffers for differential power analysis across multiple key operations.

#### Scenario: Buffer allocation
- **WHEN** performing differential analysis
- **THEN** three 1000-sample buffers are available for comparing power traces from different key inputs

#### Scenario: Buffer capture on key press
- **WHEN** a key input pin (KEY_1, KEY_2, or KEY_3) is activated
- **THEN** the corresponding buffer captures 1000 ADC samples during the operation

### Requirement: Differential Power Analysis
The system SHALL support differential power analysis by comparing power traces from operations with different input keys.

#### Scenario: Run key difference analysis
- **WHEN** user initiates key difference analysis
- **THEN** the system compares buffered traces to identify power consumption differences correlated with key bits

### Requirement: Voltage Conversion
The system SHALL convert raw ADC values to calibrated voltage readings using DAC-to-voltage conversion constants.

#### Scenario: ADC to voltage conversion
- **WHEN** displaying or transmitting ADC readings
- **THEN** raw values are converted to millivolts using calibration polynomial
