#include <SPI.h>
#include <MFRC522.h>
#include <Keypad.h>
#include <analogWrite.h>
 
#define SS_PIN 21
#define RST_PIN 22
MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.
 int in1=25;
 int in2=26;
 int in3=32;
 int in4=33;
 int en=27;
 

//keypad
#define ROW_NUM     4 // four rows
#define COLUMN_NUM  3 // three columns

char keys[ROW_NUM][COLUMN_NUM] = {
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'},
  {'*', '0', '#'}
};

byte pin_rows[ROW_NUM] = {5, 17, 16,4}; // GIOP18, GIOP5, GIOP17, GIOP16 connect to the row pins
byte pin_column[COLUMN_NUM] = {0, 2, 15};  // GIOP4, GIOP0, GIOP2 connect to the column pins

Keypad keypad = Keypad( makeKeymap(keys), pin_rows, pin_column, ROW_NUM, COLUMN_NUM );


//ultrasonic
const int trigPin = 14;
const int echoPin = 13;
//define sound velocity in cm/uS
#define SOUND_VELOCITY 0.034
#define CM_TO_INCH 0.393701

long duration;
float distanceCm=4;
float distanceInch;
float D;

void setup() 
{
  Serial.begin(115200);   // Initiate a serial communication
  SPI.begin();      // Initiate  SPI bus
  mfrc522.PCD_Init();   // Initiate MFRC522
  Serial.println("Put your card to the reader...");
  Serial.println();
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  pinMode(en, OUTPUT);
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
  
}

float ultrasonic()
{
     digitalWrite(trigPin, LOW);
     delayMicroseconds(2);
    // Sets the trigPin on HIGH state for 10 micro seconds
     digitalWrite(trigPin, HIGH);
     delayMicroseconds(10);   
     digitalWrite(trigPin, LOW);
     delayMicroseconds(2);
  
  // Reads the echoPin, returns the sound wave travel time in microseconds
    duration = pulseIn(echoPin, HIGH);
    delayMicroseconds(2);
  
  // Calculate the distance
    distanceCm = duration * SOUND_VELOCITY/2;
  
  // Prints the distance on the Serial Monitor
    Serial.print("Distance (cm): ");
    Serial.println(distanceCm);
    Serial.println("BB");
    return distanceCm;
}

void forward()
{
      digitalWrite(in1, HIGH);
      digitalWrite(in2, LOW);
      digitalWrite(in3, HIGH);
      digitalWrite(in4, LOW);
      analogWrite(en, 110);  
  }
  
void stop1()
{
     Serial.print("stop");
    digitalWrite(in1, HIGH);
    digitalWrite(in2, HIGH);
    digitalWrite(in3, HIGH);
    digitalWrite(in4, HIGH);
    delay(5000);
    
}
void left1()
{  digitalWrite(in1, LOW);
   digitalWrite(in2, HIGH);
   digitalWrite(in3, HIGH);
   digitalWrite(in4, LOW);
   analogWrite(en, 140);
   delay(3800);
  
  }
void left2()
{  digitalWrite(in1, HIGH);
   digitalWrite(in2, LOW);
   digitalWrite(in3, LOW);
   digitalWrite(in4, HIGH);
   analogWrite(en, 140);
   delay(3800);
  }
void forward1()
{
  
  come4:
  if ( ! mfrc522.PICC_IsNewCardPresent()) 
  { 

    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
    digitalWrite(in3, HIGH);
    digitalWrite(in4, LOW);
    analogWrite(en, 110);
    goto come4;
  
  }

  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) 
  {
    return;
    
  }
  //Show UID on serial monitor
  Serial.print("UID tag :");
  String content= "";
  byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
     Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
     Serial.print(mfrc522.uid.uidByte[i], HEX);
     content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }

  Serial.println();
  Serial.print("Message : ");
  content.toUpperCase();
  Serial.print(content);

    if(content.substring(1)=="1D 63 71 3A")
  {
    stop1();
    delay(10000);
  }
  goto come4;
  }


void backward()
{ 
  Serial.print("hww");
  come3:
  if ( ! mfrc522.PICC_IsNewCardPresent()) 
  { 
    Serial.println("Crd");
    D=ultrasonic();
    if(D>=4.00 && D<=10.00)
    {
      stop1();
     }
  else
  {
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);
    digitalWrite(in3, LOW);
    digitalWrite(in4, HIGH);
    analogWrite(en, 105);
  }
    goto come3;
  
  }

  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) 
  {
    return;
    
  }
  //Show UID on serial monitor
  Serial.print("UID tag :");
  String content= "";
  byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
     Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
     Serial.print(mfrc522.uid.uidByte[i], HEX);
     content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }

  Serial.println();
  Serial.print("Message : ");
  content.toUpperCase();
  Serial.print(content);

  if(content.substring(1)=="9D F6 61 3A")
  {   
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
    digitalWrite(in3, HIGH);
    digitalWrite(in4, LOW);
    analogWrite(en, 110); 
    delay(1000);
    
    left2();
    forward1();
    goto come3; 
  }

    if(content.substring(1)=="1D 63 71 3A")
  {
    stop1();
    delay(10000);
  }
  goto come3;
  }


void loop() 
{
    come2:
    char key = keypad.getKey();
    Serial.println(key);
    if (key){
    Serial.println("CC");
          

  // Look for new cards
  come1:
  if ( ! mfrc522.PICC_IsNewCardPresent()) 
  { 
    forward();
    goto come1;
  
  }

  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) 
  {
    goto come1;
  }
  
  //Show UID on serial monitor
  Serial.print("UID tag :");
  String content= "";
  byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
     Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
     Serial.print(mfrc522.uid.uidByte[i], HEX);
     content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }

  //Serial.println();
  //Serial.print("Message : ");
  content.toUpperCase();
  //Serial.print(content);

  if(content.substring(1)=="9D F6 61 3A")
  {
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
    digitalWrite(in3, HIGH);
    digitalWrite(in4, LOW);
    analogWrite(en, 110); 
    delay(1900);
    left2();
  }
  
   if(content.substring(1)=="7D FA 64 3A")
  {
    stop1();
    delay(5000);
    backward();
    delay(3000);
    goto come2;
    }
  goto come1;
 }
 }
