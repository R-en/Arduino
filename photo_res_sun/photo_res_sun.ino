const int NbrLEDs = 8;
const int ledPins[] = {5,6,7,8,9,10,11,12};
const int photocellPin = A0;
int sensorValue = 0;
int ledLevel = 0;

void setup() {
  // put your setup code here, to run once:
for(int led = 0; led< NbrLEDs;led++)
{
  pinMode(ledPins[led],OUTPUT);
  }
Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
sensorValue = analogRead(photocellPin);
//sensorValue = constrain(sensorValue,300,1023);
ledLevel = map(sensorValue,100,1023,NbrLEDs,0);
Serial.println(ledLevel);
delay(500);
for(int led = 0; led < NbrLEDs; led++){
  if(led < ledLevel)
  {
  digitalWrite(ledPins[led],HIGH);
  delay(2);
  }
  else
  {
    digitalWrite(ledPins[led],LOW);
      delay(2);
    }
}
}
