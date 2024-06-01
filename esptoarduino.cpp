uint timer =0;
void setup() {
  Serial.begin(9600);
  pinMode(12, OUTPUT);    // sets the digital pin 5 as output
  pinMode(34,INPUT);
}

void loop() {
 digitalWrite(12, HIGH); // sets the digital pin 13 on
 delay(1000);
//  int val=digitalRead(15);
 if(digitalRead(34)==HIGH){
   digitalWrite(12,LOW);
   Serial.println("12 OFF");
   delay(5000);
 }
//  digitalWrite(12,HIGH);
 delay(1000);
// digitalWrite(5,LOW);
// delay(1000);
          // waits for a second
}