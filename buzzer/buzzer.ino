int button = 10;
int buzzer = 12;

boolean lastButton = LOW;
boolean current = false;
void setup() {
  // put your setup code here, to run once:
pinMode(button,INPUT);
pinMode(buzzer,OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
if(digitalRead(button) == HIGH  && lastButton == LOW){
    current = !current; 
    lastButton = HIGH;
  }
  else{
    lastButton = digitalRead(button);
    }
   digitalWrite(buzzer,current);
   delay(100);

}
