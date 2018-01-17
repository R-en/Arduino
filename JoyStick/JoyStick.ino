const int xPin = A0;
const int yPin = A1;
const int btPin = 7;

void setup() {
pinMode(btPin,INPUT);
digitalWrite(btPin,HIGH);
Serial.begin(9600);
}

void loop() {
Serial.print("X: ");
Serial.print(analogRead(xPin),DEC);//decimal
Serial.print("Y: ");
Serial.print(analogRead(yPin),DEC);
Serial.print("\tZ: ");
Serial.println(digitalRead(btPin));
delay(100);

}
