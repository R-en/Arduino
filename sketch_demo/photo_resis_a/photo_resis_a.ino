int sensePin = 0;
int ledPin = 9;
void setup() {
  // put your setup code here, to run once:
  pinMode(ledPin,OUTPUT);
Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  int val = analogRead(sensePin);
  val = constrain(val,300,590);
  int ledLev = map(val,300,590,255,0);
  
  analogWrite(ledPin,ledLev);
  
}
