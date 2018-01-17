const int a = 7;
const int b = 6;
const int c = 5;
const int d = 11;
const int e = 10;
const int f = 8;
const int g = 9;
const int dp  = 4;

void setup() {
  for (int startPin = 4; startPin <= 11; startPin++) 
  {
    pinMode(startPin, OUTPUT);
    }
  }

  void loop() {
  digital_0();//diaplay 0 to the 7-segment
  delay(1000);
  digital_1();//diaplay 1 to the 7-segment
  delay(1000);
  digital_2();//diaplay 2 to the 7-segment
  delay(1000);
  digital_3();//diaplay 3 to the 7-segment
  delay(1000);
  digital_4();//diaplay 4 to the 7-segment
  delay(1000);
  digital_5();//diaplay 5 to the 7-segment
  delay(1000);
  digital_6();//diaplay 6 to the 7-segment
  delay(1000);
  digital_7();//diaplay 7 to the 7-segment
  delay(1000);
  digital_8();//diaplay 8 to the 7-segment
  delay(1000);
  digital_9();//diaplay 9 to the 7-segment
  delay(1000);
  digital_A();//diaplay A to the 7-segment
  delay(1000);
}

void digital_0(void) {
  unsigned char j;
  for ( j = 5 ; j <= 11; j++) {
    digitalWrite(g, LOW);
    digitalWrite(j, HIGH);
    digitalWrite(dp, LOW);
  }
}

void digital_1(void) {
  unsigned char j;
  digitalWrite(b, HIGH);
  digitalWrite(c, HIGH);
  
  for ( j = 7; j <= 11; j++) 
  {
    digitalWrite(j, LOW);
  }
  
  digitalWrite(dp, LOW);
}
void digital_2(void) {
  unsigned char j;
  for ( j = 5 ; j <= 11; j++)
  {
    digitalWrite(j, HIGH);
    digitalWrite(c, LOW);
    digitalWrite(f, LOW);
    digitalWrite(dp, LOW);
  }
}

void digital_3(void) {
  unsigned char j;
  for ( j = 5 ; j <= 11; j++)
  {
    digitalWrite(j, HIGH);
    digitalWrite(e, LOW);
    digitalWrite(f, LOW);
    digitalWrite(dp, LOW);
  }
}

void digital_4(void) {
  unsigned char j;
  for ( j = 5 ; j <= 11; j++) 
  {
    digitalWrite(j, HIGH);
    digitalWrite(a, LOW);
    digitalWrite(e, LOW);
    digitalWrite(d, LOW);
    digitalWrite(dp, LOW);
  }
}

void digital_5(void) {
  unsigned char j;
  for ( j = 5 ; j <= 11; j++) 
  {
    digitalWrite(j, HIGH);
    digitalWrite(b, LOW);
    digitalWrite(e, LOW);
    digitalWrite(dp, LOW);

  }
}

void digital_6(void) {
  unsigned char j;
  for ( j = 5 ; j <= 11; j++)
  {
    digitalWrite(j, HIGH);
    digitalWrite(a, LOW);
    digitalWrite(b, LOW);
    digitalWrite(dp, LOW);

  }
}

void digital_7(void) {
  unsigned char j;
  digitalWrite(a, HIGH);
  digitalWrite(b, HIGH);
  digitalWrite(c, HIGH);

  for ( j = 8 ; j <= 11; j++) 
  {
    digitalWrite(j, LOW);
    digitalWrite(dp, LOW);
  }
}

void digital_8(void) {
  unsigned char j;
  for ( j = 5 ; j <= 11; j++) 
  {
    digitalWrite(j, HIGH);
    digitalWrite(dp, LOW);

  }
}

void digital_9(void) {
  unsigned char j;
  for ( j = 5 ; j <= 11; j++)
  {
    digitalWrite(j, HIGH);
    digitalWrite(e, LOW);
    digitalWrite(d, LOW);
    digitalWrite(dp, LOW);
  }
}

void digital_A(void) {
  unsigned char j;
  for ( j = 5 ; j <= 11; j++) 
  {
    digitalWrite(j, HIGH);
    digitalWrite(d, LOW);
    digitalWrite(dp, LOW);
  }
}

