const int latchPin = 12;
const int clockPin = 8;
const int dataPin = 11;

//1,2,3,4,5,6,7,8,9,a,b,c,d,e,f
int datArray[16] = {252, 96, 218, 242, 102, 182, 190, 224, 254, 246, 238, 62, 156, 122, 158, 142};

void setup() {
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
}

void loop() {
  for (int num = 0; num < 16; num++)
  {
    digitalWrite(latchPin, LOW);; //ground latchPin and hold low as long as you are transimitting
    shiftOut(dataPin, clockPin, MSBFIRST, datArray[num]);
    //return the latch pin to high if no longer needs to listen for info
    digitalWrite(latchPin, HIGH);
    delay(1000);
  }
}
