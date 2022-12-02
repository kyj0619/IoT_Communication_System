#define USE_ARDUINO_INTERRUPTS true
#include <PulseSensorPlayground.h>
#include <LiquidCrystal_I2C.h>       


const int PulseWire = A0;    
const int buzzer = 9; 
int Threshold = 30;
int sensor = A3;         
                               
PulseSensorPlayground pulseSensor;
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {   

  Serial.begin(9600);
  pinMode(buzzer,HIGH);       
  pulseSensor.analogInput(PulseWire);          
  pulseSensor.setThreshold(Threshold);   

   if (pulseSensor.begin()) {
    Serial.println("We created a pulseSensor Object !"); 
  }
  Serial.begin(115200);
  lcd.init();
  lcd.backlight();
}



void loop() {

 int myBPM = pulseSensor.getBeatsPerMinute(); 


if (myBPM > Threshold) {            
 Serial.println("A HeartBeat Happened ! ");
 Serial.print("BPM: ");               
 Serial.println(myBPM);  
 digitalWrite(buzzer, HIGH);                 
}

else {
  digitalWrite(buzzer,LOW);
}

  delay(20);                 

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
