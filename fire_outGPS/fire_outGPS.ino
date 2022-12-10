#include <SoftwareSerial.h>
#include "SNIPE.h"

#define TXpin 9 //11 or D9
#define RXpin 8 //10 or D8
#define ATSerial Serial

//16byte hex key
String lora_app_key = "22 11 33 44 55 66 77 88 99 aa bb cc dd ee ff 00";  

int flame = A1;     //센서 연결
int buzzer = 2;     //buzzer 연결

String data = "";

SoftwareSerial DebugSerial(RXpin,TXpin);
SNIPE SNIPE(ATSerial);

void setup() {
  // put your setup code here, to run once:
  pinMode(flame, INPUT);
  pinMode(buzzer, OUTPUT);

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
  // put your main code here, to run repeatedly:

  int val = analogRead(flame);
  noTone(buzzer);
  DebugSerial.print("val: ");
  DebugSerial.println(val);


  if(val < 1023){
    data = "37.5829, 127.0107";
    tone(buzzer, 500, 500);
  }
  else
  {
    data = "No Fire";
    noTone(buzzer);
  }
  
  String recv = SNIPE.lora_recv();
  DebugSerial.println(recv);
  

    if (recv != "AT_RX_TIMEOUT" && recv != "AT_RX_ERROR")
    {
      
      if(recv == "Fire send")
      {
        if(SNIPE.lora_send(data))
       {
        DebugSerial.println("send success, fire node");
         //delay(1000);
       }
        else
       {
        DebugSerial.println("send fail, fire node");
        delay(500);
       }     
      }
    }
  
}
