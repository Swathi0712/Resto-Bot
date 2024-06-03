/**
 * Arduino to ESP Communication
 * 
 * This code reads an analog sensor value and sends it to an ESP32 via serial communication.
 * It also controls an LED based on the sensor value threshold.
 */

// Pin definitions
const int OUTPUT_SIGNAL_PIN = 12;  // Digital pin to send signal to ESP
const int ANALOG_SENSOR_PIN = A0;  // Analog pin to read sensor data

// Threshold settings
const float VOLTAGE_THRESHOLD = 3.0;  // Threshold voltage to trigger output signal
const int SIGNAL_DURATION = 5000;     // Duration to keep signal HIGH in milliseconds
const int LOOP_DELAY = 1000;          // Delay between sensor readings in milliseconds
const int RESET_DELAY = 4000;         // Delay after setting signal LOW in milliseconds

void setup() {
  // Initialize serial communication at 9600 baud rate
  Serial.begin(9600);
  
  // Configure the output signal pin as OUTPUT
  pinMode(OUTPUT_SIGNAL_PIN, OUTPUT);
}

void loop() {
  // Read analog sensor value (range 0-1023)
  int sensorValue = analogRead(ANALOG_SENSOR_PIN);
  
  // Convert sensor value to voltage (0-5V range)
  float voltage = sensorValue * (5.0 / 1023.0);
  
  // Send voltage value to serial monitor/ESP
  Serial.println(voltage);
  
  // Wait for specified delay
  delay(LOOP_DELAY);
  
  // Check if voltage is above threshold
  if (voltage > VOLTAGE_THRESHOLD) {
    // Set output signal HIGH for SIGNAL_DURATION
    digitalWrite(OUTPUT_SIGNAL_PIN, HIGH);
    delay(SIGNAL_DURATION);
  }
  
  // Reset output signal to LOW
  digitalWrite(OUTPUT_SIGNAL_PIN, LOW);
  
  // Wait for reset delay before next reading
  delay(RESET_DELAY);
}