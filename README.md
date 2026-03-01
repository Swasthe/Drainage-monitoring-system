# Smart drainage monitoring system

An IoT-based real-time drainage monitoring solution designed to detect blockages and hazardous gases while ensuring worker safety through a web-based dashboard.

**Features**

  Real-Time Dashboard: A web interface served directly from the ESP32 with a 3-second auto-refresh rate.
  
  Multi-Mode Simulation: Toggle between Normal, Blockage + Gas, and Blockage Only modes to test system response.
  
  Live Sensor Data: Monitors water flow (L/min), water level (cm), and gas concentration levels.
  
  Safety Alerts: Visual indicators for blockages and harmful gas detection.
  
  Worker Protection: Integrated "Worker Status" tracking that triggers an Immediate Danger alert if gas is detected while a worker is inside.

**Hardware Requirements**

  The system is configured for the ESP32 development board with the following pin assignments:
  
  **Component	ESP32 Pin	Description**
  
  Flow Sensor	GPIO 26	
  Measures water flow rate.
  
  Ultrasonic (Trig)	GPIO 5	
  Trigger pin for water level sensing.
  
  Ultrasonic (Echo)	GPIO 18	
  Echo pin for water level sensing.
  
  Gas Sensor (AO)	GPIO 34	
  Analog input for gas concentration.
  
  Gas Sensor (DO)	GPIO 25	
  Digital alert pin for gas detection.

**Software Setup**

  WiFi Configuration: Update the ssid and password variables in the code to match your local network.
  
  Dependencies: Ensure the WiFi.h and WebServer.h libraries are installed in your Arduino IDE.

  Deployment: Upload the sketch to your ESP32 and open the Serial Monitor (115200 baud) to find the local IP address.
  
  Access: Enter the IP address into any web browser on the same network to view the dashboard.

**Project Structure**

  handleRoot(): Generates the dynamic HTML/CSS dashboard with live readings and color-coded alerts.
  
  simulateValues(): Logic for generating test data based on the selected operational mode.
  
  handleSetMode() / handleToggleWorker(): Server endpoints to process user interaction from the web UI.

**Safety Logic**

The system uses a critical safety check: if workerInside is true and gasDetected is true, the dashboard displays a high-priority "Immediate Danger!" alert to signal evacuation.
