# Smart Aquaculture System for Micro Fish Tanks

This project is a comprehensive, solar-powered IoT system designed to automate and monitor a small 5-8 liter aquarium. It uses an ESP32 microcontroller and the Blynk IoT platform to provide real-time data, remote control, and automated actions, making smart aquaculture accessible and affordable.



## ✨ Features
- **Real-Time Monitoring:** Live tracking of water temperature, turbidity, and ambient ammonia levels.
- **Automated Actuator Control:** Scheduled fish feeding via a servo and intelligent, time-based lighting controlled by a relay and synced with internet time.
- **Remote Control & Visualization:** A custom dashboard on both web and mobile platforms using Blynk for viewing sensor data and manually controlling the lights and feeder.
- **Alerts & Notifications:** Instant push notifications for critical events like low food, out-of-range sensor values, or low water levels.
- **Off-Grid Power System:** A self-sufficient power system using a solar panel, an 18650 Li-ion battery, and a boost converter for continuous operation.

## Hardware Requirements

| Component               | Quantity | Purpose                             |
----------------------------------------------------------------------------
| ESP32 Dev Kit           | 1        | Microcontroller with Wi-Fi          |
| DS18B20 Temp Sensor     | 1        | Water temperature measurement       |
| Turbidity Sensor        | 1        | Water clarity monitoring            |
| MQ-135 Gas Sensor       | 1        | Ammonia/Air Quality monitoring      |
| SG90 Servo Motor        | 1        | Automated feeding mechanism         |
| 5V Relay Module         | 1        | Controls power to the LED strip     |
| 5V Waterproof LED Strip | 1m       | Aquarium lighting                   |
| 18650 Li-ion Battery    | 1        | Energy storage                      |
| Solar Panel             | 1        | 5V or 6V, 10 Watt recommended       |
| Charge Controller       | 1        | CN3791 (MPPT) or TP5100 recommended |
| MT3608 Boost Converter  | 1        | Boosts battery to a stable 5V       |
| Jumper Wires            | Set      | Prototyping connections             |
| Breadboard              | 1        | Prototyping base                    |
| 4.7kΩ Resistor          | 1        | Pull-up resistor for DS18B20        |
----------------------------------------------------------------------------

## 🔌 Hardware Setup & Wiring

### Part 1: The Power System
This circuit provides a stable 5V to all components.
1.  Connect the **Solar Panel** to the **input** of your Charge Controller.
2.  Connect the **18650 Battery** to the **B+/B-** terminals of the Charge Controller.
3.  Connect the **OUT+/-** of the Charge Controller to the **VIN+/-** of the **MT3608 Boost Converter**.
4.  **Important:** Before connecting anything else, power the circuit and adjust the MT3608's potentiometer until the output (VOUT+/-) is exactly **5V**.

### Part 2: Powering the Breadboard & ESP32
1.  Connect the **VOUT+ (5V)** from the MT3608 to the **red (+) power rail** of the breadboard.
2.  Connect the **VOUT- (GND)** from the MT3608 to the **blue (-) ground rail** of the breadboard.
3.  Connect the breadboard's **red (+) rail** to the **VIN** pin on the ESP32.
4.  Connect the breadboard's **blue (-) rail** to a **GND** pin on the ESP32.

### Part 3: Component Wiring Diagram

| Component              | Pin / Wire              | Connects To                      |
---------------------------------------------------------------------------------------
| **DS18B20 Temp Sensor**| VCC (Red)               | ESP32 `3V3` Pin                  |
|                        |GND (Black)              | Breadboard GND Rail (-)          |                        
|                        |Data (Yellow)            | ESP32 `GPIO 4`                   | 
                                                   | (with 4.7kΩ pull-up to 3.3V)     |

| **Turbidity Sensor**   | VCC                     | Breadboard 5V Rail (+)           |
|                        |GND                      | Breadboard GND Rail (-)          |                      
|                        |Signal (AOUT)            | ESP32 `GPIO 34`                  |                        

| **MQ-135 Sensor**      | VCC                     | Breadboard 5V Rail (+)           |
|                        | GND                     | Breadboard GND Rail (-)          |
|                        | Signal (AOUT)           | ESP32 `GPIO 35`                  |

| **SG90 Servo Motor**   | Power (Red)             | Breadboard 5V Rail (+)           |
|                        | Ground (Brown)          | Breadboard GND Rail (-)          |
|                        | Signal (Orange)         | ESP32 `GPIO 22`                  |

| **Relay Module**       | VCC                     | Breadboard 5V Rail (+)           |
|                        | GND                     | Breadboard GND Rail (-)          |
|                        | IN (Signal)             | ESP32 `GPIO 23`                  |

| **LED Strip Power**    | 5V Source               | Relay `COM` Terminal             |
|                        | Relay `NO` Terminal     | LED Strip Positive (+)           |
|                        | LED Strip Negative (-)  | Breadboard GND Rail (-)          |
---------------------------------------------------------------------------------------
## ☁️ Blynk Setup Guide

### Phase 1: Datastream Configuration
In the Blynk web console, navigate to your Template and create the following Datastreams. All are **Virtual Pins**.

| Function         | Name                      | V-Pin  | Data Type | Details                          |
--------------------------------------------------------------------------------------------------------
| Light Control    | `Light Control`           | `V0`   | Integer   | Min: 0, Max: 1                   |
| Light Status     | `Light Status`            | `V1`   | Integer   | Min: 0, Max: 1                   |
| Feeder Control   | `Feeder Control`          | `V2`   | Integer   | Min: 0, Max: 1                   |
| Temperature      | `Temperature`             | `V10`  | Double    | Units: Celsius, Min: 0, Max: 50  |
| Turbidity        | `Turbidity`               | `V11`  | Integer   | Min: 0, Max: 4095                |
| Ammonia Level    | `Ammonia Level`           | `V12`  | Integer   | Min: 0, Max: 4095                |
| Battery Level    | `Battery Level`           | `V13`  | Integer   | Units: Percent, Min: 0, Max: 100 |
| Charging Status  | `Charging Status`         | `V14`  | String    | -                                |
| Terminal Log     | `Terminal Log`            | `V20`  | String    | -                                |
--------------------------------------------------------------------------------------------------------

### Phase 2: Web Dashboard Setup
Navigate to the **Web Dashboard** tab and drag widgets onto the canvas, connecting them to the Datastreams you just created.

| Widget Type       | Title          | Connect to Datastream  |
---------------------------------------------------------------
| **Switch**        | LED Lights     | `Light Control (V0)`   |
| **LED**           | Light Status   | `Light Status (V1)`    |
| **Button**        | Feed Fish      | `Feeder Control (V2)`  |
| **Gauge**         | Temperature    | `Temperature (V10)`    |
| **Gauge**         | Turbidity      | `Turbidity (V11)`      |
| **Gauge**         | Ammonia Level  | `Ammonia Level (V12)`  |
| **Gauge**         | Battery Level  | `Battery Level (V13)`  |
| **Value Display** | Charging Status| `Charging Status (V14)`|
| **Terminal**      | System Log     | `Terminal Log (V20)`   |
---------------------------------------------------------------

### Phase 3: Mobile Dashboard Setup
1.  Open the Blynk app on your phone.
2.  Select your project and tap the **wrench icon** to enter Developer Mode.
3.  Tap the canvas to open the Widget Box.
4.  Add widgets corresponding to the list above and connect them to the **same Datastreams**.
5.  Arrange the layout, then tap the wrench icon again to save and exit.

## 💻 Code Setup & Upload
1.  Open the main project `.ino` file in the Arduino IDE.
2.  Install the required libraries from the Library Manager: `Blynk`, `OneWire`, `DallasTemperature`, `ESP32Servo`.
3.  Fill in your personal credentials at the top of the code:
    ```cpp
    #define BLYNK_TEMPLATE_ID "YOUR_TEMPLATE_ID"
    #define BLYNK_TEMPLATE_NAME "Smart Aquaculture System"
    #define BLYNK_AUTH_TOKEN "YOUR_AUTH_TOKEN" // From Blynk

    char ssid[] = "YOUR_WIFI_SSID";
    char pass[] = "YOUR_WIFI_PASSWORD";
    ```
4.  In the Arduino IDE, go to **Tools > Board** and select your ESP32 model.
5.  Select the correct COM Port under **Tools > Port**.
6.  Click the **Upload** button.
7.  Open the **Serial Monitor** at **115200 baud** to see status messages.
