const int ledPin = 13;
void setup() {
  // put your setup code here, to run once:
pinMode(2,INPUT);
pinMode(13,OUTPUT);
digitalWrite(2,HIGH);
}

void loop() {
  // put your main code here, to run repeatedly:
  int digitalVal = digitalRead(2);
if(digitalVal == HIGH)//not breakover
digitalWrite(ledPin,LOW);
else
digitalWrite(ledPin,HIGH);//breakover
}
