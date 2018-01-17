const int photocellPin = A0;
int sensorValue = 0;
const int buzzerPin = 9;

void setup() {
  // put your setup code here, to run once:
pinMode(buzzerPin,OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
sensorValue = analogRead(photocellPin);
digitalWrite(buzzerPin,HIGH);
delay(sensorValue);
digitalWrite(buzzerPin,LOW);
delay(sensorValue);
}
