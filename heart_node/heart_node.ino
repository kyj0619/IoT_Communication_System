#define USE_ARDUINO_INTERRUPTS true
#include <PulseSensorPlayground.h>
#include <LiquidCrystal_I2C.h>   
#include <SoftwareSerial.h>
#include "SNIPE.h"    


#define TXpin 11 //11 or D9
#define RXpin 10 //10 or D8
#define ATSerial Serial

//16byte hex key
String lora_app_key = "22 11 33 44 55 66 77 88 99 aa bb cc dd ee ff 00";

SoftwareSerial DebugSerial(RXpin,TXpin);
SNIPE SNIPE(ATSerial);

const int PulseWire = A0;    
const int buzzer = 9; 
int Threshold = 30;
int sensor = A3;   

// This Node == Heart
                               
PulseSensorPlayground pulseSensor;
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {   

  //Serial.begin(9600);
  pinMode(buzzer,HIGH);       
  pulseSensor.analogInput(PulseWire);          
  pulseSensor.setThreshold(Threshold);   

   if (pulseSensor.begin()) {
    Serial.println("We created a pulseSensor Object !"); 
  }
  Serial.begin(115200);
  lcd.init();
  lcd.backlight();

  ATSerial.begin(115200);

  // put your setup code here, to run once:
  while(ATSerial.read()>= 0) {}
  while(!ATSerial);

  DebugSerial.begin(115200);

  /* SNIPE LoRa Initialization */
  if (!SNIPE.lora_init()) {
    DebugSerial.println("SNIPE LoRa Initialization Fail!");
    while (1);
  }

  /* SNIPE LoRa Set Appkey */
  if (!SNIPE.lora_setAppKey(lora_app_key)) {
    DebugSerial.println("SNIPE LoRa app key value has not been changed");
  }
  
  /* SNIPE LoRa Set Frequency */
  if (!SNIPE.lora_setFreq(LORA_CH_1)) {
    DebugSerial.println("SNIPE LoRa Frequency value has not been changed");
  }

  /* SNIPE LoRa Set Spreading Factor */
  if (!SNIPE.lora_setSf(LORA_SF_7)) {
    DebugSerial.println("SNIPE LoRa Sf value has not been changed");
  }

  /* SNIPE LoRa Set Rx Timeout 
   * If you select LORA_SF_12, 
   * RX Timout use a value greater than 5000  
  */
  if (!SNIPE.lora_setRxtout(5000)) {
    DebugSerial.println("SNIPE LoRa Rx Timout value has not been changed");
  }  
    
  DebugSerial.println("SNIPE LoRa Message Test");
}



void loop() {

 int myBPM = pulseSensor.getBeatsPerMinute(); 


if (myBPM > Threshold) {            
 DebugSerial.println("A HeartBeat Happened ! ");
 DebugSerial.println("BPM: ");               
 DebugSerial.println(myBPM);  
 digitalWrite(buzzer, HIGH);

  String recv = SNIPE.lora_recv();
    
   if (recv != "AT_RX_TIMEOUT" && recv != "AT_RX_ERROR")
    {
      DebugSerial.println(recv);
      if(recv == "Heart send")
      {
        if(SNIPE.lora_send(String(myBPM)))
       {
        DebugSerial.println("send success, heart node");
        //delay(1000);
       }
        else
       {
        DebugSerial.println("send fail, heart node");
        delay(500);
       }     
      }
    }
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
