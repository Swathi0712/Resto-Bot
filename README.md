# Restaurant Bot Automated Management System

## Overview
The Restaurant Bot Automated Management System is an intelligent robotic solution designed to automate various service tasks in a restaurant environment. The system uses a combination of hardware components including RFID tags for navigation, ultrasonic sensors for obstacle detection, stepper motors for mechanical operations, and a keypad for user input.

## Key Features
- **Autonomous Navigation**: Uses RFID tags placed on the floor to navigate through the restaurant
- **Obstacle Detection**: Utilizes ultrasonic sensors to detect and avoid obstacles in its path
- **Automated Serving**: Features a tray lift mechanism controlled by stepper motors
- **User Input Interface**: Provides a keypad for staff to input commands
- **Multiple Operation Modes**: Can operate in forward, backward, and turning modes based on the detected RFID tags

## System Architecture

### Hardware Components
- **Microcontrollers**:
  - Arduino board (for sensor reading and basic control)
  - ESP32 board (for advanced processing and communication)
- **Sensors**:
  - RFID Reader (MFRC522 module)
  - Ultrasonic Distance Sensor
  - Analog Voltage Sensor
- **Actuators**:
  - L298N Motor Driver
  - NEMA23 Stepper Motor (main movement)
  - NEMA17 Stepper Motor (secondary movement)
- **Input/Output**:
  - 4x3 Keypad
  - Serial Communication between ESP32 and Arduino

### Software Components
The system consists of four main software modules:

1. **Arduino to ESP Communication** (`arduinotoesp.cpp`):
   - Reads analog sensor values and sends to ESP32
   - Controls output signals based on threshold values

2. **ESP to Arduino Communication** (`esptoarduino.cpp`):
   - Processes input signals from Arduino
   - Controls output signals based on the processed data

3. **Main Control System** (`finalcode.ino`):
   - Handles RFID tag detection for navigation
   - Processes user input from keypad
   - Controls motor movements based on detected tags
   - Implements obstacle avoidance using ultrasonic sensor

4. **Stepper Motor Control** (`stepper.cpp`):
   - Controls the movement of two stepper motors
   - Executes precise movement sequences for serving operations

## Installation and Setup

### Hardware Setup
1. Connect the L298N motor driver to the ESP32:
   - Connect IN1 to GPIO25
   - Connect IN2 to GPIO26
   - Connect IN3 to GPIO32
   - Connect IN4 to GPIO33
   - Connect ENA to GPIO27

2. Connect the RFID module to the ESP32:
   - Connect SS to GPIO21
   - Connect RST to GPIO22
   - Connect MOSI, MISO, and SCK to appropriate SPI pins

3. Connect the keypad to the ESP32:
   - Connect row pins to GPIO5, GPIO17, GPIO16, GPIO4
   - Connect column pins to GPIO0, GPIO2, GPIO15

4. Connect the ultrasonic sensor to the ESP32:
   - Connect TRIG to GPIO14
   - Connect ECHO to GPIO13

5. For stepper motor control:
   - Connect NEMA23 DIR pin to Arduino digital pin 6
   - Connect NEMA23 STEP pin to Arduino digital pin 7
   - Connect NEMA17 DIR pin to Arduino digital pin 10
   - Connect NEMA17 STEP pin to Arduino digital pin 9

### Software Setup
1. Install the required libraries:
   - SPI.h
   - MFRC522.h
   - Keypad.h
   - analogWrite.h
   - AccelStepper.h
   - SoftwareSerial.h

2. Upload the corresponding code to each microcontroller:
   - `arduinotoesp.cpp` to the Arduino connected to the analog sensor
   - `esptoarduino.cpp` to the ESP32 for signal processing
   - `finalcode.ino` to the main ESP32 controlling navigation
   - `stepper.cpp` to the Arduino controlling the stepper motors

## Operation Instructions

### Basic Operation
1. Place RFID tags at strategic locations:
   - Junction points (for turning)
   - Stop points (for temporary stops)
   - Extended stop points (for longer service periods)

2. Power on the system and wait for initialization
   
3. Use the keypad to enter commands:
   - Any key press will initiate the navigation sequence

### Navigation Sequence
The robot will follow these behaviors:
1. Move forward until an RFID tag is detected
2. Based on the detected tag:
   - At junctions: Turn right and continue forward
   - At stop points: Stop, wait, then move backward
   - At extended stop points: Stop for an extended period

### Mechanical Operation Sequence
When triggered by the analog sensor:
1. The system waits for a set period
2. The secondary stepper motor extends
3. The main stepper motor extends
4. The secondary stepper returns to home position
5. The control signal is activated
6. After a delay, the main stepper returns to home position

## RFID Tag Configuration
- **Junction Tag UID**: 9D F6 61 3A
- **Stop Tag UID**: 7D FA 64 3A 
- **Extended Stop Tag UID**: 1D 63 71 3A

## Troubleshooting

### Common Issues and Solutions

1. **Robot not detecting RFID tags**
   - Ensure the RFID reader module is properly connected
   - Check if the tags are placed within the detection range
   - Verify that the tags are using the correct UID format

2. **Stepper motors not moving**
   - Check the power supply to the stepper motor drivers
   - Verify that the correct pins are connected
   - Ensure the AccelStepper library is properly installed
   - Check if the stepper motor driver current is set correctly

3. **Robot not stopping at obstacles**
   - Check ultrasonic sensor connections
   - Verify that the MIN_DISTANCE and MAX_DISTANCE values are set appropriately
   - Test the ultrasonic sensor separately to confirm functionality

4. **Motors running but robot not moving**
   - Check if the wheels are properly connected to the motors
   - Verify that the motor driver is receiving sufficient power
   - Check if the MOTOR_ENABLE value is high enough for movement

## Maintenance

### Regular Maintenance Tasks

1. **Weekly Tasks**
   - Check and tighten all wheel connections
   - Test all RFID tag placements and responses
   - Clean ultrasonic sensor to ensure accurate readings

2. **Monthly Tasks**
   - Check battery levels and recharge/replace as needed
   - Update RFID tag locations if restaurant layout has changed
   - Calibrate the ultrasonic sensor distance readings
   - Lubricate mechanical parts of the stepper motor assemblies

## Future Improvements

1. **Advanced Navigation**
   - Implement dynamic path finding algorithms
   - Add more RFID tag types for different restaurant zones
   - Integrate with restaurant table mapping

2. **Enhanced User Interface**
   - Develop a touchscreen interface for staff
   - Create a mobile app for remote control and monitoring
   - Add voice commands for easier interaction

3. **Improved Sensing**
   - Add multiple ultrasonic sensors for 360° obstacle detection
   - Integrate light sensors for ambient light adaptation
   - Add thermal sensors for hot dish detection

## System Parameters

### Motor Control Parameters
- Normal Movement Speed: 110 (PWM value)
- Turning Speed: 140 (PWM value)
- Slow/Reverse Speed: 105 (PWM value)
- Turn Duration: 3800 ms
- Stop Duration: 5000 ms

### Sensor Parameters
- Voltage Threshold: 3.0V
- Minimum Safe Distance: 4.0 cm
- Maximum Detection Distance: 10.0 cm

### Stepper Motor Parameters
- Maximum Speed: 1000 steps/second
- Acceleration: 500 steps/second²
- Main Stepper Extended Position: 10000 steps
- Secondary Stepper Extended Position: -12000 steps

## License
This project is proprietary and confidential. All rights reserved.

## Acknowledgments
- Thanks to all team members who contributed to this project
- Special thanks to the restaurant staff for testing and feedback