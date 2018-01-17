const int analogPin = A0;
const int ledPin = 9;
int inputValue = 0;
int outputValue = 0;
void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:
inputValue = analogRead(analogPin);
outputValue = map(inputValue,0, 1023,0,255);
analogWrite(ledPin,outputValue);
}
