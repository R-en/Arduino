#define lmPin A0
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,16,2);
float tem = 0;
long lmVal =0;
void setup() {
  lcd.init();
  lcd.backlight();
}

void loop() {
  // put your main code here, to run repeatedly:
lmVal = analogRead(lmPin);
tem = (lmVal * 0.0048828125 *100);//5/1024
lcd.setCursor(5,0);
lcd.print("LM35");
lcd.setCursor(0,1);
lcd.print("Tem= ");
lcd.setCursor(5,1);
lcd.print(tem);
lcd.print(char(223));//print the unit ℃//degree
lcd.print("C");
delay(200);
}
