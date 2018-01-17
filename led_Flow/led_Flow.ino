int ledNum = 8;
byte ledPin[] = { 2, 3, 4, 5, 6, 7, 8, 9};
int ledDelay;
int direction = 1;
int currentLED = 0;
unsigned long changeTime;
int potPin = 0;


void setup() {
  for (int x = 0; x < ledNum; x++) {
    pinMode(ledPin[x], OUTPUT);
  }
  changeTime = millis();
}

void loop() {
  ledDelay = analogRead(potPin);

  if ((millis() - changeTime) > ledDelay) {
    changeLED();
    changeTime = millis();
  }
}

void changeLED()
{
  for (int x = 0; x < ledNum; x++) {
    digitalWrite(ledPin[x], LOW);
  }
  digitalWrite(ledPin[currentLED], HIGH);
  currentLED += direction;
  if (currentLED == ledNum - 1)
  {
    direction = -1;
  }
  if ( currentLED ==  0) {
    direction = 1;
  }
}

