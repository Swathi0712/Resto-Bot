void setup() {
  // initialize digital pin LED_BUILTIN as an output.
Serial.begin(9600);
pinMode(12,OUTPUT);
}

// the loop function runs over and over again forever
void loop() {
int sensorValue = analogRead(A0);
float voltage= sensorValue * (5.0 / 1023.0);
Serial.println(voltage);
delay(1000);
if(voltage>3.0)
{
  // delay(10000);
  digitalWrite(12,HIGH);
  delay(5000);

}

digitalWrite(12,LOW);
delay(4000);

}