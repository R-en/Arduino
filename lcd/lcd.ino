#include <Wire.h>
#include <LiquidCrystal_I2C.h>;

char array1[] = "你好!私は任です                  ";
char array2[] = "NEP,NEP,NEP,NEP,NEP!      ";
int tim = 500;

LiquidCrystal_I2C lcd(0x27, 16, 2); //addr 0 x27 char 16 line 2
void setup() {
  lcd.init();
  lcd.backlight();
}

void loop() {
  lcd.setCursor(15, 0); //col 15 line 0
  for (int positionCounter1 = 0; positionCounter1 < 26 ; positionCounter1++) {
    lcd.scrollDisplayLeft();
    lcd.print(array1[positionCounter1]);
    delay(tim);
  }

  lcd.clear();
  lcd.setCursor(15, 1);

  for ( int positionCounter = 0; positionCounter < 26; positionCounter++) {
    lcd.scrollDisplayLeft();
    lcd.print(array2[positionCounter]);
    delay(tim);
  }
  lcd.clear();
}

