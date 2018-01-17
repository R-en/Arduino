const int redPin = 11;
const int greenPin = 10;
const int bluePin = 9;

void setup() {
pinMode(redPin,OUTPUT);
pinMode(bluePin,OUTPUT);
pinMode(greenPin,OUTPUT);
}

void loop() {
color(0,255,255);//red
delay(1000);
color(255,0,255);//green
delay(1000);
color(255,255,0);//blue
delay(1000);

//blends
color(0,125,255);
delay(1000);
color(125,255,255);
delay(1000);
color(0,255,0);
delay(1000);
color(0,255,125);
delay(1000);
color(0,123,255);
delay(1000);
color(124,123,125);
delay(1000);
color(0,0,0);
delay(1000);
color(255,255,255);
delay(1000);
}

void color(unsigned char red,unsigned char green,unsigned char blue)
{
  analogWrite(redPin,red);
  analogWrite(greenPin,green);
  analogWrite(bluePin,blue);
  }


