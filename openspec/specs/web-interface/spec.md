# Web Interface Capability

## Purpose
The web interface module provides a browser-based control panel for the Glitchy device. It uses an async HTTP server with WebSocket support for real-time communication. The frontend Vue.js application is served from an SD card.

## Requirements

### Requirement: Async HTTP Server
The system SHALL host an async HTTP web server on port 80 for serving the web GUI and handling API requests.

#### Scenario: Server initialization
- **WHEN** the system starts and WiFi is connected
- **THEN** an async HTTP server starts on port 80

#### Scenario: Static file serving
- **WHEN** a browser requests files (HTML, CSS, JS)
- **THEN** the server serves files from the SD card root directory

### Requirement: WebSocket Communication
The system SHALL provide a WebSocket endpoint at /ws for real-time bidirectional communication with the web GUI.

#### Scenario: WebSocket connection
- **WHEN** the web GUI connects to /ws
- **THEN** a persistent WebSocket connection is established

#### Scenario: Receive commands via WebSocket
- **WHEN** the web GUI sends a JSON command
- **THEN** the server parses and executes the command (e.g., start glitching, update parameters)

#### Scenario: Send updates via WebSocket
- **WHEN** glitch status changes or ADC data is available
- **THEN** the server pushes JSON updates to connected clients

### Requirement: WiFi Connectivity
The system SHALL support both Access Point (AP) mode and client (station) mode for WiFi connectivity.

#### Scenario: Default AP mode
- **WHEN** the system starts without stored WiFi credentials
- **THEN** it creates an access point for direct connection

#### Scenario: Client mode connection
- **WHEN** valid WiFi credentials are configured
- **THEN** the system connects to the specified network as a client

### Requirement: JSON API Protocol
The system SHALL use JSON format for all WebSocket message exchanges with defined message types for commands and responses.

#### Scenario: Glitch parameter update message
- **WHEN** sending glitch parameters to the GUI
- **THEN** the message contains all timing parameters in a structured JSON object

#### Scenario: ADC streaming message
- **WHEN** streaming power analysis data
- **THEN** ADC values are sent as JSON arrays with timestamp information

### Requirement: State Synchronization
The system SHALL maintain synchronized state between firmware and web GUI using getter/setter functions and WebSocket updates.

#### Scenario: GUI requests current state
- **WHEN** the web GUI connects or requests state
- **THEN** the server sends current glitch parameters and system status

#### Scenario: State change notification
- **WHEN** firmware state changes (e.g., glitching starts/stops)
- **THEN** connected clients receive immediate notification
