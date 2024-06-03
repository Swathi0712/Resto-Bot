/**
 * ESP to Arduino Communication
 * 
 * This code controls an output signal based on input from another pin.
 * It demonstrates basic communication between ESP32 and Arduino.
 */

// Global variables
unsigned int cycleTimer = 0;  // Timer for tracking cycles

// Pin definitions
const int OUTPUT_SIGNAL_PIN = 12;  // Digital pin to send signal to Arduino
const int INPUT_SENSOR_PIN = 34;   // Digital pin to read sensor data

// Timing settings
const int SIGNAL_ON_DURATION = 1000;   // Duration to keep signal HIGH in milliseconds
const int SIGNAL_OFF_DURATION = 5000;  // Duration to keep signal LOW in milliseconds when triggered
const int CHECK_DELAY = 1000;          // Delay between checks in milliseconds

void setup() {
  // Initialize serial communication at 9600 baud rate
  Serial.begin(9600);
  
  // Configure the output and input pins
  pinMode(OUTPUT_SIGNAL_PIN, OUTPUT);  // Set output pin
  pinMode(INPUT_SENSOR_PIN, INPUT);    // Set input pin
}

void loop() {
  // Set output signal HIGH
  digitalWrite(OUTPUT_SIGNAL_PIN, HIGH);
  
  // Wait for specified delay
  delay(SIGNAL_ON_DURATION);
  
  // Check if input sensor is HIGH
  if (digitalRead(INPUT_SENSOR_PIN) == HIGH) {
    // Set output signal LOW
    digitalWrite(OUTPUT_SIGNAL_PIN, LOW);
    
    // Log to serial monitor
    Serial.println("12 OFF");
    
    // Wait for specified off duration
    delay(SIGNAL_OFF_DURATION);
  }
  
  // Wait for check delay before next cycle
  delay(CHECK_DELAY);
}