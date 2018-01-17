const int ledPin = 9;
void setup() {
  // put your setup code here, to run once:
pinMode(ledPin,OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
for(int a = 0; a <= 255; a++)
{
  analogWrite(ledPin, a);
  delay(9);
}

for(int a = 255; a >= 0; a--){
  analogWrite(ledPin,a);
  delay(9);
  }

  delay(800);

}
