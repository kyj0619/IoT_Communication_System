#include <LiquidCrystal_I2C.h>

int sensor = A3;

LiquidCrystal_I2C lcd(0x27, 16, 2);
void setup() {
  Serial.begin(115200);
  lcd.init();
  lcd.backlight();

}

void loop() {
  if(analogRead(sensor)<50) {
    lcd.setCursor(0,0);
    lcd.print("more pressure");    
  }
  else {
    lcd.setCursor(0,0);
    lcd.print("keep going");
  }
  int value = analogRead(A0);
  
  delay(500);
  lcd.clear();

}