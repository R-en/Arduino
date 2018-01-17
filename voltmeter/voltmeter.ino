#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,16,2);
float val = 0;

void setup() {
Serial.begin(9600);
lcd.init();
lcd.backlight();
lcd.print("Voltage Value: ");

}

void loop() {
val = analogRead(A0);
val =val/1024*5.0;
Serial.print(val);
Serial.print("V");
lcd.setCursor(6,1);
Serial.println();
lcd.setCursor(6,1);
lcd.print(val);
lcd.print("V");
delay(200);
}
