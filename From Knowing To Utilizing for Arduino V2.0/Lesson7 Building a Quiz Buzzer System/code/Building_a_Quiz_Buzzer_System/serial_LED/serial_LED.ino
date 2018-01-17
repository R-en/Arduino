const int greenPin = 2;
const int yellowPin = 3;
const int redPin = 4;
String comdata = "";
int lastLength = 0;

void setup() {
 pinMode(greenPin,OUTPUT);
 pinMode(yellowPin,OUTPUT);
 pinMode(redPin,OUTPUT);
 Serial.begin(9600);
 Serial.print("please input any color of LED: ");
}

void loop() {
if(Serial.available() > 0)
{
  comdata = "";
  while(Serial.available()>0 )
  {
    comdata += char(Serial.read());
    delay(2);
    }
    Serial.println(comdata);
  }
  
  if(comdata == "red"){
    digitalWrite(redPin,HIGH);
    digitalWrite(greenPin,LOW);
    digitalWrite(yellowPin,LOW);
  }
  else if (comdata == "yellow"){
    digitalWrite(redPin,LOW);
    digitalWrite(greenPin,LOW);
    digitalWrite(yellowPin,HIGH);
    
  } else if(comdata == "green"){
    digitalWrite(redPin,LOW);
    digitalWrite(greenPin,HIGH);
    digitalWrite(yellowPin,LOW);
    
  } else {
    digitalWrite(redPin,LOW);
    digitalWrite(greenPin,LOW);
    digitalWrite(yellowPin,LOW);
  }
 
}
