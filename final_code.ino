/**
 * Restaurant Bot Control System
 * 
 * This code controls a restaurant service robot that navigates using RFID markers,
 * detects obstacles with ultrasonic sensors, and moves based on keypad input.
 * 
 * Components used:
 * - RFID RC522 Module
 * - 4x3 Keypad
 * - Ultrasonic distance sensor
 * - L298N motor driver
 */

 #include <SPI.h>
 #include <MFRC522.h>
 #include <Keypad.h>
 #include <analogWrite.h>
  
 // RFID pins
 #define RFID_SS_PIN 21
 #define RFID_RST_PIN 22
 MFRC522 rfidReader(RFID_SS_PIN, RFID_RST_PIN);  // Create RFID reader instance
 
 // Motor driver pins
 const int MOTOR_LEFT_FORWARD = 25;    // Left motor forward pin
 const int MOTOR_LEFT_BACKWARD = 26;   // Left motor backward pin
 const int MOTOR_RIGHT_FORWARD = 32;   // Right motor forward pin
 const int MOTOR_RIGHT_BACKWARD = 33;  // Right motor backward pin
 const int MOTOR_ENABLE = 27;          // Motor driver enable pin
 
 // Motor speed settings
 const int NORMAL_SPEED = 110;         // Normal movement speed
 const int TURN_SPEED = 140;           // Turning movement speed
 const int SLOW_SPEED = 105;           // Slow/reverse movement speed
 const int TURN_DURATION = 3800;       // Duration for turning (milliseconds)
 const int FORWARD_DURATION = 1900;    // Short forward movement duration
 const int STOP_DURATION = 5000;       // Duration to remain stopped
 const int EXTEND_STOP_DURATION = 10000;  // Extended stop duration
 
 // RFID card identifiers
 const String RFID_JUNCTION = "9D F6 61 3A";  // Junction/turn point card
 const String RFID_STOP = "7D FA 64 3A";      // Stop point card
 const String RFID_EXTENDED_STOP = "1D 63 71 3A";  // Extended stop card
 
 // Keypad configuration
 #define ROW_NUM     4  // Four rows
 #define COLUMN_NUM  3  // Three columns
 
 // Keypad key mapping
 char keys[ROW_NUM][COLUMN_NUM] = {
   {'1', '2', '3'},
   {'4', '5', '6'},
   {'7', '8', '9'},
   {'*', '0', '#'}
 };
 
 // Keypad pins
 byte rowPins[ROW_NUM] = {5, 17, 16, 4};      // Row pins of the keypad
 byte columnPins[COLUMN_NUM] = {0, 2, 15};    // Column pins of the keypad
 
 // Create keypad instance
 Keypad keypad = Keypad(makeKeymap(keys), rowPins, columnPins, ROW_NUM, COLUMN_NUM);
 
 // Ultrasonic sensor pins
 const int ULTRASONIC_TRIGGER_PIN = 14;
 const int ULTRASONIC_ECHO_PIN = 13;
 
 // Ultrasonic sensor constants
 #define SOUND_VELOCITY 0.034      // Sound velocity in cm/uS
 #define CM_TO_INCH 0.393701       // Conversion factor from cm to inches
 #define MIN_DISTANCE 4.00         // Minimum safe distance in cm
 #define MAX_DISTANCE 10.00        // Maximum detection distance in cm
 
 // Ultrasonic sensor variables
 long duration;                    // Duration of the pulse
 float distanceCm = 4;             // Measured distance in cm
 float distanceInch;               // Measured distance in inches
 float measuredDistance;           // Current measured distance
 
 void setup() {
   // Initialize serial communication
   Serial.begin(115200);
   
   // Initialize SPI bus
   SPI.begin();
   
   // Initialize RFID reader
   rfidReader.PCD_Init();
   
   // Output information to serial monitor
   Serial.println("Restaurant Bot Control System");
   Serial.println("Place RFID card to start...");
   Serial.println();
   
   // Configure motor driver pins
   pinMode(MOTOR_LEFT_FORWARD, OUTPUT);
   pinMode(MOTOR_LEFT_BACKWARD, OUTPUT);
   pinMode(MOTOR_RIGHT_FORWARD, OUTPUT);
   pinMode(MOTOR_RIGHT_BACKWARD, OUTPUT);
   pinMode(MOTOR_ENABLE, OUTPUT);
   
   // Configure ultrasonic sensor pins
   pinMode(ULTRASONIC_TRIGGER_PIN, OUTPUT);
   pinMode(ULTRASONIC_ECHO_PIN, INPUT);
 }
 
 /**
  * Measure distance using ultrasonic sensor
  * @return float Distance in centimeters
  */
 float measureDistance() {
   // Clear the trigger pin
   digitalWrite(ULTRASONIC_TRIGGER_PIN, LOW);
   delayMicroseconds(2);
   
   // Set the trigger pin high for 10 microseconds
   digitalWrite(ULTRASONIC_TRIGGER_PIN, HIGH);
   delayMicroseconds(10);
   digitalWrite(ULTRASONIC_TRIGGER_PIN, LOW);
   delayMicroseconds(2);
   
   // Read the echo pin (duration of sound wave travel)
   duration = pulseIn(ULTRASONIC_ECHO_PIN, HIGH);
   delayMicroseconds(2);
   
   // Calculate the distance
   distanceCm = duration * SOUND_VELOCITY / 2;
   
   // Print the distance to serial monitor
   Serial.print("Distance (cm): ");
   Serial.println(distanceCm);
   
   // Return the measured distance
   return distanceCm;
 }
 
 /**
  * Move the robot forward at normal speed
  */
 void moveForward() {
   digitalWrite(MOTOR_LEFT_FORWARD, HIGH);
   digitalWrite(MOTOR_LEFT_BACKWARD, LOW);
   digitalWrite(MOTOR_RIGHT_FORWARD, HIGH);
   digitalWrite(MOTOR_RIGHT_BACKWARD, LOW);
   analogWrite(MOTOR_ENABLE, NORMAL_SPEED);
 }
 
 /**
  * Stop the robot for a specified duration
  */
 void stopRobot() {
   Serial.print("Stopping robot");
   
   // Brake motors by setting both pins HIGH
   digitalWrite(MOTOR_LEFT_FORWARD, HIGH);
   digitalWrite(MOTOR_LEFT_BACKWARD, HIGH);
   digitalWrite(MOTOR_RIGHT_FORWARD, HIGH);
   digitalWrite(MOTOR_RIGHT_BACKWARD, HIGH);
   
   // Wait for stop duration
   delay(STOP_DURATION);
 }
 
 /**
  * Turn the robot left (counter-clockwise)
  */
 void turnLeft() {
   digitalWrite(MOTOR_LEFT_FORWARD, LOW);
   digitalWrite(MOTOR_LEFT_BACKWARD, HIGH);
   digitalWrite(MOTOR_RIGHT_FORWARD, HIGH);
   digitalWrite(MOTOR_RIGHT_BACKWARD, LOW);
   analogWrite(MOTOR_ENABLE, TURN_SPEED);
   delay(TURN_DURATION);
 }
 
 /**
  * Turn the robot right (clockwise)
  */
 void turnRight() {
   digitalWrite(MOTOR_LEFT_FORWARD, HIGH);
   digitalWrite(MOTOR_LEFT_BACKWARD, LOW);
   digitalWrite(MOTOR_RIGHT_FORWARD, LOW);
   digitalWrite(MOTOR_RIGHT_BACKWARD, HIGH);
   analogWrite(MOTOR_ENABLE, TURN_SPEED);
   delay(TURN_DURATION);
 }
 
 /**
  * Continue moving forward until RFID card detected
  */
 void moveForwardUntilRFID() {
   // Tag to mark beginning of RFID detection loop
   rfidDetectionLoop:
   
   // Check if there's a new card present
   if (!rfidReader.PICC_IsNewCardPresent()) {
     // If no card detected, continue moving forward
     moveForward();
     goto rfidDetectionLoop;
   }
 
   // Try to read the card
   if (!rfidReader.PICC_ReadCardSerial()) {
     return;
   }
   
   // Read and process the RFID card ID
   Serial.print("RFID Tag UID: ");
   String cardID = "";
   byte letter;
   
   // Extract the RFID card's UID
   for (byte i = 0; i < rfidReader.uid.size; i++) {
     Serial.print(rfidReader.uid.uidByte[i] < 0x10 ? " 0" : " ");
     Serial.print(rfidReader.uid.uidByte[i], HEX);
     cardID.concat(String(rfidReader.uid.uidByte[i] < 0x10 ? " 0" : " "));
     cardID.concat(String(rfidReader.uid.uidByte[i], HEX));
   }
 
   // Convert to uppercase for comparison
   Serial.println();
   Serial.print("Card ID: ");
   cardID.toUpperCase();
   Serial.print(cardID);
 
   // Check for extended stop card
   if (cardID.substring(1) == RFID_EXTENDED_STOP) {
     // Stop for extended duration
     stopRobot();
     delay(EXTEND_STOP_DURATION);
   }
   
   // Continue the RFID detection loop
   goto rfidDetectionLoop;
 }
 
 /**
  * Move the robot backward with obstacle avoidance until RFID card detected
  */
 void moveBackwardUntilRFID() {
   // Tag to mark beginning of backward movement loop
   backwardMovementLoop:
   
   // Check if there's a new card present
   if (!rfidReader.PICC_IsNewCardPresent()) {
     // Log card detection
     Serial.println("Checking for cards");
     
     // Measure distance with ultrasonic sensor
     measuredDistance = measureDistance();
     
     // Check if obstacle is detected within safe distance range
     if (measuredDistance >= MIN_DISTANCE && measuredDistance <= MAX_DISTANCE) {
       // Stop if obstacle detected
       stopRobot();
     } else {
       // Continue moving backward if no obstacle
       digitalWrite(MOTOR_LEFT_FORWARD, LOW);
       digitalWrite(MOTOR_LEFT_BACKWARD, HIGH);
       digitalWrite(MOTOR_RIGHT_FORWARD, LOW);
       digitalWrite(MOTOR_RIGHT_BACKWARD, HIGH);
       analogWrite(MOTOR_ENABLE, SLOW_SPEED);
     }
     
     // Continue the backward movement loop
     goto backwardMovementLoop;
   }
 
   // Try to read the card
   if (!rfidReader.PICC_ReadCardSerial()) {
     return;
   }
   
   // Read and process the RFID card ID
   Serial.print("RFID Tag UID: ");
   String cardID = "";
   byte letter;
   
   // Extract the RFID card's UID
   for (byte i = 0; i < rfidReader.uid.size; i++) {
     Serial.print(rfidReader.uid.uidByte[i] < 0x10 ? " 0" : " ");
     Serial.print(rfidReader.uid.uidByte[i], HEX);
     cardID.concat(String(rfidReader.uid.uidByte[i] < 0x10 ? " 0" : " "));
     cardID.concat(String(rfidReader.uid.uidByte[i], HEX));
   }
 
   // Convert to uppercase for comparison
   Serial.println();
   Serial.print("Card ID: ");
   cardID.toUpperCase();
   Serial.print(cardID);
 
   // Check for junction card
   if (cardID.substring(1) == RFID_JUNCTION) {
     // Move forward for a short distance
     digitalWrite(MOTOR_LEFT_FORWARD, HIGH);
     digitalWrite(MOTOR_LEFT_BACKWARD, LOW);
     digitalWrite(MOTOR_RIGHT_FORWARD, HIGH);
     digitalWrite(MOTOR_RIGHT_BACKWARD, LOW);
     analogWrite(MOTOR_ENABLE, NORMAL_SPEED);
     delay(1000);
     
     // Turn right at junction
     turnRight();
     
     // Continue forward after turning
     moveForwardUntilRFID();
     
     // Continue the backward movement loop
     goto backwardMovementLoop;
   }
 
   // Check for extended stop card
   if (cardID.substring(1) == RFID_EXTENDED_STOP) {
     // Stop for extended duration
     stopRobot();
     delay(EXTEND_STOP_DURATION);
   }
   
   // Continue the backward movement loop
   goto backwardMovementLoop;
 }
 
 void loop() {
   // Tag to mark beginning of main loop
   mainLoop:
   
   // Read keypad input
   char key = keypad.getKey();
   Serial.println(key);
   
   // Process keypad input if available
   if (key) {
     Serial.println("Keypad input received");
     
     // Start navigation after keypad input
     navigationLoop:
     
     // Check if there's a new card present
     if (!rfidReader.PICC_IsNewCardPresent()) {
       // Continue moving forward while looking for RFID cards
       moveForward();
       goto navigationLoop;
     }
 
     // Try to read the card
     if (!rfidReader.PICC_ReadCardSerial()) {
       goto navigationLoop;
     }
     
     // Read and process the RFID card ID
     Serial.print("RFID Tag UID: ");
     String cardID = "";
     byte letter;
     
     // Extract the RFID card's UID
     for (byte i = 0; i < rfidReader.uid.size; i++) {
       Serial.print(rfidReader.uid.uidByte[i] < 0x10 ? " 0" : " ");
       Serial.print(rfidReader.uid.uidByte[i], HEX);
       cardID.concat(String(rfidReader.uid.uidByte[i] < 0x10 ? " 0" : " "));
       cardID.concat(String(rfidReader.uid.uidByte[i], HEX));
     }
 
     // Convert to uppercase for comparison
     cardID.toUpperCase();
 
     // Check if junction card detected
     if (cardID.substring(1) == RFID_JUNCTION) {
       // Move forward for a short distance
       digitalWrite(MOTOR_LEFT_FORWARD, HIGH);
       digitalWrite(MOTOR_LEFT_BACKWARD, LOW);
       digitalWrite(MOTOR_RIGHT_FORWARD, HIGH);
       digitalWrite(MOTOR_RIGHT_BACKWARD, LOW);
       analogWrite(MOTOR_ENABLE, NORMAL_SPEED);
       delay(FORWARD_DURATION);
       
       // Turn right at junction
       turnRight();
     }
     
     // Check if stop card detected
     if (cardID.substring(1) == RFID_STOP) {
       // Stop the robot
       stopRobot();
       
       // Move backward for a set duration
       delay(STOP_DURATION);
       moveBackwardUntilRFID();
       delay(3000);
       
       // Return to main loop
       goto mainLoop;
     }
     
     // Continue navigation
     goto navigationLoop;
   }
 }